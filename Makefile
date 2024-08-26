CXX = g++

all: app

app: ./src/input_output.cpp
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