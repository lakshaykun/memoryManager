CXX = g++

# Default target
all: ./bin/app.out ./bin/test.out

# Compile the application
./bin/app.out: ./src/input_output.cpp
	@mkdir -p bin
	@mkdir -p traces
	@mkdir -p output
	$(CXX) ./src/input_output.cpp -o ./bin/app.out

# Run the application
run: 
	./bin/app.out

# Compile the test
./bin/test.out: ./test/test.cpp
	$(CXX) ./test/test.cpp -o ./bin/test.out -lpthread

# Run the test
trace: ./bin/test.out
	./bin/test.out

# Clean the project	
clean:
	rm -f ./bin/*.out
	rm -f ./traces/trace.txt
	rm -f ./output/*.csv