all: client server

client:
	gcc client.c -o client

server:
	gcc main.c server.c dispatcher.c -o server

clean:
	rm server client
