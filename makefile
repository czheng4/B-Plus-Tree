all: bin/main bin/example

FLAGS = -O3 -std=c++14 -Wall -Wextra -g
INCLUDE = -Iinclude/

obj/main.o: src/main.cpp
	c++ $(FLAGS) $(INCLUDE) -c -o $@ $< 

obj/example.o: src/example.cpp
	c++ $(FLAGS) $(INCLUDE) -c -o $@ $< 


bin/main: obj/main.o 
	c++ $(FLAGS) $(INCLUDE) -o $@ $^

bin/example: obj/example.o
	c++ $(FLAGS) $(INCLUDE) -o $@ $^

clean:
	rm obj/* bin/*