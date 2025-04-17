use serde::{Deserialize, Serialize};
use std::fmt;
use colored::*; // Add this for colored output

/// Represents a single to-do item.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct TodoItem {
    pub id: u32,
    pub title: String,
    pub description: Option<String>,
    pub completed: bool,
}

impl fmt::Display for TodoItem {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let status = if self.completed {
            "✔".green() // Green checkmark for completed tasks
        } else {
            "✘".red() // Red cross for incomplete tasks
        };

        write!(
            f,
            "{} | {} | {} | {}",
            format!("ID: {}", self.id).cyan(),
            format!("Title: {}", self.title).bold(),
            format!(
                "Description: {}",
                self.description.as_deref().unwrap_or("None")
            )
            .italic(),
            format!("Completed: {}", status)
        )
    }
}

/// Represents a list of to-do items.
#[derive(Debug, Serialize, Deserialize)]
pub struct TodoList {
    pub items: Vec<TodoItem>,
}

impl fmt::Display for TodoList {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(f, "{}", "Todo List:".bold().underline())?;
        for item in &self.items {
            writeln!(f, "{}", item)?;
        }
        Ok(())
    }
}

pub trait DisplayPartial {
    fn display_completed(&self) -> String;
    fn display_incomplete(&self) -> String;   
}

impl DisplayPartial for TodoList {
    fn display_completed(&self) -> String {
        let mut output = String::new();
        output.push_str(&format!("{}", "Completed Tasks:".bold().underline()));
        output.push('\n');
        let tmp_item = self.items
            .iter()
            .filter(|item| item.completed)
            .map(|item| format!("{}", item))
            .collect::<Vec<_>>()
            .join("\n");
        output.push_str(&tmp_item);
        output.push('\n'); 
        output
    }

    fn display_incomplete(&self) -> String {
        let mut output = String::new();
        output.push_str(&format!("{}", "Pending Tasks:".bold().underline()));
        output.push('\n');
        let tmp_item = self.items
            .iter()
            .filter(|item| !item.completed)
            .map(|item| format!("{}", item))
            .collect::<Vec<_>>()
            .join("\n");
        output.push_str(&tmp_item);
        output.push('\n');
        output
    }
}

impl TodoList {
    /// Creates a new, empty to-do list.
    pub fn new() -> Self {
        TodoList { items: Vec::new() }
    }

    /// Adds a new to-do item to the list.
    pub fn add_item(&mut self, title: String, description: Option<String>) {
        let id = self.items.len() as u32 + 1;
        let item = TodoItem {
            id,
            title,
            description,
            completed: false,
        };
        self.items.push(item);
    }

    /// Marks a to-do item as completed by its ID.
    pub fn mark_completed(&mut self, id: u32) -> Result<(), String> {
        if let Some(item) = self.items.iter_mut().find(|item| item.id == id) {
            item.completed = true;
            Ok(())
        } else {
            Err(format!("Item with ID {} not found", id))
        }
    }

    /// Removes a to-do item by its ID.
    pub fn remove_item(&mut self, id: u32) -> Result<(), String> {
        if let Some(pos) = self.items.iter().position(|item| item.id == id) {
            self.items.remove(pos);
            Ok(())
        } else {
            Err(format!("Item with ID {} not found", id))
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_add_item() {
        let mut todo_list = TodoList::new();
        todo_list.add_item("Test Item".to_string(), Some("This is a test".to_string()));

        assert_eq!(todo_list.items.len(), 1);
        assert_eq!(todo_list.items[0].title, "Test Item");
        assert_eq!(todo_list.items[0].description, Some("This is a test".to_string()));
        assert!(!todo_list.items[0].completed);
    }

    #[test]
    fn test_mark_completed() {
        let mut todo_list = TodoList::new();
        todo_list.add_item("Test Item".to_string(), None);

        let result = todo_list.mark_completed(1);
        assert!(result.is_ok());
        assert!(todo_list.items[0].completed);

        let invalid_result = todo_list.mark_completed(2);
        assert!(invalid_result.is_err());
    }

    #[test]
    fn test_remove_item() {
        let mut todo_list = TodoList::new();
        todo_list.add_item("Test Item 1".to_string(), None);
        todo_list.add_item("Test Item 2".to_string(), None);

        let result = todo_list.remove_item(1);
        assert!(result.is_ok());
        assert_eq!(todo_list.items.len(), 1);
        assert_eq!(todo_list.items[0].title, "Test Item 2");

        let invalid_result = todo_list.remove_item(3);
        assert!(invalid_result.is_err());
    }

    #[test]
    fn test_todo_item_display() {
        let todo_item = TodoItem {
            id: 1,
            title: "Learn Rust".to_string(),
            description: Some("Complete the Rust book".to_string()),
            completed: false,
        };

        // Expected output without colors
        let expected_output = "ID: 1 | Title: Learn Rust | Description: Complete the Rust book | Completed: ✘";
        let actual_output = format!("{}", todo_item);

        // Strip ANSI escape codes for comparison
        let stripped_output = strip_ansi_codes(&actual_output);
        assert_eq!(stripped_output, expected_output);
    }

    #[test]
    fn test_todo_list_display() {
        let mut todo_list = TodoList::new();
        todo_list.add_item("Learn Rust".to_string(), Some("Complete the Rust book".to_string()));
        todo_list.add_item("Write code".to_string(), None);

        // Expected output without colors
        let expected_output = "\
Todo List:
ID: 1 | Title: Learn Rust | Description: Complete the Rust book | Completed: ✘
ID: 2 | Title: Write code | Description: None | Completed: ✘
";
        let actual_output = format!("{}", todo_list);

        // Strip ANSI escape codes for comparison
        let stripped_output = strip_ansi_codes(&actual_output);
        assert_eq!(stripped_output, expected_output);
    }

    /// Helper function to strip ANSI escape codes from a string
    fn strip_ansi_codes(input: &str) -> String {
        let re = regex::Regex::new(r"\x1b\[[0-9;]*m").unwrap();
        re.replace_all(input, "").to_string()
    }
}
