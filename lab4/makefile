all: shm.a get_shm.a

shm.a: shm.c
	gcc shm.c -o shm.a

get_shm.a: get_shm.c
	gcc get_shm.c -o get_shm.a

clean:
	rm -r *.a