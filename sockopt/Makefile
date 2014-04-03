CC	:=cc
all	:=a.out exec

default:$(all)
a.out:set_fd_close_exe.o
	$(CC) $^ -o $@
exec:exe_fd.o
	$(CC) $^ -o $@

clean:
	-rm -rf 1.txt set_fd_close_exe.o exe_fd.o
cleanall:clean
	rm -rf $(all)
.PHONY:
	default clean cleanall
