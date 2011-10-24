package main

import (
	"fmt"
	"flag"
	"strconv"
)

type Element struct {
	value int
	weight int
	name string
}
	
func greedy_choose(e []Element, residual int) (res []Element, tmp bool) {
	if residual == 0 {
		return res ,true
	} else {
		final := false
		var my_element Element
		i := 0
		for !final && len(e) > i {
			my_element = e[i]
			my_weight := e[i].weight
			if residual-my_weight >= 0 {
				res, final = greedy_choose( e[i+1:], residual-my_weight)
			}
			i++
		}
		if final {
			res = append(res, my_element)
			return res, true
		} 
	}
	return res, false
}


func main(){
	deposit := []Element{
		{ 10, 5, "elemento1" },
		{ 9, 7, "elemento2" },
		{ 8, 3, "elemento3" },
		{ 7, 1, "elemento4" },
		{ 6, 5, "elemento5" },
		{ 5, 2, "elemento6" } }

	args := flag.Args()
	if len(args)!= 1 {
		fmt.Println("Do you miss to specify value for the space in the sack?")
		return 
	}

	for _, x := range deposit {
		fmt.Println( x )
	}
	
	fmt.Println("============================")
	residual, _ := strconv.Atoi(args[0])
	tmp, _ := greedy_choose( deposit[0:], residual)
	
	fmt.Println(len(tmp))
	for _, x := range tmp {
		fmt.Println( x )
	}
}