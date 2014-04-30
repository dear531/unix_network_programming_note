CC	:= cc
CFLAGS	:= -Wall -g -c
target	:= server client
vpath	%.h .
default:$(target)
server:	mcast_server.o
	$(CC) $^ -o $@
mcast_server.o:mcast_server.c share.h
	$(CC) $(CFLAGS) $< -o $@
client:	mcast_client.o
	$(CC) $^ -o $@
mcast_client.o:mcast_client.c share.h
	$(CC) $(CFLAGS) $< -o $@
fromnet:serverl clientl
serverl:server.o
	$(CC) $^ -o $@
server.o:server.c share.h
	$(CC) $(CFLAGS) $< -o $@
clientl:client.o
	$(CC) $^ -o $@
client.o:client.c share.h
	$(CC) $(CFLAGS) $< -o $@
clean:
	-rm -rf *.o a.out *~
cleanall:clean
	-rm -rf $(target) serverl clientl
.PHONY:
	default clean cleanall
