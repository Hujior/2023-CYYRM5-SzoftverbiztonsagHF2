parser:	./src/main.c
	gcc -c ./src/main.c
	gcc -L./lib/libwebp -o ./parser ./main.o -lwebp -lwebpdemux -lwebpmux -lm

jpeg: ./src/main.c
	gcc -o ./parser ./src/main.c ./lib/openjpeg/libopenjp2.a
