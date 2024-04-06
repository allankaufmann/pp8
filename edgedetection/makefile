edgedetection: edgedetection.o img.o
	gcc -g -o edgedetection edgedetection.o img.o -lm -fopenmp

edgedetection.o: edgedetection.c
	gcc -g -Wall -c edgedetection.c -fopenmp

img.o: img.c img.h
	gcc -g -Wall -c img.c

clean:
	rm edgedetection *.o
