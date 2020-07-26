all: makebin stringsearch_bruteforce stringsearch_horspool stringsearch_boyermoore

stringsearch_bruteforce: makebin StringSearch_BruteForce.c
	gcc StringSearch_BruteForce.c -Wall -lm -o bin/stringsearch_bruteforce -ansi -std=c99 -g

stringsearch_horspool: makebin StringSearch_Horspool.c
	gcc StringSearch_Horspool.c -Wall -lm -o bin/stringsearch_horspool -ansi -std=c99 -g

stringsearch_boyermoore: makebin StringSearch_BoyerMoore.c
	gcc StringSearch_BoyerMoore.c -Wall -lm -o bin/stringsearch_boyermoore -ansi -std=c99 -g


del:
	rm bin/*
	
makebin:
	mkdir -p bin
