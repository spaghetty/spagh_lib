mod core;

use crate::core::{DisplayPartial, TodoList};
use clap::{Arg, Command};
use std::fs;

fn main() {
    let matches = Command::new("To-Do CLI")
        .version("1.0")
        .author("Your Name <spaghetty>")
        .about("Manage your to-do list")
        .arg(
            Arg::new("file")
                .short('f')
                .long("file")
                .default_value("todo_list.json") // Default value for the file
                .help("Specify a custom file for the to-do list (default: todo_list.json)"),
        )
        .subcommand(
            Command::new("add")
                .about("Add a new to-do item")
                .arg(Arg::new("title").required(true).help("Title of the to-do item"))
                .arg(Arg::new("description").help("Description of the to-do item")),
        )
        .subcommand(
            Command::new("list")
                .about("List all to-do items")
                .arg(
                    Arg::new("completed")
                        .long("completed")
                        .short('c') 
                        .help("List only completed tasks")
                        .action(clap::ArgAction::SetTrue)
                        .conflicts_with("pending"), // Prevent using both flags together
                )
                .arg(
                    Arg::new("pending")
                        .long("pending")
                        .short('p') 
                        .action(clap::ArgAction::SetTrue)
                        .help("List only pending tasks"),
                ),
        )
        .subcommand(
            Command::new("complete")
                .about("Mark a to-do item as completed")
                .arg(Arg::new("id").required(true).help("ID of the to-do item to mark as completed")),
        )
        .subcommand(
            Command::new("remove")
                .about("Remove a to-do item")
                .arg(Arg::new("id").required(true).help("ID of the to-do item to remove")),
        )
        .get_matches();

    // Determine the file to use
    let file_path = matches.get_one::<String>("file").unwrap();

    // Load or initialize the to-do list
    let mut todo_list = load_todo_list(file_path);

    // Handle subcommands
    match matches.subcommand() {
        Some(("add", sub_matches)) => {
            let title = sub_matches.get_one::<String>("title").unwrap().to_string();
            let description = sub_matches.get_one::<String>("description").map(|s| s.to_string());
            todo_list.add_item(title, description);
            println!("Item added successfully!");
        }
        Some(("list", sub_matches)) => {
            if sub_matches.get_flag("completed") {
                println!("{}", todo_list.display_completed());
            } else if sub_matches.get_flag("pending") {
                println!("{}", todo_list.display_incomplete());
            } else {
                println!("{}", todo_list);
            }
        }
        Some(("complete", sub_matches)) => {
            let id: u32 = sub_matches.get_one::<String>("id").unwrap().parse().unwrap();
            match todo_list.mark_completed(id) {
                Ok(_) => println!("Item marked as completed!"),
                Err(err) => println!("Error: {}", err),
            }
        }
        Some(("remove", sub_matches)) => {
            let id: u32 = sub_matches.get_one::<String>("id").unwrap().parse().unwrap();
            match todo_list.remove_item(id) {
                Ok(_) => println!("Item removed successfully!"),
                Err(err) => println!("Error: {}", err),
            }
        }
        _ => {
            println!("Use --help to see available commands.");
        }
    }

    // Save the updated to-do list
    save_todo_list(&todo_list, file_path);
}

/// Load the to-do list from a file or create a new one if the file doesn't exist.
fn load_todo_list(file_path: &str) -> TodoList {
    if let Ok(data) = fs::read_to_string(file_path) {
        if let Ok(todo_list) = serde_json::from_str(&data) {
            return todo_list;
        }
    }
    TodoList::new()
}

/// Save the to-do list to a file.
fn save_todo_list(todo_list: &TodoList, file_path: &str) {
    let data = serde_json::to_string_pretty(todo_list).unwrap();
    fs::write(file_path, data).unwrap();
}
