CC	:=cc
all	:=server client
default:$(all)
server:udpserver.o
	$(CC) $^ -o $@
client:udpclient.o
	$(CC) $^ -o $@
cleanall:clean
	-rm -rf $(all)
clean:
	-rm -rf udpserver.o udpclient.o
.PHONY:
	default clean cleanall
