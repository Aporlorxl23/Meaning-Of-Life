package main

import (
	"encoding/json"
	"fmt"
	"os"
	"os/signal"
	"time"

	. "../Engine"
)

func main() {
	var ID string = CreateID()
	Data := []string{}
	CreateConnections(ID)
	fmt.Println("[+] Your Server: https://ping.eren.run/" + ID)
	fmt.Println("[*] Waiting Connections...")
	c := make(chan os.Signal, 1)
	signal.Notify(c, os.Interrupt)
	go func() {
		for sig := range c {
			fmt.Println("\n[-] Exiting...", sig)
			ConnectionsDelete(ID)
			os.Exit(1)
		}
	}()
	for i := 0; i <= 300; i++ {
		var Connection string = GetConnections(ID)
		err := json.Unmarshal([]byte(Connection), &Data)
		if err == nil {
			keys := make(map[string]bool)
			list := []string{}
			for _, entry := range Data {
				if _, value := keys[entry]; !value {
					keys[entry] = true
					list = append(list, entry)
				}
			}
			fmt.Printf("%v\r", list)
		}

		time.Sleep(2 * time.Second)
	}
	ConnectionsDelete(ID)
}
