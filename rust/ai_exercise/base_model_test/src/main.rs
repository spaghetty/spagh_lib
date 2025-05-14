// --- Imports ---
use burn::config::Config;
use burn::module::Module;
use burn::nn::{Linear, LinearConfig};
//use burn::optim::{Adam, AdamConfig};
use burn::tensor::{Tensor, backend::Backend};
//use burn::train::{ClassificationOutput, TrainOutput, TrainStep, ValidStep};

// --- Model Definition ---
#[derive(Module, Debug)]
pub struct MyModel<B: Backend> {
    linear: Linear<B>,
}

impl<B: Backend> MyModel<B> {
    pub fn forward(&self, input: Tensor<B, 2>) -> Tensor<B, 2> {
        self.linear.forward(input)
    }
}

// --- Model Configuration ---
#[derive(Config, Debug)]
pub struct MyModelConfig {
    input_dim: usize,
    output_dim: usize,
}

impl MyModelConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> MyModel<B> {
        MyModel {
            linear: LinearConfig::new(self.input_dim, self.output_dim).init(device),
        }
    }
}

// --- Example Usage Function ---
pub fn run_example<B: Backend>(device: &B::Device) {
    // 1. Define Input Data
    let input_data = [[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]];
    let input_tensor = Tensor::<B, 2>::from_data(input_data, device);

    println!("Input Tensor: {:?}", input_tensor.to_data());

    // 2. Initialize Model
    let model_config = MyModelConfig::new(3, 1);
    let model = model_config.init(device);

    println!("Model: {:?}", model);

    // 3. Perform a Forward Pass
    let output_tensor = model.forward(input_tensor.clone());

    println!("Output Tensor: {:?}", output_tensor.to_data());

    // --- Showing a hypothetical training step (for context) ---
    let targets = Tensor::<B, 2>::from_data([[7.0], [8.0]], device);
    let loss = (output_tensor - targets).powf_scalar(2.0).mean();

    println!("Loss: {:?}", loss.to_data());
}

// --- Main Function with Backend Selection ---
fn main() {
    use burn::backend::NdArray;
    let device = Default::default();
    println!("Using NdArrayBackend on device: {:?}", device);
    run_example::<NdArray>(&device);
}
