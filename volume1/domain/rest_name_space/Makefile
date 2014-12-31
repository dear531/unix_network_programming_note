default:server client

server:unix_domain_server_restnamespace.c
	cc -g $^ -o  $@
client:unix_domain_client_restnamespace.c
	cc -g $^ -o  $@
clean:
	rm -rf *.o server client
.PHONY:
	default clean
