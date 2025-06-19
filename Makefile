CXX = clang++
CXXFLAGS = -std=c++17 -pthread

INCLUDES = -Iinclude
SRC = src/OrderManagement.cpp src/main.cpp
OUT = build/order_system

all:
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -rf build
