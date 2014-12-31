default:server client

server:mkfifo_server.o
	cc $^ -o $@
mkfifo_server.o:mkfifo_server.c
	cc -g -c $< -o $@
client:openfifo_client.o
	cc  $^ -o $@
openfifo_client.o:openfifo_client.c
	cc -g -c $< -o $@
clean:
	rm -rf server client *.o
.PHONY:
	clean
