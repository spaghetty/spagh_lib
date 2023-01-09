use clap::{Arg, Command};
use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::{PathBuf};
use id_tree::*;



#[derive(Debug)]
enum LineType {
    Command,
    Result,
}
#[derive(Debug)]
struct CmdLine {
    ltype: LineType,
    l1: String,
    l2: String,
}

#[derive(Debug)]
struct FSNode {
    name: String,
    size: usize,
    is_dir: bool,
}

fn main() {
    let total_size = 70000000;
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
    let mut curr_dir = PathBuf::new();
    let mut fs: Tree<FSNode> = TreeBuilder::new().build();
    let _root_id: NodeId = fs.insert(Node::new(FSNode{name:"/".to_string(), size: 0, is_dir: true}), InsertBehavior::AsRoot).unwrap();
    if let Ok(file) = open_file{
        let lines = io::BufReader::new(file).lines();
        for line in lines {
            let l = line.unwrap();
            let tmp = line_parser(l);
            match tmp.ltype {
                LineType::Command => {
                    if tmp.l1 == "cd" {
                        if tmp.l2 != ".." {
                            add_node_dir(&mut fs, tmp.l2.clone(), curr_dir.clone());
                            curr_dir.push(tmp.l2.clone());
                        }
                        if tmp.l2 == ".." {
                            curr_dir.pop();
                        }
                    }
                }
                LineType::Result => (
                    if tmp.l1 != "dir" {
                        add_file(&mut fs, FSNode{name: tmp.l2.clone(), size: tmp.l1.parse::<usize>().unwrap(), is_dir: false}, curr_dir.clone());
                    }
                )
            }
        }
        let mymap = calculate_dir_size(&mut fs);
        let mut s = String::new();
        fs.write_formatted(&mut s).unwrap();
        println!("{}", s);
        let mut total: usize = 0;
        for (dir, size) in &mymap {
            if *size <= 100000 {
                println!("valid folder is {} with a size of {}", dir, size);
                total += size;
            }
        }
        println!("the total size is {}", total);
        let used_space = mymap.get(&node_name("/".to_string(), fs.root_node_id().unwrap().clone())).unwrap();
        let available_space = total_size- used_space;
        println!("available space is {}", total_size- used_space);
        let mut smalles_dir_size: usize = usize::MAX;
        for (_, size) in &mymap {
            if available_space + size >= 30000000 {
                if *size < smalles_dir_size {
                    smalles_dir_size = *size;
                }
            }
        }
        println!("the minimal size is {}", smalles_dir_size);
    }
   
}

fn node_name(a: String, b: NodeId,) -> String {
    return format!("{}-{:?}",a, b);
}

fn calculate_dir_size(f: &mut Tree<FSNode>) -> HashMap<String, usize>{
    let mut res_map: HashMap<String, usize> = HashMap::new();
    //need to support same name folder ad different levels.
    for nodeid  in f.traverse_post_order_ids(f.root_node_id().unwrap()).unwrap() {
        let node = f.get(&nodeid).unwrap();
        if !node.data().is_dir {
            let p = node.parent().unwrap();
            let parent = f.get(p).unwrap().data().name.clone();
            let unique = format!("{}-{:?}",parent, *p);
            if res_map.contains_key(&unique) {
                let t = res_map.get_mut(&unique).unwrap();
                *t = *t + node.data().size;
            } else {
                res_map.insert(unique, node.data().size);
            }
        } else {
            if node.data().name != "/" {
                let p = node.parent().unwrap();
                let parent = f.get(p).unwrap().data().name.clone();
                let unique = format!("{}-{:?}",parent,*p);
                let unique_local = format!("{}-{:?}", node.data().name, nodeid);
                let local_size = res_map.get(&unique_local).unwrap().clone();
                if res_map.contains_key(&unique) {
                    let t = res_map.get_mut(&unique).unwrap();
                    *t = *t + local_size;
                } else {
                    res_map.insert(unique, local_size);
                }
            }
        }
    }
    return res_map;
}

fn find_rightful_parent(t: &Tree<FSNode>, path: PathBuf) -> Option<NodeId> {
    let root_node = &((t).root_node_id().unwrap().clone());
    let mut tmp_node = root_node;
    {
        for i in path.iter() {
            if i == "/" {
                continue;
            }
            let tmp_cursor = t.get(tmp_node).unwrap();
            let mut found = false;
            for c in tmp_cursor.children().into_iter() {
                let tmp = t.get(c).unwrap().data();
                if tmp.name == i.to_string_lossy() {
                    tmp_node = c;
                    found= true;
                }
            }
            if !found {
                return None;
            } 
        }
        return Some(tmp_node.to_owned());
    }
}
fn add_file(t: &mut Tree<FSNode>, d: FSNode, path: PathBuf) -> Option<NodeId> {
    let tmp_node = find_rightful_parent(t, path);
    if let Some(x) = tmp_node {
        let res = t.insert(Node::new(d), InsertBehavior::UnderNode(&x)).unwrap();
        return Some(res);
    }
    return tmp_node;
}

fn add_node_dir(t: &mut Tree<FSNode>, d: String, path: PathBuf) -> Option<NodeId> {
    if d == "/" {
        return Some(t.root_node_id().unwrap().to_owned())
    }
    let tmp_node = find_rightful_parent(t, path);
    if let Some(x) = tmp_node {
        let res = t.insert(Node::new(FSNode{name: d, size: 0, is_dir: true}), InsertBehavior::UnderNode(&x));
        return Some(res.unwrap());
    }
    return tmp_node;
}

fn line_parser(l: String) -> CmdLine {
    let tmp = l.trim();
    if tmp.starts_with("$") {
        return l_cmd_parser(tmp);
    } 
    return l_response_parser(tmp);
}

fn l_cmd_parser(l: &str) -> CmdLine {
    let mut tmp = l.strip_prefix("$").unwrap();
    tmp = tmp.trim();
    let offset = tmp.find(" ");
    if let Some(x) = offset {
        let cmd = tmp.get(0..x).unwrap();
        let arg = tmp.get(x+1..).unwrap();
        return CmdLine { ltype: LineType::Command, l1: cmd.to_string(), l2: arg.to_string() }
    }
    return CmdLine{ ltype: LineType::Command, l1: tmp.to_string(), l2: "".to_string()};
    }

fn l_response_parser(l: &str) -> CmdLine {
    let offset = l.find(" ").unwrap();
    let par1 = l.get(0..offset).unwrap();
    let par2 = l.get(offset+1..).unwrap();
    return CmdLine { ltype: LineType::Result, l1: par1.to_string(), l2: par2.to_string() }
}