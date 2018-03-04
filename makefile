all: dataparse.c
	gcc -o dataparse.exe dataparse.c
	
clean:
	rm *.exe
