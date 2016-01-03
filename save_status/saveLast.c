#include <stdio.h>
#include <stdlib.h>

#define RATE 100

void getValue(int value)
{
	printf("%s\n",__func__);
	static float rateCur = 0;
	static float rateLast = 0;
	static int count = 1;
	if(count == 1)
	{
		printf("1111\n");
		rateCur = RATE;
	}
	if(value >= 100)
	{
		rateCur = rateCur*0.8;
		rateLast = rateCur;
	}
	rateCur = rateLast;
	printf("rateCur is %f\n",rateCur);
	printf("rateLast is %f\n",rateLast);
	count++;
}

int main()
{
	int i = 0;
	for(;i<3;i++)
	{
		getValue(100+i);
	}
	return 0;
}
