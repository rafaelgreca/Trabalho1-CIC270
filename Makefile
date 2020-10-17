CC = gcc

GLLIBS = -lglut -lGLEW -lGL

all: modelo.c 
	$(CC) modelo.c -o modelo $(GLLIBS)

clean:
	rm -f modelo
