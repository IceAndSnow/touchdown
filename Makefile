SRC_DIR = ./src
BUILD_DIR = ./build

SOURCES = $(shell find $(SRC_DIR)/ -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPENDENCIES = $(OBJECTS:%.o=%.d)

run: Touchdown
	./Touchdown

Touchdown: $(OBJECTS) Makefile
	g++ -o $@ --std=c++11 $(OBJECTS)

-include $(DEPENDENCIES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	g++ -MMD -c -O3 -o $@ $<

clean:
	-rm -r $(BUILD_DIR) || true
	-rm Touchdown || true
