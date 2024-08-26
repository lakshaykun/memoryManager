CXX = g++

# Default target
all: app test

# Compile the application
app: ./src/input_output.cpp
	@mkdir -p bin
	@mkdir -p traces
	@mkdir -p output
	$(CXX) ./src/input_output.cpp -o ./bin/app.out

# Run the application
run: 
	./bin/app.out

# Compile the test
test: ./test/test.cpp
	$(CXX) ./test/test.cpp -o ./bin/test.out

# Run the test
trace: ./bin/test.out
	./bin/test.out

# Clean the project	
clean:
	rm -f ./bin/*.out
	rm -f ./traces/trace.txt
	rm -f ./output/*.csv