CC	:=cc
FLAGS	:= -Wall -g -c
EXEC	:= server client

default:$(EXEC)
server:domainserver.o
	$(CC) $^ -o $@
domainserver.o:domainserver.c
	$(CC) $(FLAGS) $< -o $@
client:domainclient.o
	$(CC) $^ -o $@
domainclient.o:domainclient.c
	$(CC) $(FLAGS) $< -o $@
clean:
	-rm -rf *.o
cleanall:clean
	-rm -rf $(EXEC)
cp:
	cp $(EXEC) /home/akaedu 
.PHONY:
	default clean cleanall copy
