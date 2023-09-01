all: txt2epub

txt2epub: txt2epub.c
	gcc -Wall -o txt2epub txt2epub.c
clean:
	@rm -rf *.o txt2epub valgrind-out.txt *.epub epub epub.zip
valgrind:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
		 --trace-children=yes \
         --log-file=valgrind-out.txt \
         ./txt2epub MobyX4.txt f1.txt f2.txt f3.txt