CC	:= cc
CFLAGS	:= -Wall -g -c
target	:= getfd a.out


default:$(target)
getfd:getfd.o
	$(CC) $^ -o $@
getfd.o:getfd.c
	$(CC) $(CFLAGS) $< -o $@
a.out:deliverfd.o
	$(CC) $^ -o $@
deliverfd.o:deliverfd.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	-rm -rf *.o 1.txt
cleanall:clean
	-rm -rf $(target)
.PHONY:
	default clean clean
