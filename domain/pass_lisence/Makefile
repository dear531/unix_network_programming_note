CC	:= cc
CFLAGS	:= -Wall -g -c
target	:= server client


default:$(target)
client:client.o
	$(CC) $^ -o $@
client.o:client.c
	$(CC) $(CFLAGS) $< -o $@
a.out:server.o
	$(CC) $^ -o $@
server.o:server.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	-rm -rf *.o
cleanall:clean
	-rm -rf $(target)
.PHONY:
	default clean clean
