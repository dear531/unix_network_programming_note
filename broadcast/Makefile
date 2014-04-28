CC	:= cc
CFLAGS	:= -Wall -g -c
targer	:= send recv
default:$(targer)
send:send_broad_cast.o
	$(CC) $^ -o $@
recv:recv_broad_cast.o
	$(CC) $^ -o $@

clean:
	-rm -rf *.o *~
cleanall:clean
	-rm -rf $(targer)
send_broad_cast.o:send_broad_cast.c
	$(CC) $(CFLAGS) $< -o $@
recv_broad_cast.o:recv_broad_cast.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY:
	default clean cleanall
