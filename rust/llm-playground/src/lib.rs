use tokenizers::tokenizer::{Tokenizer, Encoding};
use std::fs;

pub fn encode(source: String, is_file: bool) -> Encoding {
    let tokenizer = Tokenizer::from_pretrained("bert-base-cased", None).unwrap();
    let input = if is_file {
        fs::read_to_string(source).expect("Should have been able to read the file")
    } else {
        source
    };

    tokenizer.encode(input, true).unwrap()
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_encode_with_string_input() {
        let input = String::from("Hello, world!");
        let encoding = encode(input, false);
        assert!(encoding.get_ids().len() > 0, "Encoding should not be empty");
    }

    #[test]
    fn test_encode_with_file_input() {
        let file_path = "/tmp/test_input.txt";
        let content = "This is a test file.";
        std::fs::write(file_path, content).expect("Unable to write test file");

        let encoding = encode(file_path.to_string(), true);
        assert!(encoding.get_ids().len() > 0, "Encoding should not be empty");

        std::fs::remove_file(file_path).expect("Unable to delete test file");
    }

    #[test]
    #[should_panic(expected = "Should have been able to read the file")]
    fn test_encode_with_nonexistent_file() {
        let file_path = "/tmp/nonexistent_file.txt";
        encode(file_path.to_string(), true);
    }

    #[test]
    fn test_encode_with_empty_string() {
        let input = String::from("");
        let encoding = encode(input, false);
        assert_eq!(encoding.get_ids().len(), 0, "Encoding should be empty for empty input");
    }
}
