# https://dev.to/dmendoza99/how-to-write-a-good-makefile-for-c-32e3
# SDL include notes: https://forums.libsdl.org/viewtopic.php?t=5997

CXX				:= g++
CXX_FLAGS		:= -Wall -Wextra -std=c++17 -ggdb $(sdl2-config --cflags)
LINKER_FLAGS	:= -lSDL2 -lGL -lSDL2_image
BIN				:= bin
SRC				:= src
INCLUDE			:= include
LIB				:= LIB
LIBRARIES		:= 
EXECUTABLE		:= cavestory


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	@echo "Executing..."
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	@echo "Building..."
	$(CXX) $^  $(CXX_FLAGS) -I /usr/include/SDL2/ -I$(INCLUDE) $(LINKER_FLAGS) -L$(LIB) -o $@ $(LIBRARIES)

clean:
	@echo "Cleaning..."
	-rm $(BIN)/*