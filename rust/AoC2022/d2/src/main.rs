use clap::{Arg, Command};
use std::fs::File;
use std::io::{self, BufRead};
use std::collections::VecDeque;


fn main() {
    let matches = Command::new("my d1 code")
        .version("0.1.0")
        .author("spaghetty")
        .arg(Arg::new("file")
             .short('f')
             .long("file")
             .default_value("test_input.txt")
             .help("the input file"))
        .get_matches();
    let input = matches.get_one::<String>("file").unwrap();
    let open_file = File::open(input);
    let mut plays: VecDeque<(i32, i32)> = VecDeque::new();
    //let results: VecDeque<i32> = VecDeque::new(); 
    if let Ok(file) = open_file{
        let lines = io::BufReader::new(file).lines();
        for line in lines {
            let value = line.unwrap().parse::<String>();
            match value {
             Ok(x) => { 
                let mut tmp = x.clone();
                tmp.retain(|c| !c.is_whitespace());
                let p1: i32 = tmp.chars().nth(0).unwrap() as i32 - 'A' as i32 +1;
                let p2: i32 = tmp.chars().nth(1).unwrap() as i32 - 'X' as i32 +1;
                plays.push_front((p1, p2));
             },
            Err(e) => println!("{:?}", e)
            }
        }

        let mut result = 0;
        println!("number of plays {:?}", plays.len());
        for play in &plays {
            let res = play.1 - play.0;
            if res == 1 {
                result = result + 6;
            }
            if res == 0 {
                result = result + 3;
            }
            if res == -2 {
                result = result + 6;
            }
            result = result + play.1;
        }
        println!("{:?}", result);
        result = 0;
        // A Rock, B Paper, C Scissor
        // X lose, Y draw, Z win
        for play in &plays {
            if play.1 == 1 {
                let mut tmp = play.0 - 1;
                if tmp <= 0 {
                    tmp = 3;
                }
                result = result + tmp;
            }
            if play.1 == 2 {
                result = result + 3 + play.0;
            }
            if play.1 == 3 {
                let mut tmp = play.0 + 1;
                if tmp > 3 {
                    tmp = 1;
                }
                result = result + 6 + tmp;
            }
        }
        println!("second -> {:?}", result);
    }
    println!("Hello, world!");
}
