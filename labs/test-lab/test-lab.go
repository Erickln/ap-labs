package main

import (
	"fmt"
	"os"
)

func main() {
	var name = ""
	if len(os.Args) < 2 {
		fmt.Println("Not enoguh data")
		return
	} else if len(os.Args) == 2 {
		name = os.Args[1]
	} else if len(os.Args) > 2 {
		name = ""
		for i := 1; i < len(os.Args); i++ {
			name += os.Args[i] + " "
		}
	}
	fmt.Printf("Welcome %sto the jungle\n", name)

}
