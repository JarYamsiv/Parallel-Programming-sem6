import matplotlib.pyplot as plt
import sys
import random

def ret_rand_col(arg):
	return '#'+"%06x" % random.randint(0, 0xFFFFFF)

file_name = sys.argv[1]

print("making graphs for "+file_name)



mrf = open("report/"+file_name+".txt","r")



contents = mrf.readlines()

mutex_report_dict = {}
counts = {}
t_counts = {}
time_taken = 0
count = 0
thread_count = 0

for i in range(len(contents)):
	splited = contents[i].split(" ")

	if i%3 == 1:
		count = int(splited[1])
		counts[count] = 1
		thread_count = int(splited[2])
		t_counts[thread_count] = 1

	if i%3 == 2:
		time_taken = float(splited[0])
		mutex_report_dict[(count,thread_count)] = time_taken
		

count_key = list(counts.keys())
t_keys = list(t_counts.keys())

#print(count_key)
#print(t_keys)
colors = ['b','r','g','c','m','y','k']
colors.extend( [ ret_rand_col(x) for x in count_key] )

plt.ylabel("time takes (s)")
plt.xlabel("number of threads")
for (i,c) in enumerate(count_key):
	t_array = []
	s_array = []
	for t in t_keys:	
		t_array.append(t)
		s_array.append(mutex_report_dict[(c,t)])

	plt.plot(t_array,s_array,colors[i],label="count="+str(c))
plt.legend()
plt.savefig('report/final/'+file_name+'.png', dpi=500)
		
