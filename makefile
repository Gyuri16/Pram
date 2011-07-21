FLAGS = -O2 -g -Wall 
OBJS = main.o
TARGET = pram.exe
CC = g++

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

all: $(TARGET)

clean: 
	\rm *.o pram.exe

main.o: 
	$(CC) $(FLAGS) -c main.cpp 
