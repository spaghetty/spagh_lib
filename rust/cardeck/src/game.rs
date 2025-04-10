use crate::board::{Board};
#[allow(unused_imports)]
use crate::deck::{generate_francedeck, generate_naplesdeck, ShuffleType};
use crate::player::{Player, DealType};
use std::rc::Rc;
use std::collections::VecDeque;
use text_io::read;




pub trait Game {
    fn shuffle(&mut self);
    fn results(&mut self) -> VecDeque<(String, u32)>;
    fn deal_hand(&mut self) -> Option<()>;
    fn play_hand(&mut self) -> Option<()>;
    fn add_player(&mut self, p: Player) -> Result<Rc<Player>,&str>;
    fn new_player(&mut self, name: &str) -> Result<Rc<Player>, &str>;
}


pub struct UpDown {
    board: Board,
    hand_size: usize,
    n_player: usize,
    players: VecDeque<Rc<Player>>
}

pub fn new_up_down_game() -> UpDown {
    let d = generate_naplesdeck();
    return UpDown{board: Board::new(d), hand_size:1, n_player: 5, players: VecDeque::new()}
}

impl Game for UpDown {
    fn deal_hand(&mut self) -> Option<()> {
        if !self.board.has_card(self.hand_size){
            return None;
        } 
        for _ in 1 ..= self.hand_size {
            for j in self.players.iter_mut() {
                let c = self.board.get_card();
                let tmp = Rc::get_mut(j)?;
                tmp.receive_card(c, DealType::HIDDEN);
            }
        }
        Some(())
    }
    fn results(&mut self) -> VecDeque<(String, u32)> {
        let mut tmp = VecDeque::new();
        for p in self.players.iter_mut() {
            let name = p.name.to_owned();
            let score = p.score; 
            Rc::get_mut(p).unwrap().score = 0;
            tmp.push_back((name, score));
        }
        return tmp;

    }
    fn play_hand(&mut self) -> Option<()> {
        for p in self.players.iter_mut() {
            println!("player: {} decide if it's high or low card ->", p.name);
            let mut_p = Rc::get_mut(p)?;
            let c = mut_p.show_cards(&[]);
            'outer: loop {
                let s: String = read!();
                for key in s.chars().enumerate() {
                    match key {
                        (_, 'h')  => {
                            //play the game
                            println!("you choose upper char and you got {:}", c[0]);
                            if c[0].rank > 5u8 {
                                mut_p.score += 1;
                                println!("you win");
                            }
                            break 'outer;
                        },
                        (_,'l') => {
                            println!("you choose lower char and you got {:}", c[0]);
                            if c[0].rank < 6u8 {
                                mut_p.score += 1;
                                println!("you win");
                            }
                            break 'outer
                        }
                        (_, 'q')  => {
                            println!("Exiting game...");
                            return None; 
                        },
                        (_, _) => continue
                };
                };
            }
            mut_p.recollect_cards(&[]);
        }
        Some(())
    }
    fn shuffle(&mut self) {
        self.board.shuffle(ShuffleType::FULL);
    }

    fn add_player(&mut self, p: Player) -> Result<Rc<Player>,&str> {
        if self.n_player == self.players.len() {
            return Err("no more sits");
        }
        let pos = self.players.len();
        let x = Rc::new(p);
        self.players.insert(pos,x);
        let tmp = self.players.get_mut(pos).unwrap();
        return Ok(Rc::clone(tmp));
    }

    fn new_player(&mut self, name: &str) -> Result<Rc<Player>, &str> {
        let tmp = Player::new(name);
        let res = self.add_player(tmp);
        return res
    }
}