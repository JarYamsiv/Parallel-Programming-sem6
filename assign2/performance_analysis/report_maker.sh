echo "making report please wait.."

echo "the detailed reports ie, *.txt is of the format
<pgm_name> count thread_count
time elapsed in seconds" > report/how_to_read.txt

#======================================================================================================
#
#				TEST CASES FOR MUTEX AND CV ARE DIFFERENT FROM BUSY_WAIT
#
#======================================================================================================


PROGRAMS=(mutex condition_variable)

COUNT_MAX_ARRAY=(50 100 200 500 1000 10000 100000 1000000)
THREAD_ARRAY=(1 2 3 4 5 6 7 8 9 10)
THREAD_ARRAY_SIZE=${#THREAD_ARRAY[@]}
COUNT_ARRAY_SIZE=${#COUNT_MAX_ARRAY[@]}
TOTAL_ARRAY_SIZE=$[${THREAD_ARRAY_SIZE}*${COUNT_ARRAY_SIZE}]



for p in ${PROGRAMS[*]}
do
	printf "\nrunning %s with %d cases\n" ${p} ${TOTAL_ARRAY_SIZE}
	COUNTER=0
	echo "" > report/${p}.txt
	for c in ${COUNT_MAX_ARRAY[*]}
	do
		for t in ${THREAD_ARRAY[*]}
		do
			COUNTER=$[$COUNTER+1]

			printf "\r%d (count:%d , num_thread:%d) of %d done" ${COUNTER} ${c} ${t} ${TOTAL_ARRAY_SIZE}
			echo ${p} ${c} ${t} >> report/${p}.txt
			./${p}/main ${c} ${t} >> report/${p}.txt
			echo "" >> report/${p}.txt
		done
	done
	printf "\n"  
done


#======================================================================================================
#
#				BUSY_WAIT WILL TAKE TOO MUCH TIME WHEN THE NUMBER OF THREADS ARE GRATER THAN
#				HARDWARE CONCURRENCY . HENCE A DIFFERENT TEST CASES
#
#======================================================================================================

COUNT_MAX_ARRAY=(50 100 200 500 1000)
THREAD_ARRAY=(1 2 3 4 5 6 7 8)
THREAD_ARRAY_SIZE=${#THREAD_ARRAY[@]}
COUNT_ARRAY_SIZE=${#COUNT_MAX_ARRAY[@]}
TOTAL_ARRAY_SIZE=$[${THREAD_ARRAY_SIZE}*${COUNT_ARRAY_SIZE}]

p=busy_wait

printf "\nrunning %s with %d cases\n" ${p} ${TOTAL_ARRAY_SIZE}
COUNTER=0
echo "" > report/${p}.txt
for c in ${COUNT_MAX_ARRAY[*]}
do
	for t in ${THREAD_ARRAY[*]}
	do
		COUNTER=$[$COUNTER+1]

		printf "\r%d (count:%d , num_thread:%d) of %d done" ${COUNTER} ${c} ${t} ${TOTAL_ARRAY_SIZE}
		echo ${p} ${c} ${t} >> report/${p}.txt
		./${p}/main ${c} ${t} >> report/${p}.txt
		echo "" >> report/${p}.txt
	done
done
printf "\n" 


#======================================================================================================
#
#				MAKING GRAPHS USING PYTHON 
#
#======================================================================================================


PROGRAMS=(mutex condition_variable busy_wait)

for p in ${PROGRAMS[*]}
do
	python report_finaliser.py ${p}
done

lscpu > report/hardware_spec.txt
printf "report made \n"




