all:mutex busy_wait

forced:
	gcc mutex/main.c -o mutex/main -lpthread
	gcc busy_wait/main.c -o busy_wait/main -lpthread
	

mutex:mutex/main.c
	gcc mutex/main.c -o mutex/main -lpthread

busy_wait:busy_wait/main.c
	gcc busy_wait/main.c -o busy_wait/main -lpthread

run:all
	bash report_maker.sh

clean:
	rm report/final/*.png
	rm report/*.txt