all: strassen.c 
	gcc -o strassen strassen.c

clean: 
	$(RM) strassen