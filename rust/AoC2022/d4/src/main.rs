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
    let mut myschedule: VecDeque<((i32, i32),(i32, i32))> = VecDeque::new();
    let mut count: i32 = 0;
    let mut count2: i32 = 0;
    if let Ok(file) = open_file{
        let lines = io::BufReader::new(file).lines();
        for line in lines {
            let l = line.unwrap();
            let mut mypair: ((i32,i32),(i32,i32)) = ((0,0),(0,0));
            if let Some(tmp) = l.split_once(",") {
                let (p1, p2) = tmp;
                if let Some(tmp) = p1.split_once("-") {
                    let(lower, upper) = tmp;
                    mypair.0 = (lower.parse::<i32>().unwrap(), upper.parse::<i32>().unwrap());
                }
                if let Some(tmp) = p2.split_once("-") {
                    let (lower, upper) = tmp;
                    mypair.1 = (lower.parse::<i32>().unwrap(), upper.parse::<i32>().unwrap());
                }
                if (mypair.0.0 <= mypair.1.0) && (mypair.0.1 >= mypair.1.1) {
                    count+=1;
                } else {
                    if (mypair.0.0 >= mypair.1.0) && (mypair.0.1 <= mypair.1.1) {
                        count+=1;
                    }
                }
                let mymin = min(mypair.0.0, mypair.1.0);
                if mymin == 0 && mypair.1.0 <= mypair.0.1 {
                        count2 += 1;
                }
                if mymin == 1 && mypair.0.0 <= mypair.1.1 {
                    count2 +=1;
                }
                myschedule.push_back(mypair.clone());
            }
        }
        println!("number of contained schedules are {:?}", count);
        println!("numbero of partially contained schedule are {:?}", count2);
    }
}

fn min(a:i32, b:i32) -> usize {
    if a<=b {
        return 0;
    }
    return 1;
}