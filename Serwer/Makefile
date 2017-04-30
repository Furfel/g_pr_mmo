CFLAGS=
LIB=-lpthread -lm
BIN_SUFFIX=

dontsendmap : CFLAGS=-std=gnu99 -D_DEBUG_ -D_DONTSENDMAP_ -g
dontsendmap : BIN_SUFFIX=-debug
dontsendmap : gameserver

debug : CFLAGS=-std=gnu99 -D_DEBUG_ -g
debug : BIN_SUFFIX=-debug
debug : gameserver

harddebug : CFLAGS=-D_DEBUG_ -D_HARDDEBUG_ -g
harddebug : BIN_SUFFIX=-debug
harddebug : gameserver

release : gameserver

gameserver : players.o main.o utils.o network.o watchdog.o game.o
	gcc -o build/gameserver$(BIN_SUFFIX) main.o utils.o players.o network.o watchdog.o game.o $(LIB)
	make clean
	
main.o : main.c utils.h network.h players.h game.h
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
