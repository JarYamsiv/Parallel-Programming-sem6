#include <iostream>
#include <stdlib.h>

#define INF 999999

using namespace std;

void floydWarshall(int** graph,int n);

int main(){

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

	floydWarshall(graph,n);


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

			if(graph[i][j]==-1){
				dist[i][j] = INF;
			}
			else{
				dist[i][j] = graph[i][j];
			}
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
				cout<<"INF";
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
		
