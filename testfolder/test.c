#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

int main(int argc, char const *argv[])
{
	void* x = malloc(SIZE_MAX + 1);
	if(NULL == x){
		printf("DID NOT WORK");
	}
}
