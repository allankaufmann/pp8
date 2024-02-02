edgedetection: edgedetection.o img.o
	gcc -o edgedetection edgedetection.o img.o -lm

edgedetection.o: edgedetection.c
	gcc -Wall -c edgedetection.c

img.o: img.c img.h
	gcc -Wall -c img.c

clean:
	rm edgedetection *.o
