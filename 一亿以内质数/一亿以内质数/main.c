#include<stdio.h>
#include<time.h>

#define N 100000000

/*一亿以内质数总数为5761455*/

void main()
{
	int begintime, endtime;
	begintime = clock();
	static char array[N/2] = { 0 };
	array[0] = 2;
	for (int i = 3; i < N; i+=2)
	{
		array[i/2] = i;
	}
	int cnt = N/2;
	for (int i = 3; i < N; i+=2)
	{
		
		if (i*i <= N)
		{
			for (int j = i * i; j < N; j += 2 * i)
			{
				if (array[j/2] != 0)
				{
					cnt--;
					array[j/2] = 0;
				}
			}
		}
		else
		{
			break;
		}
	}
	endtime = clock();
	printf("%d\n\nRunning Time：%dms\n",cnt, endtime - begintime);
}