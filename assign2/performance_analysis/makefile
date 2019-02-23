all:mutex

forced:
	gcc mutex/main.c -o mutex/main -lpthread
	

mutex:mutex/main.c
	gcc mutex/main.c -o mutex/main -lpthread

run:all
	bash report_maker.sh
