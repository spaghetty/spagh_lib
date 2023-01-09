use clap::{Arg, Command};
use std::fs::File;
use std::io::{self, BufRead};
use std::collections::VecDeque;
use regex::Regex;

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
    if let Ok(file) = open_file{
        let re = Regex::new(r"move (\d+) from (\d+) to (\d+)").unwrap();
        let mut schema: VecDeque<String> = VecDeque::new();
        let mut board: VecDeque<VecDeque<char>> = VecDeque::new();
        let mut column_count = 0;
        let lines = io::BufReader::new(file).lines();
        let mut schema_read = false;
        for line in lines {
            let l = line.unwrap();
            if !schema_read && !l.is_empty(){
                schema.push_front(l.clone());
            }
            if l.is_empty() {
                schema_read = true;
                let tmp = schema.pop_front().unwrap();
                for c in tmp.chars() {
                    if !c.is_whitespace() {
                        column_count+= 1;
                    }
                }
                for _ in 0..column_count {
                    board.push_front(VecDeque::new());
                }
                for e in schema.iter() {
                    let mut tmp = e.as_str();
                    for i in 0..column_count{
                        let segment: &str;
                        if tmp.len() >= 4 {
                            (segment, tmp)= tmp.split_at(4);
                        } else {
                            segment = tmp;
                            tmp = "";
                        }
                        if let Some(tmp) = segment.find("[") {
                            board[i].push_front(segment.chars().nth(tmp+1).unwrap());
                        }
                    }
                    
                }
            }
            if !l.is_empty() && schema_read==true {
                let res = re.captures(l.as_str()).unwrap();
                if res.len() == 4 {
                    let mut spool: VecDeque<char> = VecDeque::new();
                    for _ in 0..res[1].parse::<usize>().unwrap() {
                        let tmp = board[res[2].parse::<usize>().unwrap()-1].pop_front().unwrap();
                        spool.push_front(tmp);
                    }
                    for i in spool.iter() {
                        board[res[3].parse::<usize>().unwrap()-1].push_front(i.clone());
                    }
                    spool.clear();
                }
                println!("{:?}", l);
            }
        }
        println!("{:?} - {:?}", board, column_count);
                
        for i in 0..column_count {
            println!("{:?}", board[i].pop_front().unwrap());
        }
    }
    println!("Hello, world!");

}
