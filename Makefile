all: test

test: ./src/input_output.cpp
	g++ -o test ./src/input_output.cpp

run: test
	./test

clean:
	rm -f test