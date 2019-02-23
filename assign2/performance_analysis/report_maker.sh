echo "making report please wait.."

COUNT_MAX_ARRAY=(1000 10000 100000 1000000 10000000)

THREAD_ARRAY=(1 2 4 10 20 50 100 200)

THREAD_ARRAY_SIZE=${#THREAD_ARRAY[@]}
COUNT_ARRAY_SIZE=${#COUNT_MAX_ARRAY[@]}


TOTAL_ARRAY_SIZE=$[${THREAD_ARRAY_SIZE}*${COUNT_ARRAY_SIZE}]

COUNTER=0

printf "running mutex with %d cases\n" ${TOTAL_ARRAY_SIZE}
echo "" > report/mutex.txt
for c in ${COUNT_MAX_ARRAY[*]}
do
	for t in ${THREAD_ARRAY[*]}
	do
		COUNTER=$[$COUNTER+1]

		printf "\r%d of %d done" ${COUNTER} ${TOTAL_ARRAY_SIZE}

		echo "mut" ${c} ${t} >> report/mutex.txt
		./mutex/main ${c} ${t} >> report/mutex.txt
		echo "" >> report/mutex.txt
	done
done
printf "\n"


python report_finaliser.py

printf "report made \n"

