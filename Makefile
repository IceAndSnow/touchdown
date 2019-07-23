SRC_DIR = ./src
BUILD_DIR = ./build

SOURCES = $(shell find $(SRC_DIR)/ -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

run: Touchdown
	./Touchdown

Touchdown: $(OBJECTS) Makefile
	g++ -o $@ -O3 --std=c++11 $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	g++ -c -o $@ $^

clean:
	-rm -r $(BUILD_DIR)
