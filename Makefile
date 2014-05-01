all: client server

client:
	gcc client.c -o client

server:
	gcc server.c dispatch.c -o server

clean:
	rm server client
