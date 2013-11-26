#include <stdio.h>

int main(int argc, char const *argv[])
{
	int x, y, z;
	fscanf(stdin ,"Head(%d, %d, %d)", &x, &y, &z);
	printf("%d %d %d\n", x, y, z);
	return 0;
}