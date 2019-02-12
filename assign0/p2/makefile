all:term.out
	

%.out:%.cpp
	g++ $< -o $@ -lpthread -std=c++11

test:all
	bash testcases/t1.sh 1| ./term.out > testcases/t1_op.txt
	bash testcases/t1.sh 2| ./term.out > testcases/t2_op.txt
	bash testcases/t1.sh 3| ./term.out > testcases/t3_op.txt
	bash testcases/t1.sh 4| ./term.out > testcases/t4_op.txt
