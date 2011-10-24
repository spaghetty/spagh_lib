package main

import (
	"fmt"
	"flag"
	"rand"
	"time"
	"strconv"
)

type Element struct {
	value int
	weight int
}

type GeneticElement struct {
	chromosome uint64
	fitness int
	value int
	weight int
	tRef []Element
	initialized bool
	target int
}

func (p *GeneticElement)InitRand(e []Element, target int) {
	bitNum := rand.Int()%len(e)
	p.chromosome = uint64(0)
	for i:=0; i<=bitNum; i++ {
		tmp := uint64(1)<<uint(rand.Int()%len(e))
		p.chromosome = p.chromosome | tmp 
	}
	p.tRef = e[0:]
	p.target = target
	p.value, p.weight = evalutate(p.chromosome, e[0:])
	p.fitness = generate_fitness(p.weight, target)
	p.initialized = true
}

func generate_fitness(weight int, target int) int {
	r := target - weight
	if r < 0 {
		r = r * (-1)
	}
	return r
}

func evalutate(chromosome uint64, e []Element) (res,weight int) {
	res = 0
	weight = 0
	for i:=0; i<len(e); i++ {
		val := chromosome & (uint64(1)<<uint(i))
		if val > 0 {
			res = res + e[i].value
			weight = weight + e[i].weight
		}
	}
	return 
}

func (g* GeneticElement)Crossover(m* GeneticElement, f* GeneticElement ) {
        var mMask uint64
        var fMask uint64
        if m.initialized && f.initialized {
                guess:= rand.Int()%10
                if guess<6 { //euristic value for ricombination
                        cxIndex := uint(rand.Int()%len(f.tRef))
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

                g.tRef = f.tRef
		g.target = f.target
                g.value, g.weight = evalutate(g.chromosome, g.tRef)
		g.fitness = generate_fitness(g.weight, g.target)
        }
}

func print_genotype(g *GeneticElement) {
	fmt.Printf("%0#32b, %d, %d, %d\n",g.chromosome, g.value, g.weight, g.fitness)
}

func print_genotype_r(g *GeneticElement) {
	fmt.Printf("result: %0#32b, %d, %d, %d, %d\n",g.chromosome, g.value, g.weight, g.fitness, g.target)
}

func build_generation(parents []*GeneticElement, childs []*GeneticElement) {
	i:=0
        for i<101 {
                m := parents[rand.Int()%100]
                f := parents[rand.Int()%100]
                childs[i] = new(GeneticElement)
                childs[i].Crossover(m,f)
		roulette := 0
		if childs[i].fitness == 0 {
			roulette = 30
		} else {
			roulette = int(25/childs[i].fitness)
		}
                if roulette==30 { // valore soglia
                        print_genotype_r(childs[i])
                }
                for j:=1; j<=roulette && i+j<100; j++ {
                        childs[i+j]=childs[i]
                }
                i=i+roulette+1
        }
        return

}

func genetic_choose(e []Element, target int) []Element {
	var generations [2][100]*GeneticElement
	for j:=0; j<len(generations[0]); j++ {
		generations[0][j] = new(GeneticElement)
		generations[0][j].InitRand(e, target)
	}
	
	a,b:=0,1
	for i:=0; i<1; i++ {
		build_generation(generations[a][0:], generations[b][0:])
		tmp:=a
		a = b
		b = tmp
	}
	//k := new(GeneticElement)
	//k.Crossover(g, f)
	//for _, i := range generations[1] {
	//	print_genotype(i)
	//}
	//fmt.Printf("%0#32b ", f.chromosome)
	//fmt.Println(f.fitness, f.weight)
	return nil
}

func main() {
	deposit := []Element{
                { 10, 5 },
                { 9, 7 },
                { 8, 3 },
                { 7, 1 },
                { 6, 5 },
		{ 5, 2 } }

        args := flag.Args()
	rand.Seed(time.Nanoseconds() % 1e9)
        if len(args)!= 1 {
                fmt.Println("Do you miss to specify value for the space in the sack?")
                return
        }

        for _, x := range deposit {
                fmt.Println( x )
        }

        fmt.Println("============================")
        target, _ := strconv.Atoi(args[0])
        tmp := genetic_choose( deposit[0:], target )

        fmt.Println(len(tmp))
        for _, x := range tmp {
                fmt.Println( x )
        }
}