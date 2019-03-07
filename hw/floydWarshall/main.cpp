#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#define INF 999999

using namespace std;

pthread_barrier_t iter_barrier;

void floydWarshall(int** graph,int n);
void floydWarshallThreadBased(int** graph,int n,int t);
void* workerThread(void* args);

int main(int argc,char* argv[]){

	int t_count;
	if(argc>1)
	{
		t_count = atoi(argv[1])>0?atoi(argv[1]):1;
	}
	else
	{
		t_count = 4;
	}

	int 		n;			/*	the number of vertices	*/
	int 		**graph;	/*	the array used to store adjacency matrice	*/

	cin>>n;					/*	getting number of nodes / vertices*/


	/*	mallocing space for the adjacency martice	*/
	graph = (int**)malloc(sizeof(int*)*n);
	for(int i=0; i<n; i++){
		graph[i] = (int*)malloc(n*sizeof(int));
	}

	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){
			cin>>graph[i][j];
		}
	}

	long double time_normal;
	long double time_thread;
    struct timespec start,end;

    clock_gettime(CLOCK_REALTIME, &start);
	floydWarshall(graph,n);
	clock_gettime(CLOCK_REALTIME, &end);

	time_normal = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1000000000.0);


	clock_gettime(CLOCK_REALTIME, &start);
	floydWarshallThreadBased(graph,n,t_count);
	clock_gettime(CLOCK_REALTIME, &end);

	time_thread = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1000000000.0);

	cout<<"normal time: "<<time_normal<<"thread time : "<<time_thread<<endl;


	for (int i = 0; i < n; ++i){
		free(graph[i]);
	}
}

void floydWarshall(int** graph,int n){

	int **dist;

	dist = (int**)malloc(n*sizeof(int*));
	for (int i = 0; i < n; ++i){
		dist[i]=(int*)malloc(n*sizeof(int));
	}

	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){

			dist[i][j] = graph[i][j]==-1? INF : graph[i][j];
		}
	}

	for (int k = 0; k < n; ++k){
		for (int i = 0; i < n; ++i){
			for (int j = 0; j < n; ++j){
				
				if(dist[i][k]+dist[k][j]<dist[i][j]){
					dist[i][j] = dist[i][k]+dist[k][j];
				}

			}
		}
	}


	cout<<"the dist matrix is:\n";
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){
			if(dist[i][j]>=INF){
				cout<<" INF ";
			}
			else{
				cout<<dist[i][j]<<" ";
			}
		}
		cout<<endl;
	}




	for (int i = 0; i < n; ++i){
		free(dist[i]);
	}
}


typedef struct arg_t
{
	int **g_addr;
	int n;
	int i;
	int t;

}arg_t;

void floydWarshallThreadBased(int** graph,int n,int t){
	int **dist;

	dist = (int**)malloc(n*sizeof(int*));
	for (int i = 0; i < n; ++i){
		dist[i]=(int*)malloc(n*sizeof(int));
	}

	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){

			dist[i][j] = graph[i][j]==-1? INF : graph[i][j];

		}
	}

	pthread_t *threads =(pthread_t*)malloc(t*sizeof(pthread_t));

	arg_t* arguments = (arg_t*)malloc(t*sizeof(arg_t));

	pthread_barrier_init(&iter_barrier, NULL, t + 1);

	int t_id;
	int result_code;
	for (int i = 0; i < t; ++i){

			t_id = i;
			arguments[t_id].g_addr = dist;
			arguments[t_id].n = n;
			arguments[t_id].i = i;
			arguments[t_id].t = t;

			result_code = pthread_create(&threads[t_id],NULL,workerThread,(void*)&arguments[t_id]);
			assert(!result_code);

	}

	pthread_barrier_wait(&iter_barrier);
	for (int i = 0; i < n; ++i)
	{
		pthread_barrier_wait(&iter_barrier);
	}

	for (int i = 0; i < t; ++i)
	{
			t_id = i;
			result_code = pthread_join(threads[t_id],NULL);
			assert(!result_code);

	}


	cout<<"the dist matrix is:\n";
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){
			if(dist[i][j]>=INF){
				cout<<" INF ";
			}
			else{
				cout<<dist[i][j]<<" ";
			}
		}
		cout<<endl;
	}





	free(threads);
	free(arguments);
	for (int i = 0; i < n; ++i){
		free(dist[i]);
	}
}

void* workerThread(void* arguments_void){

	arg_t *args=(arg_t*)arguments_void; 

	int **d = args->g_addr;
	int n 	= args->n;
	int i 	= args->i;
	int t 	= args->t;

	int beg = (n/t)*i;
	int end = (n/t + 1)*i<n?(n/t + 1):n;


	pthread_barrier_wait(&iter_barrier);
	for(int k=0; k<n; k++)
	{
		// cout<<"t :"<<i*n+j<<" waiting at "<<k<<endl;
		for(int i=beg; i<end; i++)
		{
			for (int j = 0; j < n; ++j)
			{
				if( d[i][k]+d[k][j] <d[i][j]  )d[i][j] = d[i][k]+d[k][j];
			}
		}
		
		pthread_barrier_wait(&iter_barrier);
	}
	
}
		
