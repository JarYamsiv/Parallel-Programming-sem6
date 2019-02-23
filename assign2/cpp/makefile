all:mutex busy_wait

forced:FORCE
	g++ mutex/main.cpp -o mutex/main -lpthread -std=c++11 -O
	g++ busy_wait/main.cpp -o busy_wait/main -lpthread -std=c++11 

mutex:mutex/main.cpp
	g++ mutex/main.cpp -o mutex/main -lpthread -std=c++11 -O

busy_wait:busy_wait/main.cpp
	g++ busy_wait/main.cpp -o busy_wait/main -lpthread -std=c++11 

run:all
	./report_maker.sh

clean:
	rm mutex/main
	rm busy_wait/main
	rm -r report/*

FORCE: