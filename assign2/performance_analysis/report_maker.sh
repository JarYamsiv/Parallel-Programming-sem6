echo "making report please wait.."

COUNT_MAX_ARRAY=(1000 10000 100000 1000000)

THREAD_ARRAY=(1 2 3 4)

PROGRAMS=(mutex busy_wait)

THREAD_ARRAY_SIZE=${#THREAD_ARRAY[@]}
COUNT_ARRAY_SIZE=${#COUNT_MAX_ARRAY[@]}


TOTAL_ARRAY_SIZE=$[${THREAD_ARRAY_SIZE}*${COUNT_ARRAY_SIZE}]




for p in ${PROGRAMS[*]}
do
	printf "running %s with %d cases\n" ${p} ${TOTAL_ARRAY_SIZE}
	COUNTER=0
	echo "" > report/${p}.txt
	for c in ${COUNT_MAX_ARRAY[*]}
	do
		for t in ${THREAD_ARRAY[*]}
		do
			COUNTER=$[$COUNTER+1]

			printf "\r%d of %d done" ${COUNTER} ${TOTAL_ARRAY_SIZE}

			echo "mut" ${c} ${t} >> report/${p}.txt
			./${p}/main ${c} ${t} >> report/${p}.txt
			echo "" >> report/${p}.txt
		done
	done
	printf "\n"
done


for p in ${PROGRAMS[*]}
do
	python report_finaliser.py ${p}
done

lscpu > report/hardware_spec.txt
printf "report made \n"




