all: reader.a writer.a sem.a 

reader.a: reader.c
	gcc reader.c -o reader.a

writer.a: writer.c
	gcc writer.c -o writer.all

sem.a: sem.c
	gcc sem.c -o sem.a

clean:
	rm -r *a