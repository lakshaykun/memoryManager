CXX = g++

all: app test

app: ./src/input_output.cpp
	@mkdir -p bin
	@mkdir -p traces
	@mkdir -p output
	$(CXX) ./src/input_output.cpp -o ./bin/app.out

run: 
	./bin/app.out

test: ./test/test.cpp
	$(CXX) ./test/test.cpp -o ./bin/test.out

trace: ./bin/test.out
	./bin/test.out
	
clean:
	rm -f ./bin/*.out
	rm -f ./traces/*.txt
	rm -f ./output/*.csv