use rand::distributions::{Distribution, Uniform};
use rand::prelude::*;
use rand_chacha::ChaCha20Rng;
use std::collections::VecDeque;
use std::rc::Rc;


use crate::card::{Card, Suit};

#[allow(dead_code)]
pub enum ShuffleType {
    FULL,
    PARTIAL,
}

#[derive(Debug)]
pub struct Deck {
    //suits: [Suit;4],
    cards: VecDeque<Rc<Card>>,
    out_cards: VecDeque<Rc<Card>>
}

impl Deck {
    fn new(suit: [Suit;4], card_number: u8) -> Deck {
        let mut deck = VecDeque::new();
        for i in suit{
            for j in 1..=card_number {
                deck.push_back(Card::new(j, i));
            }
        }
        return Deck{cards: deck, out_cards: VecDeque::new()};
    }

    pub fn shuffle(&mut self, t: Option<ShuffleType>) {
        match t {
            Some(x) => {
                match x {
                    ShuffleType::FULL => {
                        self.reset();
                    },
                     ShuffleType::PARTIAL => (),
                }
            }
            None => self.reset(),
        }
        let mut rng = ChaCha20Rng::from_entropy();
        let c_num = self.cards.len();
        let selector = Uniform::<usize>::from(0..c_num);
        for _ in 1..=c_num {
            let pos1 = selector.sample(&mut rng);
            let pos2 = selector.sample(&mut rng);
            self.cards.swap(pos1, pos2)
        }
    }

    pub fn get(&mut self) -> Option<Rc<Card>> {
        self.out_cards.push_back(self.cards.pop_front()?);
        let tmp = self.out_cards.back()?;
        return Some(Rc::clone(tmp));
        
    }

    pub fn reset(&mut self) {
        self.cards.append(&mut self.out_cards);
    }

    pub fn remaining(&self) -> usize {
        return self.cards.len();
    }
}

#[allow(dead_code)]
pub fn generate_francedeck() -> Deck {
    Deck::new([Suit::Cuori, Suit::Quadri, Suit::Fiori, Suit::Picche], 13u8)
}

pub fn generate_naplesdeck() -> Deck {
    Deck::new([Suit::Bastoni, Suit::Denari, Suit::Coppe, Suit::Spade], 10u8)
}


#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_decks_generation() {
        let tmp = generate_francedeck();
        assert_eq!(tmp.remaining(),52);
        let tmp = generate_naplesdeck();
        assert_eq!(tmp.remaining(),40);
    }
}
