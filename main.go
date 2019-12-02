package main

import "scholars/routers"



func main() {
	router := routers.InitRouter()

	router.Run(":8000")
}
