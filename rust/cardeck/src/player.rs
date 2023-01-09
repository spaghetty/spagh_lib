use crate::card::{Card};
use std::rc::Rc;
use std::collections::VecDeque;

#[allow(dead_code)]
pub enum DealType {
    HIDDEN,
    PRIVATE,
    PUBLIC
}

#[derive(Debug)]
pub struct Player {
    pub name: String,
    pub score: u32,
    cards: VecDeque<Rc<Card>>,
    hidden_cards: VecDeque<Rc<Card>>
}


impl Player {
    pub fn new(name: &str) -> Player {
        Player{cards: VecDeque::new(), hidden_cards: VecDeque::new(), name: name.to_owned(), score: 0u32}
    }

    pub fn receive_card(&mut self, c: Rc<Card>, t: DealType) {
        match t {
         DealType::HIDDEN => {
                self.hidden_cards.push_back(c);
            }
         DealType::PRIVATE => {
                self.cards.push_back(c);
            }
         DealType::PUBLIC => {
                self.cards.push_back(c);
            }
        }
    }
    
    pub fn show_cards(&mut self, index: &[usize]) -> &mut[Rc<Card>] {
        self.hidden_cards.make_contiguous();
        let tmp = self.hidden_cards.as_mut_slices();
        return tmp.0;
    }

    pub fn recollect_cards(&mut self, index: &[usize]) {
        self.hidden_cards.clear()
    }
}