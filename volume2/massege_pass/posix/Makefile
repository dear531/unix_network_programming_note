LDFLAGS ?=-lrt
defaulte:send recv
send:mq_send.o
	cc $^ $(LDFLAGS) -o $@
mq_send.o:mq_send.c
	cc -c -g $< -o $@
recv:mq_receive.o
	cc $^ $(LDFLAGS) -o $@
mq_receive.o:mq_receive.c
	cc -c -g $< -o $@
clean:
	rm -rf *.o send recv
.PHONY:
	clean
