all: tokenring

samples: tokenring.c
	gcc -Wall -o tokenring tokenring.c
clean:
	@rm -rf pipe* *.o tokenring valgrind-out.txt
valgrind:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./tokenring 2 0.99 1
