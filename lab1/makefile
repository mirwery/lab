all: cat.a grep.a ls.a chmod.a

cat.a: cat.c 
	gcc cat.c -o cat.a

grep.a: grep.c
	gcc grep.c -o grep.a

ls.a: ls.c
	gcc ls.c -o ls.a

chmod.a: chmod.c
	gcc chmod.c -o chmod.a

clean:
	rm -r *.a
