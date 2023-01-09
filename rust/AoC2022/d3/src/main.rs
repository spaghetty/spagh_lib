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
    if let Ok(file) = open_file{
        let lines = io::BufReader::new(file).lines();
        let mut total = 0;
        let mut badge: VecDeque<char> = VecDeque::new();
        let mut group_counter: usize = 0;
        let mut group_sum_result: i32 = 0;
        for line in lines {
            let l = line.unwrap();
            if l.len() == 0 {
                continue;
            }
            let mut acc:VecDeque<char> = VecDeque::new();
            
            let (big, small) = l.split_at(l.len()/2);
            for c in big.chars().into_iter() {
                let res = small.find(c);
                match res {
                    Some(_) => {
                        if !acc.contains(&c) {
                            acc.push_front(c);
                        }
                    },
                    None => (),
                }
            }
            for e in acc {
                println!("{:?}, {:?}", e, calculate_value(e));
                total = total + calculate_value(e);
            }
            if group_counter == 0 {
                if badge.len()>0 {
                    println!("the group badge is {:?}", badge);
                    group_sum_result += calculate_value(badge[0])
                }
                badge.clear();
                badge.extend(l.chars());
            } else {
                let mut tmp = l.clone();
                for i in tmp.clone().chars() {
                    if !badge.contains(&i) {
                        tmp = tmp.replace(i.to_owned(), "");
                    }
                }
                badge.clear();
                badge.extend(tmp.chars());
            }
            group_counter = (group_counter + 1) %3;
        }
        println!("the group badge is {:?}", badge);
        group_sum_result += calculate_value(badge[0]);
        println!("total is: {:?}, and {:?}", total, group_sum_result);

    }
}

fn calculate_value(elem: char) -> i32 {
    let lower_base: i32 = 'a' as i32 -1;
    let high_base: i32 = 'A' as i32 -1;
    let ord = elem as i32;
    let mut tmp = ord - lower_base;
    if !elem.is_ascii_lowercase() {
        tmp = ord - high_base + 26;
    }
    
    return tmp;
}