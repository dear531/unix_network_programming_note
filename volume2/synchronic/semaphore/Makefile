target	:=semcreate semunlink semgetvalue semwait sempost \
producer_consumer filelock unnamefork producer_consumer_unname
LIBS	:=-lpthread
# $(foreach <var>;,<list>;,<text>;) 
default:
	$(foreach i, $(target), cc $(i).c -o $(i) $(LIBS);)
clean:
	-rm -rf *.o a.out $(target)
.PHONY:
	clean
