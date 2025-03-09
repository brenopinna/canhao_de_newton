CC = gcc
CFLAGS =  -Ofast -march=native -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Iinclude
LDLIBS = -lm -lallegro -lallegro_font -lallegro_ttf -lallegro_image -lallegro_primitives -lallegro_acodec -lallegro_audio

# Descomente no Windows!
# ALLEGRO = allegro32

ifdef ALLEGRO
CFLAGS += -I${ALLEGRO}/include
LDFLAGS = -L${ALLEGRO}/lib
endif

main: main.o lib.o
	$(CC) $(CFLAGS) $(LDFLAGS) main.o lib.o -o main $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

lib.o: lib.c
	$(CC) $(CFLAGS) -c lib.c -o lib.o  

clean:
ifdef ALLEGRO
		del /q main *.o
else
		rm main *.o
endif
