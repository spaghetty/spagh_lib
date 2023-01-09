use clap::{Arg, Command};
use std::fs::File;
use std::io::{self, BufRead};
use std::collections::VecDeque;

#[derive(Debug, Copy, Clone)]
struct Elf {
    order: i32,
    calory: i32,
}

fn main() {
    let matches = Command::new("my d1 code")
        .version("0.1.0")
        .author("spaghetty")
        .arg(Arg::new("file")
             .short('f')
             .long("file")
             .default_value("input.txt")
             .help("the input file"))
        .get_matches();
    let mut curr_elf = Elf{order:1, calory:0};
    let mut max_elf: VecDeque<Elf> = VecDeque::new();
    let input = matches.get_one::<String>("file").unwrap();
    let open_file = File::open(input);
    if let Ok(file) = open_file{
        let lines = io::BufReader::new(file).lines();
        for line in lines {
            let value = line.unwrap().parse::<i32>();
            match value {
                Ok(x) => curr_elf.calory = curr_elf.calory+x,
                Err(_) => {
                    if max_elf.is_empty() {
                        max_elf.insert(0, Elf{order:curr_elf.order, calory: curr_elf.calory});
                    } else {
                        let pos = try_insert(curr_elf, &max_elf);
                        if pos != usize::MAX {
                            max_elf.insert(pos, Elf{order:curr_elf.order, calory: curr_elf.calory});
                        } else {
                            max_elf.push_back(Elf{order: curr_elf.order, calory: curr_elf.calory});
                        }
                    }
                    curr_elf.calory = 0;
                    curr_elf.order = curr_elf.order + 1;
                }
            }
        }
        if max_elf.is_empty() {
            max_elf.insert(0, Elf{order:curr_elf.order, calory: curr_elf.calory});
        } else {
            let pos = try_insert(curr_elf, &max_elf);
            if pos != usize::MAX {
                max_elf.insert(pos, Elf{order:curr_elf.order, calory: curr_elf.calory});
            } else {
                max_elf.push_back(Elf{order: curr_elf.order, calory: curr_elf.calory});
            }
        }
        println!("here the result the efl {:?}", max_elf)
    }
    println!("Hello, world! {:?}", sum_tree(&max_elf));
}

fn sum_tree(l: &VecDeque<Elf>) -> i32 {
    let mut total = 0;
    for i in 0..3 {
        let tmp = l.get(i);
        if let Some(value) = tmp {
            total = total + value.calory;
        }
    }
    return total;
}
fn try_insert(e: Elf, max_elf: &VecDeque<Elf>) -> usize {
    for elem in max_elf.iter().enumerate() {
        let (i, elf): (usize, &Elf) = elem;
        if e.calory > elf.calory {
            return i;
        }
    }
    return usize::MAX;
}