CC = g++
CFLAGS = -c -Wall
LFLAGS = -static

game_logic: game_logic.o snake.o serialport.o
	$(CC) game_logic.o snake.o serialport.o -o game_logic

game_logic.o: game_logic.cpp
	$(CC) game_logic.cpp -o game_logic.o $(CFLAGS)

snake.o: snake.cpp snake.h
	$(CC) snake.cpp -o snake.o $(CFLAGS)

serialport.o: serialport.cpp serialport.h
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)

clean:
	@rm -f game_logic game_logic.o snake.o serialport.o