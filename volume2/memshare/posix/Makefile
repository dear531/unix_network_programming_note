target	:=shmcreate shmunlink shmwrite shmread
LIBS	:=-lrt
# $(foreach <var>;,<list>;,<text>;) 
default:
	$(foreach i, $(target), cc $(i).c -o $(i) $(LIBS);)
clean:
	-rm -rf *.o a.out $(target)
.PHONY:
	clean
