use tokenizers::tokenizer::{Result, Tokenizer};
use std::fs;

fn main() -> Result<()> {
    let tokenizer = Tokenizer::from_pretrained("bert-base-cased", None).unwrap();
    let file_path = "the-verdict.txt"; // Specify the path to your input file
    let input = fs::read_to_string(file_path)
    .expect("Should have been able to read the file");
    
    let encoding = tokenizer.encode(input, true).unwrap();
    let _tokens = encoding.get_tokens();
    let ecode = encoding.get_ids();
    println!("size: {:?}", ecode.len());
    println!("Tokens: {:?}", ecode); 
    println!("Hello, world!");
    return Ok(()); // Return Ok to indicate success
}
