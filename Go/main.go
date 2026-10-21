package main

import (
	"flag"
	"fmt"
	"os"

	"dbms/commands"
)

func main() {
	// Разбор параметров --file и --query
	fileName := flag.String("file", "", "файл данных")
	query := flag.String("query", "", "запрос")
	flag.Parse()

	if *fileName == "" || *query == "" {
		fmt.Println("usage: ./dbms --file <file> --query '<COMMAND name [args]>'")
		os.Exit(1)
	}

	reply, err := commands.ExecuteQuery(*fileName, *query)
	if err != nil {
		fmt.Println("-> ERROR:", err)
		os.Exit(2)
	}
	fmt.Println("-> " + reply)
}
