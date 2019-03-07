#include <stdio.h>

int main(int argc, char const *argv[])
{
	if(argc>1)
	{
		int n = atoi(argv[1]);
		int i,j;

		printf("%d\n",n ); 

		for (i = 0; i < n; ++i)
		{
			for (j = 0; j < n; ++j)
			{
				int val = rand()%20 - 1;
				printf("%d ", val );
			}
			printf("\n");
		}
	}
	return 0;
}