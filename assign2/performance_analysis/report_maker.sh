echo "making report please wait.."

COUNT_MAX_ARRAY=(1000 10000 100000 1000000 1000000)

THREAD_ARRAY=(1 2 4 10 20 50 100)

echo "" > report/mutex.txt
for c in ${COUNT_MAX_ARRAY[*]}
do
	for t in ${THREAD_ARRAY[*]}
	do
		echo "mut" ${c} ${t} >> report/mutex.txt
		./mutex/main ${c} ${t} >> report/mutex.txt
		echo "" >> report/mutex.txt
	done
done

echo "report made "

