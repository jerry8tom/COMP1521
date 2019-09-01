// COMP1521 19T2 ... lab 1
// cat3: Copy input to output

// Mukul Raj Sharma
// 04/06/2019
// z5220980

#include <stdio.h>
#include <stdlib.h>

static void copy (FILE *, FILE *);

int main (int argc, char *argv[])
{
	copy (stdin, stdout);
	return EXIT_SUCCESS;
}

// Copy contents of input to output, line by line using fgets/fputs
// Assumes both files open in appropriate mode
static void copy (FILE *input, FILE *output)
{
	char inp[BUFSIZ];

	while(fgets(inp, BUFSIZ, input))
	{
		fputs(inp, output);
	}
}
