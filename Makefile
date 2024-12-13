

gcc_args = -I/usr/include/ -L/usr/lib -lvlccore -lvlc `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0`

main: main.o
	gcc $(gcc_args) main.o -o Player

main.o: main.c
	gcc -c $(gcc_args) main.c