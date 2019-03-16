echo "running">report/temp.txt

./mutex/main 1000000 10
./busy_wait/main 1000000 10

python ploter.py