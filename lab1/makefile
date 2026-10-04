all: cat.a grep.a

cat.a: cat.c 
	gcc cat.c -o cat.a

grep.a: grep.c
	gcc grep.c -o grep.a

clean:
	rm -r *.a
