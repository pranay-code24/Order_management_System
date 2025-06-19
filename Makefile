CXX = clang++
CXXFLAGS = -std=c++17 -pthread
INCLUDES = -Iinclude
SRC = src/OrderManagement.cpp src/main.cpp
OUT = build/order_system

all: build $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -rf build responses.csv tools/gen_responses

# 🔧 Mock data generation
mock: tools/gen_responses
	./tools/gen_responses

tools/gen_responses: tools/GenerateMockResponses.cpp
	$(CXX) -std=c++17 tools/GenerateMockResponses.cpp -o tools/gen_responses
