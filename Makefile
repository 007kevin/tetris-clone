#OBJS specifies which files to compile as part of the project
OBJS = main.cpp init.cpp grid.cpp cell.cpp

#CC specifies which compiler we're using
CC = g++ -std=c++11

#COMPILER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our executable
OBJ_NAME = output

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

