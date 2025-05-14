use std::sync::OnceLock;
use ndarray::{Array2, arr2, Axis};
use rand::Rng;
use ndarray_stats::QuantileExt;
use std::sync::Mutex;

#[allow(non_snake_case)]
fn R() -> &'static Array2<i32> {
    static MR: OnceLock<Array2<i32>> = OnceLock::new();
    MR.get_or_init(|| { 
        let  local_r = arr2(&[ [0,0,0,0,1,0], 
                                [0,0,0,1,0,1],
                                [0,0,100,1,0,0],
                                [0,1,1,0,1,0],
                                [1,0,0,1,0,0],
                                [0,1,0,0,0,0]]);
        return local_r;
    })
}

#[allow(non_snake_case)]
fn Q() -> &'static Mutex<Array2<f32>> {
    static MQ: OnceLock<Mutex<Array2<f32>>> = OnceLock::new();
    MQ.get_or_init(|| { 
        let local_q = Mutex::new(Array2::zeros((6, 6)));
        return local_q;
    })
}

static GAMMA: f32 = 0.8;



fn possible_actions(state: usize) -> Vec<usize> {
    // This function returns the possible actions for a given state.
    // In this case, we assume that all actions are possible for all states.
    let current_state_row = R().index_axis(Axis(0), state);
    let possible_actions: Vec<usize> = current_state_row
        .iter()
        .enumerate()
        .filter_map(|(index, &value)| if value > 0 { Some(index) } else { None })
        .collect();
    return possible_actions;
}

fn action_choice(available_actions: Vec<usize>) -> usize {
    // This function chooses an action from the available actions.
    // In this case, we choose a random action.
    let mut rng = rand::rng();
    let random_index = rng.random_range(0..available_actions.len());
    return available_actions[random_index];
}

fn reward(state: usize, action: usize) {
    // This function returns the reward for a given state and action.
    // In this case, we assume that the reward is 0 for all actions except for the last action.
    let mut d = Q().lock().unwrap();
    let max_state = d.index_axis(Axis(0), action);
    let max_value = max_state.max().unwrap();
    let r = R()[[state, action]] as f32 + GAMMA * max_value;
    d[[state, action]] = r;

}

fn main() {
    let mut rng = rand::rng();
    for _ in 0..5000 {
        let state = rng.random_range(0..R().shape()[0]);
        let actions = possible_actions(state);
        let action = action_choice(actions);
        reward(state, action);
    }
    let d = Q().lock().unwrap();
    let max = d.max().unwrap();
    let normalized_q = d.mapv(|x| x / max * 100.0);
    println!("normalize reward:\n {}", normalized_q);
}