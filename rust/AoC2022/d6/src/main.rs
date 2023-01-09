use clap::{Arg, Command};
use std::collections::VecDeque;
use std::fs::File;
use std::io::{self, BufRead};

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
        let size: usize = 40;
        for line in lines {
            let l = line.unwrap();
            println!("package start for {1:0$} is {2}",size, clean_view(&l,size), find_packet_sequence(&l, 0).unwrap());
            println!("message start for {1:0$} is {2}",size,  clean_view(&l,size), find_msg_sequence(&l, 0).unwrap());
        }
    }
    println!("Hello, world!");
}
fn clean_view(s: &String, l: usize) -> String {
    if s.len() > l {
        let tollerable = (l/2)-2;
        let mut tmp = s.get(0..tollerable).unwrap().to_string();
        tmp = tmp + "...";
        tmp += s.get(s.len()-tollerable..).unwrap();
        return tmp;
    }
    return s.to_owned();
}

fn find_packet_sequence(data: &String, start: usize) -> Option<usize> {
    if data.len()< 4 {
        return None;
    }
    if data.chars().nth(0) == data.chars().nth(1) ||
        data.chars().nth(0) == data.chars().nth(2) ||
        data.chars().nth(0) == data.chars().nth(3) ||
        data.chars().nth(1) == data.chars().nth(2) ||
        data.chars().nth(1) == data.chars().nth(3) ||
        data.chars().nth(2) == data.chars().nth(3) {
            let res = find_packet_sequence(&data.get(1..).unwrap().to_string(),1);
            if let Some(x) = res {
                return Some(x + start);
            }
        } else {
            return Some(4+start);
        }
    return None;
}

fn find_msg_sequence(data: &String, start: usize) -> Option<usize> {
    if data.len() < 14 {
        return None;
    }
    let mut acc: VecDeque<char> = VecDeque::new();
    let mut valid = true;
    for i in 0..14 {
        let tmp = data.chars().nth(i).unwrap();
        if acc.contains(&tmp) {
            valid = false;
            break;
        }
        acc.push_front(tmp);
    }
    if valid {
        return Some(14+start);
    } else {
        let res = find_msg_sequence(&data.get(1..).unwrap().to_string(),1);
        if let Some(x) = res {
            return Some(x + start);
        }
    }
    return None;
}