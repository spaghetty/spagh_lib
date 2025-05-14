use burn::backend::{Autodiff, NdArray};
use burn::data::dataloader::DataLoaderBuilder;
use burn::data::dataloader::batcher::Batcher;
use burn::data::dataset::vision::{MnistDataset, MnistItem};
use burn::module::Module;
use burn::nn::loss::CrossEntropyLossConfig;
use burn::optim::SgdConfig;
use burn::optim::momentum::MomentumConfig;
use burn::prelude::*;
use burn::record::CompactRecorder;
use burn::tensor::{Tensor, backend::AutodiffBackend, backend::Backend};
use burn::train::metric::{AccuracyMetric, LossMetric};
use burn::train::{ClassificationOutput, LearnerBuilder, TrainOutput, TrainStep, ValidStep};
use std::time::Instant;

type MyBackend = Autodiff<NdArray>;
const DIM: usize = 3;
const ARTIFACT_DIR: &str = "data/";
const NUM_CLASSES: u8 = 2;

const WIDTH: usize = 28;
const HEIGHT: usize = 28;

#[derive(Clone, Default)]
pub struct MnistBatcher {}

#[derive(Clone, Debug)]
pub struct MnistBatch<B: Backend> {
    pub images: Tensor<B, 3>,
    pub targets: Tensor<B, 1, Int>,
}

impl<B: Backend> Batcher<B, MnistItem, MnistBatch<B>> for MnistBatcher {
    fn batch(&self, items: Vec<MnistItem>, device: &B::Device) -> MnistBatch<B> {
        let images = items
            .iter()
            .map(|item| TensorData::from(item.image).convert::<B::FloatElem>())
            .map(|data| Tensor::<B, 2>::from_data(data, device))
            .map(|tensor| tensor.reshape([1, 28, 28]))
            // Normalize: scale between [0,1] and make the mean=0 and std=1
            // values mean=0.1307,std=0.3081 are from the PyTorch MNIST example
            // https://github.com/pytorch/examples/blob/54f4572509891883a947411fd7239237dd2a39c3/mnist/main.py#L122
            .map(|tensor| ((tensor / 255) - 0.1307) / 0.3081)
            .collect();

        let targets = items
            .iter()
            .map(|item| {
                Tensor::<B, 1, Int>::from_data(
                    [(if item.label == 0 { 1 } else { 0 } as i64).elem::<B::IntElem>()],
                    device,
                )
            })
            .collect();

        let images = Tensor::cat(images, 0);
        let targets = Tensor::cat(targets, 0);

        MnistBatch { images, targets }
    }
}

#[derive(Module, Debug)]
struct ZeroModel<B: Backend> {
    linear: nn::Linear<B>,
}

impl<B: Backend> ZeroModel<B> {
    pub fn forward(&self, input: Tensor<B, DIM>) -> Tensor<B, 2> {
        // Flatten the input tensor from [Batch, Height, Width] to [Batch, Height * Width]
        let batch_size = input.shape().dims[0];
        let flattened_size = WIDTH * HEIGHT; // 28 * 28 = 784

        let input_flattened = input.reshape([batch_size, flattened_size]);

        // Pass the flattened tensor through the linear layer
        let x = self.linear.forward(input_flattened);

        // The output of the linear layer is already Rank 2 [Batch, NUM_CLASSES]
        x
    }

    pub fn forward_classification(
        &self,
        images: Tensor<B, 3>,
        targets: Tensor<B, 1, Int>,
    ) -> ClassificationOutput<B> {
        let output = self.forward(images);
        let loss = CrossEntropyLossConfig::new()
            .init(&output.device())
            .forward(output.clone(), targets.clone());

        ClassificationOutput::new(loss, output, targets)
    }
}

impl<B: AutodiffBackend> TrainStep<MnistBatch<B>, ClassificationOutput<B>> for ZeroModel<B> {
    fn step(&self, batch: MnistBatch<B>) -> TrainOutput<ClassificationOutput<B>> {
        let item = self.forward_classification(batch.images, batch.targets);

        TrainOutput::new(self, item.loss.backward(), item)
    }
}

impl<B: Backend> ValidStep<MnistBatch<B>, ClassificationOutput<B>> for ZeroModel<B> {
    fn step(&self, batch: MnistBatch<B>) -> ClassificationOutput<B> {
        self.forward_classification(batch.images, batch.targets)
    }
}

#[derive(Config, Debug)]
struct ModelConfig {
    i: usize,
    o: usize,
}

impl ModelConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> ZeroModel<B> {
        ZeroModel {
            linear: nn::LinearConfig::new(self.i, self.o).init(device),
        }
    }
}

#[derive(Config)]
pub struct TrainingConfig {
    pub optimizer: SgdConfig,
    #[config(default = 10)]
    pub num_epochs: usize,
    #[config(default = 128)]
    pub batch_size: usize,
    #[config(default = 1)]
    pub num_workers: usize,
    #[config(default = 42)]
    pub seed: u64,
    #[config(default = 0.02)]
    pub learning_rate: f64,
}

fn main() {
    let device = Default::default();
    let config = TrainingConfig::new(SgdConfig::new().with_momentum(Some(MomentumConfig {
        momentum: 0.9,
        dampening: 0.,
        nesterov: false,
    })));
    config
        .save(format!("{ARTIFACT_DIR}/config.json"))
        .expect("Config should be saved successfully");

    let model_config = ModelConfig::new(WIDTH * HEIGHT, NUM_CLASSES.into());
    let model: ZeroModel<MyBackend> = model_config.init(&device);

    let dataset = MnistDataset::train();
    let test_dataset = MnistDataset::test();

    let batcher = MnistBatcher::default();

    let dataloader_train = DataLoaderBuilder::new(batcher.clone())
        .batch_size(config.batch_size)
        .shuffle(config.seed)
        .num_workers(config.num_workers)
        .build(dataset);

    let dataloader_test = DataLoaderBuilder::new(batcher)
        .batch_size(config.batch_size)
        .shuffle(config.seed)
        .num_workers(config.num_workers)
        .build(test_dataset);

    let learner = LearnerBuilder::new(ARTIFACT_DIR)
        .metric_train_numeric(AccuracyMetric::new())
        .metric_valid_numeric(AccuracyMetric::new())
        .metric_train_numeric(LossMetric::new())
        .metric_valid_numeric(LossMetric::new())
        .with_file_checkpointer(CompactRecorder::new())
        .devices(vec![device.clone()])
        .num_epochs(config.num_epochs)
        .summary()
        .build(model, config.optimizer.init(), config.learning_rate);

    let now = Instant::now();
    let model_trained = learner.fit(dataloader_train, dataloader_test);
    let elapsed = now.elapsed().as_secs();
    println!("Training completed in {}m{}s", (elapsed / 60), elapsed % 60);

    model_trained
        .save_file(format!("{ARTIFACT_DIR}/model"), &CompactRecorder::new())
        .expect("Trained model should be saved successfully");
}
