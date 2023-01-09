use std::fmt;

use strum_macros::EnumIter; // 0.17.1

use std::rc::Rc;


const CLUBSUIT: &str  = "♣️";
const SPADESSUIT: &str = "♠️";
const DIAMONDSUIT: &str = "♦️";
const HEARTSUIT: &str = "♥️";
const SWORDSUIT: &str = "🗡️";
const CUPSUIT: &str = "🏆";
const COINSSUIT: &str = "🪙";
const STICKSUIT: &str = "🏑";

pub type Rank = u8;

#[derive(Debug, Copy, Clone, EnumIter)]
pub enum Suit {
    Spade,
    Denari,
    Coppe,
    Bastoni,
    Quadri,
    Picche,
    Fiori,
    Cuori
}

#[derive(Debug)]
pub struct Card {
    pub rank: Rank,
    suit: Suit,
}
impl fmt::Display for Card {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} of {}", self.rank_as_string(), self.gliph())
    }
}

impl Card {
     pub fn new(r: Rank, s: Suit) -> Rc<Card> {
        Rc::new(Card{rank: r,suit: s})
    }

    pub fn gliph(&self) -> &'static str {
        match self.suit {
            Suit::Cuori => HEARTSUIT,
            Suit::Fiori => CLUBSUIT,
            Suit::Quadri => DIAMONDSUIT,
            Suit::Picche => SPADESSUIT,
            Suit::Bastoni => STICKSUIT,
            Suit::Coppe => CUPSUIT,
            Suit::Denari => COINSSUIT,
            Suit::Spade => SWORDSUIT,
        }
    }

    pub fn rank_as_string(&self) -> &'static str {
        match self.rank {
            0 => "JOLLY",
            1 => "ACE",
            2 => "TWO",
            3 => "TREE",
            4 => "FOUR",
            5 => "FIVE",
            6 => "SIX",
            7 => "SEVEN",
            8 => {
                match self.suit {
                    Suit::Denari | Suit::Bastoni | Suit::Coppe | Suit::Spade => "DAME",
                    _ => "EIGHT",
                }

            },
            9 => {
                match self.suit {
                    Suit::Denari | Suit::Bastoni | Suit::Coppe | Suit::Spade => "KNIGHT",
                    _ => "NINE",
                }

            },
            10 => {
                match self.suit {
                    Suit::Denari | Suit::Bastoni | Suit::Coppe | Suit::Spade => "KING",
                    _ => "TEN",
                }

            },
            11 => {
                match self.suit {
                    Suit::Quadri | Suit::Picche | Suit::Fiori | Suit::Cuori => "JACK",
                    _ => "not valid ",
                }

            },
            12 => {
                match self.suit {
                    Suit::Quadri | Suit::Picche | Suit::Fiori | Suit::Cuori => "QUEEN",
                    _ => "not valid ",
                }
            },
            13 => {
                    match self.suit {
                        Suit::Quadri | Suit::Picche | Suit::Fiori | Suit::Cuori => "KING",
                        _ => "not valid ",
                    }
            },
            _ => "not a value",
        }
    }
}


#[cfg(test)]
mod tests {
    use super::*;
    use strum::IntoEnumIterator; // 0.17.1


    #[test]
    fn test_card_creation() {
        for suits in Suit::iter() {
            let tmp = Card::new(1, suits);
            assert_eq!(tmp.rank_as_string(),"ACE");
            match suits {
                Suit::Bastoni => assert_eq!(tmp.gliph(),STICKSUIT),
                Suit::Coppe => assert_eq!(tmp.gliph(),CUPSUIT),
                Suit::Cuori => assert_eq!(tmp.gliph(),HEARTSUIT),
                Suit::Denari => assert_eq!(tmp.gliph(),COINSSUIT),
                Suit::Fiori => assert_eq!(tmp.gliph(), CLUBSUIT),
                Suit::Picche => assert_eq!(tmp.gliph(),SPADESSUIT),
                Suit::Quadri => assert_eq!(tmp.gliph(), DIAMONDSUIT),
                Suit::Spade => assert_eq!(tmp.gliph(),SWORDSUIT),
            }
            
        }
    }
}