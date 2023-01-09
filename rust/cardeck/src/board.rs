use crate::card::*;
use crate::deck::*;
use std::rc::Rc;
pub struct Board{
    deck: Deck,
}

impl Board {
    pub fn new(d: Deck) -> Board {
        return Board{deck:d}
    }

    pub fn has_card(&self, n: usize) -> bool {
        return n < self.deck.remaining();
    }

    pub fn shuffle(&mut self, t: ShuffleType) {
        self.deck.shuffle(Some(t));
    }
    pub fn get_card(&mut self) -> Rc<Card> { 
        Rc::clone(&self.deck.get().unwrap())
    }
    
}