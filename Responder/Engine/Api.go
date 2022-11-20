package Engine

import (
	"io/ioutil"
	"log"
	"net/http"
)

func CreateConnections(ID string) {

	_, err := http.Get("https://ping.eren.run/Engine?ID=" + ID + "&Create")

	if err != nil {
		log.Fatal(err)
	}
}

func ConnectionsDelete(ID string) {

	_, err := http.Get("https://ping.eren.run/Engine?ID=" + ID + "&Delete")

	if err != nil {
		log.Fatal(err)
	}
}

func GetConnections(ID string) string {

	resp, err := http.Get("https://ping.eren.run/Engine?ID=" + ID)

	if err != nil {
		log.Fatal(err)
	}

	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)

	if err != nil {
		log.Fatal(err)
	}

	return string(body)
}
