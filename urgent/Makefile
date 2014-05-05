CC	:= cc
CFLAGS	:= -Wall -c -g

target	:= server client
default:$(target)
server:beat_server.o
	$(CC) $^ -o $@
beat_server.o:beat_server.c
	$(CC) $(CFLAGS) $< -o $@
client:beat_client.o
	$(CC) $^ -o $@
beat_client.o:beat_client.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	-rm -rf *.o a.out
cleanall:clean
	-rm -rf  $(target)
.PHONY:
	default clean cleanall
