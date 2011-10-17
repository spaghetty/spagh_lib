package main

import (
	"fmt"
)

func filter(prime int, in, out chan int){ // just have a filter for each prime number
	fmt.Println("this value", prime, "is prime")
	for{
		value := <- in
		if value < 0 {   // number stream ended start closing routine
			if value == -1 {
				out <- value
				exit_turn := <- out
				in <- exit_turn -1 
				return
			}
		} else if value%prime!= 0 {
			out <- value
		} // do nothing for non prime number
	}	
}

func terminator(in chan int){ // the last node when something get here we have meet new prime number because no such filter intercept it
	incoming := in
	value := 0
	for {
		value = <- incoming
		if value < 0 {
			in <- (value - 1)
			return
		} else {
			tmp := incoming
			incoming = make(chan int)
			go filter(value, tmp, incoming)  // so build new filter
		}
	}
}

func generate(out, pipe chan int){
	next := make(chan int)
	go terminator(next)
	for {
		value := <- pipe
		if value < 0 {
			if value == -1 { // start quiting things
				next <- value
				exit_turn := <- next
				pipe <- exit_turn - 1
				return
			}
		} else if value == 1 {
			fmt.Println("this value", value, "is prime")
		} else {
			next <- value
		}
	}
}

func main() {
	//first of all create a channel for collect primes
	prime := make(chan int)
	pipe := make(chan int)
	//go prime_collector(prime)
	go generate(prime, pipe)
	for i:=1; i<10000; i++ {
	pipe <- i
	}

	pipe <- -1
	 <- pipe // wait for each goroutine close
}


