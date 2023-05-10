parser:	./src/main.c
	gcc -L./lib/libwebp -o ./parser ./src/main.c -lwebp -lwebpdemux -lwebpmux -lm

