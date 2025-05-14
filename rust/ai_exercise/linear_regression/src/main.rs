use ndarray::{Array2, Axis, s};
use ndarray_linalg::solve::Inverse;
use rand::prelude::*;
use serde::Deserialize;
use std::path::Path;

#[derive(Debug, Deserialize)]
struct Position {
    #[serde(rename = "latitude")]
    lat: f64,
    #[serde(rename = "longitude")]
    lon: f64,
}

#[derive(Debug, Deserialize)]
struct Housing {
    #[serde(flatten)]
    pos: Position,
    #[serde(rename = "housing_median_age")]
    avg_age: f64,
    #[serde(rename = "total_rooms")]
    tot_rooms: f64,
    #[serde(rename = "total_bedrooms")]
    tot_bedrooms: f64,
    #[serde(rename = "population")]
    population: f64,
    #[serde(rename = "households")]
    house_holds: f64,
    #[serde(rename = "median_income")]
    avg_income: f64,
    #[serde(rename = "median_house_value")]
    avg_house_value: f64,
}

trait DataSet {
    fn as_dataset(self) -> Array2<f64>;
}
impl DataSet for Vec<Housing> {
    fn as_dataset(self) -> Array2<f64> {
        let mut res = Array2::<f64>::zeros((self.len(), 4));
        let mut i = 0;
        for k in self.into_iter() {
            res[[i, 0]] = k.avg_age / 10.0;
            res[[i, 1]] = k.house_holds / k.population;
            //res[[i, 2]] = k.house_holds;
            res[[i, 2]] = k.avg_income / 100.0;
            res[[i, 3]] = k.avg_house_value;
            i += 1;
        }
        return res;
    }
}

fn linear_regression(X: &Array2<f64>, y: &Array2<f64>) -> Array2<f64> {
    let xt = X.t(); // Transpose of X
    let xtx = xt.dot(X); // X^T * X
    let xtx_inv = xtx.inv().unwrap(); // (X^T * X)^-1
    let xty = xt.dot(y); // X^T * y
    xtx_inv.dot(&xty) // (X^T * X)^-1 * X^T * y
}

fn main() {
    let mut rng = rand::rng();
    let mut training: Vec<Housing> = Vec::new();
    let mut test: Vec<Housing> = Vec::new();
    let mut predict: Vec<Housing> = Vec::new();
    let mut reader =
        csv::Reader::from_path(Path::new("data/california_housing_train.csv")).unwrap();
    let data = reader.deserialize::<Housing>();
    for i in data {
        let bucket = rng.random_range(0..10);
        if let Err(x) = i {
            println!("{}", x);
            break;
        }
        let x = i.unwrap();
        match bucket {
            0..=4 => training.push(x),
            5 | 6 | 7 => test.push(x),
            _ => predict.push(x),
        }
    }
    let training_dataset = training.as_dataset();
    let test_dataset = test.as_dataset();
    let predict_dataset = predict.as_dataset();
    let training_features = training_dataset.clone().slice(s![.., ..3]).to_owned();
    let training_labels = training_dataset.clone().slice(s![.., 3..]).to_owned();
    println!(
        "Hello, world! {:?}{:?}, {:?}, {:?}",
        training_features.shape(),
        training_labels.shape(),
        test_dataset.shape(),
        predict_dataset.shape()
    );
    println!(
        "{} -> {}:{}",
        training_dataset.index_axis(Axis(0), 0),
        training_features.index_axis(Axis(0), 0),
        training_labels.index_axis(Axis(0), 0)
    );

    let coefficients = linear_regression(&training_features, &training_labels);

    println!("{}<--", coefficients);
    let test_features = test_dataset.clone().slice(s![.., ..3]).to_owned();
    let test_labels = test_dataset.clone().slice(s![.., 3..]).to_owned();
    let predictions = test_features.dot(&coefficients);
    for i in 0..10 {
        println!(
            "{} -> {}:{}",
            test_features.index_axis(Axis(0), i),
            predictions.index_axis(Axis(0), i),
            test_labels.index_axis(Axis(0), i)
        );
    }
}
