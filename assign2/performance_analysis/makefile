all:mutex busy_wait condition_variable

forced:
	gcc mutex/main.c -o mutex/main -lpthread
	gcc busy_wait/main.c -o busy_wait/main -lpthread
	gcc condition_variable/main.c -o condition_variable/main -lpthread
	

mutex:mutex/main.c
	gcc mutex/main.c -o mutex/main -lpthread

busy_wait:busy_wait/main.c
	gcc busy_wait/main.c -o busy_wait/main -lpthread

condition_variable:condition_variable/main.c
	gcc condition_variable/main.c -o condition_variable/main -lpthread

run:all
	bash report_maker.sh

clean:
	rm report/*.txt
	rm report/final/*.png
