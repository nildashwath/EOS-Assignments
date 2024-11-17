#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct range
{
	int start;
	int end;
	int sum;
}range_t;

void* cal_sum(void* arg)
{
	range_t* range = (range_t*)arg;
	range->sum = 0;
	
	for(int i = range->start; i<= range->end; i++)
	{
	range->sum += i;
	}
	pthread_exit(0);
}
int main()
{
	pthread_t th;
	range_t range;
	int ret, start, end;

	printf("Enter start number:");
	scanf("%d", &start);

	printf("Enter end number:");
	scanf("%d", &end);

	if(start > end)
	{
		printf("Invaild range..! Start num should be less or equal to end num. ");

		return 1;
	}

	range.start = start;
	range.end = end;
	range.sum = 0;

	ret = pthread_create(&th, NULL, cal_sum, &range);
	if(ret < 0)
	{
		perror("thread() failed");
		exit(1);
	}
	pthread_join(th, NULL);

	printf("Sum of integers from %d to %d is %d\n", range.start, range.end, range.sum);

	return 0;
}
