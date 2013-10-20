#include <stdio.h> 
#include <stdlib.h> 
#include <ncurses.h>

int main() 
{ 
 int *x; 
 int **y; 
 int a[] = {1,2,3,4,5}; 
 x=&a[1]; 
 y=&x; 
 --*y; 
 *x=2; 
 x=a; 
 int i; 
 for(i =0; i < 5; i++) 
 { 
 **y = **y**x; 
 ++*y; 
 } 
 for(i = 0; i< 5; i++) 
 { 
 printf("index %d = %d\n", i, a[i]); 
 } 
 
 //****Extra Fun******** 
 // assume a little endian machine 
 char str[5] = "cats"; 
 int *newStr = (int*)str; 
 *newStr = (*newStr)<<8; 
 *newStr|='c'; 
 printf("%s\n",str);// Extra Fun output 
 //********************** 
}
