all:
	$(CC) -o server common.c server.c
	$(CC) -o client common.c client.c

clean:
	rm server client
