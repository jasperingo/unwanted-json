
#OBJS specifies which files to compile as part of the project
OBJS = ./src/main.c ./src/unwanted_json.c

#CC specifies which compiler we're using
CC = gcc

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I./includes

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ./build/UnwantedJson

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) -o $(OBJ_NAME)
