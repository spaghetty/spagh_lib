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
        let mut forrest: VecDeque<VecDeque<u32>> = VecDeque::new();
        for line in lines {
            let l = line.unwrap();
            let mut tmp: VecDeque<u32> = VecDeque::new();
            for i in l.chars() {
                let d = i.to_digit(10).unwrap();
                tmp.push_back(d);
            }
            forrest.push_back(tmp);
        }
        let h = forrest.len();
        let l = forrest[0].len();
        let mut is_visible = 0;
        let mut max_scenic = 0;
        for i in 0..h*l {
            let row = i/l;
            let column = i%l;
            if row==0 || row == l-1 || column == h-1 || column ==0 {
                is_visible += 1;
                continue;
            }
            let tmp = forrest[row][column];
            if check_visible_in_row(&forrest, tmp, row, column) ||
                check_visible_in_column( &forrest, tmp, column, row) {
                    is_visible += 1;
            }
            let scenic_score = calc_scenic_score(&forrest, row, column);
            if scenic_score > max_scenic {
                max_scenic = scenic_score
            }
            println!("{}, {} my score: {}",row, column, scenic_score);
        }
        println!("total visible elements {:?} and max score is {}", is_visible, max_scenic);
    }
    println!("Hello, world!");
}

fn calc_scenic_score(f: &VecDeque<VecDeque<u32>>, r: usize, c: usize) -> u32 {
    let ( mut north_s, mut south_s, mut east_s, mut west_s): (u32, u32, u32, u32) = (0, 0, 0, 0);
    //let ( mut north_prev, mut south_prev, mut east_prev, mut west_prev): (u32, u32, u32, u32) = (f[r-1][c], f[r+1][c], f[r][c-1], f[r][c+1]);
    if r >= 1 {
        for i in (0..r).rev() {
            //println!("{},{} -> {}", i, c, north_prev);
            if f[i][c] < f[r][c] {
                north_s += 1;
                //north_prev = f[i][c];
            } else {
                north_s += 1;
                break;
            }
        }
    }
    if r < f.len() {
        for i in r+1..f.len() {
            if f[i][c] < f[r][c] {
                south_s += 1;
                //south_prev = f[i][c];
            } else {
                south_s +=1;
                break;
            }
        }
    }
    if c >= 1 {
        for i in (0..c).rev() {
            if f[r][i] < f[r][c] {
                east_s += 1;
                //east_prev = f[r][i];
            } else {
                east_s += 1;
                break;
            }
        }
    }
    if c < f[0].len() {
        for i in c+1..f[0].len() {
            if f[r][i] < f[r][c] {
                west_s += 1;
                //west_prev = f[r][i];
            } else {
                west_s += 1;
                break;
            }
        }
    }
    return north_s * south_s * east_s * west_s;
}

fn check_visible_in_row(f: &VecDeque<VecDeque<u32>>, t: u32, r: usize, c: usize) -> bool{
    let mut res_l = true;
    let mut res_r = true;
    for i in 0..c {
        if f[r][i] >= t {
            res_l = false;
            break;
        }
    }
    for i in c+1..f[0].len() {
        if f[r][i] >= t {
            res_r = false;
            break;
        }
    }
    //println!("is {} visible in row {:?}: {}", t, f[r], res_l || res_r);
    return res_l || res_r;
}

fn check_visible_in_column(f: &VecDeque<VecDeque<u32>>, t: u32, c: usize, r: usize) -> bool {
    let mut res_t = true;
    let mut res_b = true;
    for i in 0..r {
        if f[i][c] >= t {
            res_t = false;
            break;
        }
    }
    for i in r+1..f.len() {
        if f[i][c] >= t {
            res_b = false;
            break;
        }
    }
    //println!("is {} visible in column {:?}: {}", t, c, res_t || res_b);
    return res_t || res_b;
}