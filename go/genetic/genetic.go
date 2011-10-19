package main

/* encode
 * 0:         0000   0x
 * 1:         0001   1x
 * 2:         0010   2x
 * 3:         0011   3x
 * 4:         0100   4x
 * 5:         0101   5x
 * 6:         0110   6x
 * 7:         0111   7x
 * 8:         1000   8x
 * 9:         1001   9x
 * +:         1010   Ax
 * -:         1011   Bx
 * *:         1100   Cx
 * /:         1101   Dx
 * */

import (
	"fmt"
	"flag"
	"rand"
	"time"
	"bytes"
	"strconv"
)

type encMap map[int] uint64

type GeneticElement struct {
	fitness int
	chromosome uint64
	maptable encMap
	initialized bool
}


func (g* GeneticElement)RandInit(maptable encMap){
	values := make([]int, 16, 16)
	i:=0
	buffer := bytes.NewBufferString("")
	for key:= range maptable {
		values[i] = key
		i++
	}
	for i := 0; i < 9; i++ {
		fmt.Fprint(buffer, string(values[rand.Int()%len(maptable)]));
	}
	g.chromosome = encode(buffer.String(), maptable)
	g.maptable = maptable
	g.computeFitness()
	g.initialized = true
}

func (g* GeneticElement)computeFitness(){
	g.fitness = solve(decode(g.chromosome,g.maptable))
}

func (g* GeneticElement)evaluateFitness(ref int) int{
	ref64 := float64(ref)
	if g.initialized {
		tmp:= 1.5
		if (ref64-float64(g.fitness))!=0 {
			if (ref64-float64(g.fitness))<0 {
				tmp = 1/((ref64-float64(g.fitness))*(-1))
			} else {
				tmp = 1/((ref64-float64(g.fitness)))
			}
		}
		return int(20*tmp)
	}
	return 1
}

func (g* GeneticElement)Crossover(m* GeneticElement, f* GeneticElement ) {
	var mMask uint64
	var fMask uint64
	if m.initialized && f.initialized {
		guess:= rand.Int()%10
		if guess<6 { //euristic value for ricombination
			cxIndex := uint(rand.Int()%36)
			mMask = 0xFFFFFFFFFFFFFFFF << cxIndex
			fMask = 0xFFFFFFFFFFFFFFFF ^ mMask
			g.chromosome = (fMask & f.chromosome) | (mMask & m.chromosome)
		}else {
			g.chromosome = f.chromosome
		}
		guess = rand.Int()%100  // generate a mutation into genoma
		if guess<=1 {
			mutatedbit:= uint(rand.Int()%36)
			mMask = 0x1 << mutatedbit
			g.chromosome = g.chromosome ^ mMask
		}

		g.maptable = f.maptable
		g.computeFitness()
		g.initialized = true
	}
}

func view(code uint64) {
	fmt.Printf("questo e' il codice %0#36b\n", code)
}

func decode(code uint64, maptable encMap) []int {
	reverseMap := make(map[uint64] int)
	instr := make([]int, 9, 9)
	for key, value := range maptable {
		reverseMap[value] = key
	}
	for i:=0; i<9; i=i+1 {
		tmp := code & (uint64(15)<<(4*uint(i)))
		tmp = tmp >> (4*uint(i))
		instr[(8-i)] = reverseMap[tmp]
	}
	return instr
}

func is_number(elem int) bool {
	return (elem>='0' && elem<='9')
}

func is_operator(elem int) bool {
	return (elem == '+' || elem <= '-' || elem == '/' || elem == '*')
}

func compute(e1 int, op int, e2 int) int {
	res := 0
	if op == '+' { res = e1 + e2 }
	if op == '-' { res = e1 - e2 }
	if op == '*' { res = e1 * e2 }
	if op == '/' {
		if e2!=0{ res = e1 / e2
		}else{ res = e1 }
	}
	return res
}

func solve(instr []int) int {
	elem1, op, elem2 := -1, -1, -1
	for i:=0; i<9; i++ {
		if is_number(instr[i]) && elem1<0 && op<0 && elem2<0 {
			elem1 = instr[i]-48
		}
		if is_operator(instr[i]) && elem1>0 && op<0 && elem2<0 {
			op = instr[i]
		}
		if is_number(instr[i]) && elem1>0 && op>0 && elem2<0 {
			elem2 = instr[i]-48
			elem1 = compute(elem1,op,elem2)
			op = -1
			elem2 = -1
		}
		
	}
	return elem1
}

func encode(code string, maptable encMap) uint64 {
	/* schema num-op-num-op-num-op-num-op-num */
	res := uint64(0)
	if len(code) < 9 {
		fmt.Println("wrong chromosome not enought genes")
		return 0
	}
	if len(code) > 9 {
		fmt.Println("wrong chromosome not enought genes")
		return 0
	}
	for pos, char:= range code {
		element:= maptable[char]
		res = res | (element<<(4*uint(8-pos)))
	}
	return res
}

func buildGeneration(parents []*GeneticElement, childs []*GeneticElement, target int) (r* GeneticElement) {
	i:=0
	r=nil
	for i<100 {
		m := parents[rand.Int()%100]
		f := parents[rand.Int()%100]
		childs[i] = new(GeneticElement)
		childs[i].Crossover(m,f)
		roulette := childs[i].evaluateFitness(target)
		//viewGenotype(childs[i])
		if roulette==30 { // valore soglia
			r=childs[i]
		}
		for j:=1; j<=roulette && i+j<100; j++ {
			childs[i+j]= childs[i]
		}
		i=i+roulette+1
	}
	return
}

func viewGenotype(elem* GeneticElement){
	instr := decode(elem.chromosome, elem.maptable)
	fmt.Printf("la risposta e' [ %c, %c, %c, %c, %c, %c, %c, %c, %c] ",
		instr[0],
		instr[1],
		instr[2],
		instr[3],
		instr[4],
		instr[5],
		instr[6],
		instr[7],
		instr[8])              
	fmt.Println(solve(instr))
}

func main(){
	maptable := encMap {
		'0': 0x0,
		'1': 0x1,
		'2': 0x2,
		'3': 0x3,
		'4': 0x4,
		'5': 0x5,
		'6': 0x6,
		'7': 0x7,
		'8': 0x8,
		'9': 0x9,
		'+': 0xA,
		'-': 0xB,
		'*': 0xC,
		'/': 0xD,
		'_': 0xE,
		'$': 0xF,
	}
	populationA := make([]*GeneticElement, 100, 100)
	populationB := make([]*GeneticElement, 100, 100)
	rand.Seed(time.Nanoseconds() % 1e9)
	args := flag.Args()

	if len(args)==1{
		i:= 0
		target, _ := strconv.Atoi(args[0])
		for i<100 {
			populationA[i]= new(GeneticElement)
			populationA[i].RandInit(maptable)
			roulette := populationA[i].evaluateFitness(target)
			for j:=1; j<=roulette && i+j<100; j++ {
				populationA[i+j]= populationA[i]
			}
			i=i+roulette+1
		}
		
		var result *GeneticElement = nil
		tmp1 := populationA
		tmp2 := populationB
		for result==nil {
			fmt.Printf(".")
			result = buildGeneration(tmp1, tmp2, target)
			swp := tmp1
			tmp1 = tmp2
			tmp2 = swp
		}

		fmt.Printf("\n")
		for k:=0; k<len(tmp1); k++ {
			if tmp1[k]!= nil {
				instr := decode(tmp1[k].chromosome, maptable)
				fmt.Printf("%d\t[ %c, %c, %c, %c, %c, %c, %c, %c, %c] ", k,
			 		instr[0],
			 		instr[1],
			 		instr[2],
			 		instr[3],
			 		instr[4],
			 		instr[5],
			 		instr[6],
			 		instr[7],
			 		instr[8])              
				fmt.Println(target, solve(instr), tmp1[k].evaluateFitness(target))
			} else {
				fmt.Println("fanculo bastardo")
			}
		}
		
		instr := decode(result.chromosome, maptable)
		fmt.Printf("la risposta e' [ %c, %c, %c, %c, %c, %c, %c, %c, %c] ",
			instr[0],
			instr[1],
			instr[2],
			instr[3],
			instr[4],
			instr[5],
			instr[6],
			instr[7],
			instr[8])              
		fmt.Println(target, solve(instr), result.evaluateFitness(target))
		
	}else{
		fmt.Println("mmm do you miss to specify the value ?")
		return
	}
}