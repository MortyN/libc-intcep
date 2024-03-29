package main

import (
	"fmt"
	"os"
)

func main() {
	// Create or open the file
	file, err := os.Create("filenamejava.txt")
	if err != nil {
		fmt.Println("Error:", err)
		return
	}
	defer file.Close()

	// Write "hello" to the file
	_, err = file.WriteString("hello")
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	fmt.Println("Successfully wrote to filenamejava.txt")
}
