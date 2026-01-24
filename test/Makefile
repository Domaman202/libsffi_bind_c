all:
	gcc -o main main.c -I../include -L../lib  -lsffi  
	gcc -shared -o testlib.so testlib.c
	./main

clean:
	rm main
	rm testlib.so
