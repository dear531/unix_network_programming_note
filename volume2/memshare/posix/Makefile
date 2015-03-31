target	:=shmcreate shmunlink shmwrite shmread \
chid_parent_memory_position
LIBS	:=-lrt
# $(foreach <var>;,<list>;,<text>;) 
default:
	$(foreach i, $(target), cc $(i).c -o $(i) $(LIBS);)
clean:
	-rm -rf *.o a.out $(target)
.PHONY:
	clean
