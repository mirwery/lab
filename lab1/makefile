all: cat.a grep.a ls.a

cat.a: cat.c 
	gcc cat.c -o cat.a

grep.a: grep.c
	gcc grep.c -o grep.a

ls.a: ls.c
	gcc ls.c -o ls.a

clean:
	rm -r *.a
