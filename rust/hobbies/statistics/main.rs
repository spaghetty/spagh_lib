// the Monty Hall Paradox simulation
use rand::Rng;
use rand::prelude::*;
use rand_chacha::ChaCha20Rng;
use std::fmt;

const CARGLYPH: &str = "🚗";
const SHEEPGLYPH: &str = "🐏";
const FAILGLYPH: &str = "❌";
const SUCCESSGLYPH: &str = "✅";

#[derive(Debug, PartialEq, Eq)]
enum Prize {
    Sheep,
    Car,
}

#[derive(Debug)]
struct Element {
    content: Prize,
    selected: bool,
    opened: bool,
}

impl fmt::Display for Element {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let mut myglyph = SHEEPGLYPH;
        let mut mymod = "🚪";
        if self.content == Prize::Car {
           myglyph = CARGLYPH;
        }
        if self.opened {
            mymod = "✖️";
        }
        if self.selected {
            mymod = "🕺";
        }
        
        write!(f, "{}{}", mymod, myglyph)
    }
}

#[derive(Debug)]
struct GameBoard {
    doors: [Element; 3],
}

impl fmt::Display for GameBoard {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} {} {}", self.doors[0], self.doors[1], self.doors[2])
    }
}

fn setup_game() -> GameBoard {
    let mut rng = ChaCha20Rng::from_entropy();
    let pos = rng.gen_range(0..3);
    let e1 = if  pos == 0  {
        Element{content: Prize::Car, selected: false, opened: false}
    } else {
        Element{content: Prize::Sheep, selected: false, opened: false}
    };
    let e2 = if  pos == 1  {
        Element{content: Prize::Car, selected: false, opened: false}
    } else {
        Element{content: Prize::Sheep, selected: false, opened: false}
    };
    let e3 = if  pos == 2  {
        Element{content: Prize::Car, selected: false, opened: false}
    } else {
        Element{content: Prize::Sheep, selected: false, opened: false}
    };
    let g = GameBoard{doors: [e1, e2, e3]};
    return g;
}

fn player_selection() -> usize {
    let mut rng = ChaCha20Rng::from_entropy();
    return rng.gen_range(0..3);
}
fn show_sheep(g: &GameBoard) -> usize {
    for x in 0..3 {
        if g.doors[x].content == Prize::Sheep && !g.doors[x].selected {
            return x
        }
    };
    return 4
}
fn main () {
    let mut success = 0;
    let max_tests = 30;
    for _x in 0..max_tests {
        let mut g = setup_game();
        g.doors[player_selection()].selected = true;
        let s = show_sheep(&g);
        g.doors[s].opened = true;
        let mut mymod = FAILGLYPH;
        for x in 0..3 {
            if !g.doors[x].selected && !g.doors[x].opened && g.doors[x].content == Prize::Car{
                success= success + 1;
                mymod = SUCCESSGLYPH;
            } 
        }
        println!("{} {}", g, mymod);
    }
    println!("success are {} on {}, rate {}", success, max_tests, success as f32/max_tests as f32);
    //println!("setup is: {:?}", g);
}