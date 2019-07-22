SRC_DIR = ./src
BUILD_DIR = ./build

sources = $(shell find $(SRC_DIR)/ -name '*.cpp')
objects = $(sources:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

run: Touchdown
	./Touchdown

Touchdown: $(objects) Makefile
	g++ -o $@ --std=c++11 $(objects)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	g++ -c -o $@ $^

clean:
	-rm -r $(BUILD_DIR)
