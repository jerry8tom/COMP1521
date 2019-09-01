// COMP1521 19T2 ... lab 1
// cat4: Copy input to output

// Mukul Raj Sharma
// 04/06/2019
// z5220980

#include <stdio.h>
#include <stdlib.h>

static void copy (FILE *, FILE *);

int main (int argc, char *argv[])
{
	char *error = "Can't read";

	if (argc == 1)
	{
		copy (stdin, stdout);
	}
	else 
	{
		for (int i = 1; i < argc; i++)
		{
			FILE *input;

			if((input = fopen(argv[i], "r")) == NULL)
			{
				printf("%s %s\n", error, argv[i]);
			}
			else
			{
				copy(input, stdout);
				fclose(input);
			}
		}
	}
	
	return EXIT_SUCCESS;
}

// Copy contents of input to output, line-by-line using fgets/fputs
// Assumes both files open in appropriate mode
static void copy (FILE *input, FILE *output)
{
	char inp[BUFSIZ];
	int line_n = 1;

	while(fgets(inp, BUFSIZ, input))
	{	
		printf("%6d\t%s", line_n, inp);
		//fputs(inp, output);
		line_n++;
	}
}
