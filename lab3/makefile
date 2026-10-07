all: pipe.a fifo_in.a fifo_out.a

pipe.a: pipe.c
	gcc pipe.c -o pipe.a

fifo_in.a: fifo_in.c
	gcc fifo_in.c -o fifo_in.a

fifo_out.a: fifo_out.c
	gcc fifo_out.c -o fifo_out.a
	
clean:
	rm -r *.a