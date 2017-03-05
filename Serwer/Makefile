CFLAGS=
LIB=-lpthread
BIN_SUFFIX=

debug : CFLAGS=-D_DEBUG_ -g
debug : BIN_SUFFIX=-debug
debug : gameserver

release : gameserver

gameserver : players.o main.o utils.o network.o watchdog.o game.o
	gcc -o build/gameserver$(BIN_SUFFIX) main.o utils.o players.o network.o watchdog.o game.o $(LIB)
	make clean
	
main.o : main.c utils.h network.h players.h
	gcc $(CFLAGS) -c main.c

utils.o : utils.c utils.h
	gcc $(CFLAGS) -c utils.c

players.o : players.c utils.h players.h
	gcc $(CFLAGS) -c players.c
	
network.o : network.c network.h players.h
	gcc $(CFLAGS) -c network.c
	
watchdog.o : watchdog.c watchdog.h utils.h
	gcc $(CFLAGS) -c watchdog.c

game.o : game.c game.h utils.h
	gcc $(CFLAGS) -c game.c

clean:
	rm *.o
