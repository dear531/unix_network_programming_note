target	:=mmap_share_memory not_shared_fork_memory \
memory_semaphore_share_memory mmap_not_inherit
LIBS	:=-lpthread
# $(foreach <var>;,<list>;,<text>;) 
default:
	$(foreach i, $(target), cc $(i).c -o $(i) $(LIBS);)
clean:
	-rm -rf *.o a.out $(target)
.PHONY:
	clean
