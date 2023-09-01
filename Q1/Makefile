all: samples

samples: samples.c
	gcc -Wall -o samples samples.c
clean:
	@rm -rf *.o samples valgrind-out.txt
valgrind:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./samples quote.txt 15 99
