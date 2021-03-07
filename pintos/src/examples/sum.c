/* sum.c

   Prints fibonacci and sum_of_four_int */

#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int
main (int argc, char **argv) 
{
	int n[4], i, fibo, a, b, sum=0;
	fibo = a = b = 1;
	for (i = 1; i < argc; i++) { n[i - 1] = atoi(argv[i]); sum += n[i - 1]; }
	if (n[0] > 2) {
		for (i = 0; i < n[0] - 2; i++) {
			fibo = a + b;
			a = b;
			b = fibo;
		}
	}
	printf("%d %d\n", fibo, sum);
	return EXIT_SUCCESS;
}
