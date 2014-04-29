CC	:= cc
CFLAGS	:= -Wall -g -c
target	:= server client
default:$(target)
server:mcast_server.o
	$(CC) $^ -o $@
mcast_server.o:mcast_server.c
	$(CC) $(CFLAGS) $< -o $@
client:mcast_client.o
	$(CC) $^ -o $@
mcast_client.o:mcast_client.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	-rm -rf *.o a.out *~
cleanall:clean
	-rm -rf $(target)
.PHONY:
	default clean cleanall
