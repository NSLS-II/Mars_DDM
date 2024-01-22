#include <stdio.h>
#include <stdlib.h>

unsigned int reverseBits(int nobits, unsigned int num) 
{  
	unsigned int reverse_num = 0; 
	int i; 
	for (i = 0; i < nobits; i++) 
	{ 
		if((num & (1 << i))) 
		reverse_num |= 1 << ((nobits - 1) - i); 
	} 
	return reverse_num; 
} 
  
int main(int argc, char *argv[]){

unsigned int num, nobits;

num=atoi(argv[2]);
nobits=atoi(argv[1]);

printf("num= %x, nobits=%i\n",num,nobits);
printf("reversed= %x\n",reverseBits(nobits,num));

return(0);
}
