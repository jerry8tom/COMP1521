// COMP1521 19t2 ... lab 2 warm-up

// Mukul Raj Sharma
// 09.06.2019
// z5220980

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void range_uchar (void);
static void range_char (void);
static void range_uhint (void);
static void range_hint (void);
static void range_int (void);
static void range_uint (void);
static void range_lint (void);
static void range_ulint (void);
static void range_llint (void);
static void range_ullint (void);

int main (void)
{
	range_char ();
	range_uchar ();
	range_hint ();
	range_uhint ();
	range_int ();
	range_uint ();
	range_lint ();
	range_ulint ();
	range_llint ();
	range_ullint ();

	return EXIT_SUCCESS;
}

// Calculate and print the largest and smallest `int` values
static void range_int (void)
{
	unsigned int max = 0; 
    max = ~max; 
    max = max >> 1; 
      
    int min = max; 
    min = ~min; 
	printf (
		"                   int (%zu bytes): %d ... %d\n",
		sizeof (int), min, max
	);
}

// Calculate and print the largest and smallest `unsigned int` values
static void range_uint (void)
{
	unsigned int max = 0;
	max = ~max;
	max = max >> 1;
	
	printf (
		"          unsigned int (%zu bytes): %u ... %u\n",
		sizeof (unsigned int), 0, 2*max+1
	);
}

// Calculate and print the largest and smallest `long int` values
static void range_lint (void)
{
	unsigned long int max = 0;
	max = ~max;
	max = max >> 1;

	long int min = max;
	min = ~max;
	printf (
		"              long int (%zu bytes): %ld ... %ld\n",
		sizeof (long int), min, max
	);
}

// Calculate and print the largest and smallest `unsigned long int` values
static void range_ulint (void)
{
	unsigned long int max = 0;
	max = ~max;
	max = max >> 1;

	printf (
		"     unsigned long int (%zu bytes): %d ... %lu\n",
		sizeof (unsigned long int), 0, 2*max+1
	);
}

// Calculate and print the largest and smallest `long long int` values
static void range_llint (void)
{
	unsigned long long int max = 0;
	max = ~max;
	max = max >> 1;

	long long int min = max;
	min = ~max;
	printf (
		"         long long int (%zu bytes): %lld ... %lld\n",
		sizeof (long long int), min, max
	);
}

// Calculate and print the largest and smallest `unsigned long long int` values
static void range_ullint (void)
{
	unsigned long long int max = 0;
	max = ~max;
	max = max >> 1;

	printf (
		"unsigned long long int (%zu bytes): %d ... %llu\n",
		sizeof (unsigned long long int), 0, 2*max+1
	);
}

// Calculate and print the largest and smallest `short int` values
static void range_hint (void)
{
	unsigned short int max = 0;
	max = ~max;
	max = max >> 1;

	short int min = max;
	min = ~min;

	printf (
		"             short int (%zu bytes): %hd ... %hd\n",
		sizeof (short int), min, max
	);
}

// Calculate and print the largest and smallest `unsigned short int` values
static void range_uhint (void)
{
	unsigned short int max = 0;
	max = ~max;
	max = max >> 1;

	printf (
		"    unsigned short int (%zu bytes): %hu ... %hu\n",
		sizeof (unsigned short int), 0, 2*max+1
	);
}

// Calculate and print the largest and smallest `char` values
static void range_char (void)
{
	unsigned char max = 0;
	max = ~max;
	max = max >> 1;

	char min = max;
	min = ~min;

	printf (
		"                  char (%zu bytes): %d ... %d\n",
		sizeof (char), min, max
	);
}

// Calculate and print the largest and smallest `unsigned char` values
static void range_uchar (void)
{
	unsigned char max = 0;
	max = ~max;
	max = max >> 1;

	printf (
		"         unsigned char (%zu bytes): %d ... %d\n",
		sizeof (unsigned char), 0, 2*max+1
	);
}
