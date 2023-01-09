mod deck;
mod card;
mod board;
mod game;
mod player;
use crate::game::*;

fn main() {
    println!("this would be a framework for deal with cards game!");
    println!("frist of all start with a card");
    let mut g = new_up_down_game();
    g.shuffle();
    let _ = g.new_player("test1");
    let _ = g.new_player("test2");
   
    for _ in 1..=40 {
        let d = g.deal_hand();
        if d.is_none() {
            break;
        }
        g.play_hand();
        println!("new hand");
    }
    let r = g.results();
    for i in r {
        println!("the result for {} is {}", i.0, i.1);
    }
    println!("================================= let's play again");

}
