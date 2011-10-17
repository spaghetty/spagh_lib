package main

import (
	"fmt"
	"strings"
)

func WordCount(s string) map[string]int {
	word_list := strings.Fields(s)
	words_map := make(map[string]int)
	for i:=0; i < len(word_list); i++ {
     		val, ok := words_map[word_list[i]]
	 	if ok {
	      		words_map[word_list[i]] = val + 1
		}else{
			words_map[word_list[i]] = 1
		}
	}
	return words_map
}

func main() {
	fmt.Println(WordCount("tu sei una bestia. e tu puzzi"))
}