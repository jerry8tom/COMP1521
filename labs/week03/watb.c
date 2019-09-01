// COMP1521 19t2 ... lab 03: where are the bits?
// watb.c: determine bit-field order

#include <stdio.h>
#include <stdlib.h>

struct _bit_fields {
	unsigned int a : 4;
	unsigned int b : 8;
	unsigned int c : 20;
};

union bit_fields {
	struct _bit_fields obj;
	unsigned int obj_1;
};
int main (void)
{
	union bit_fields x;
	x.obj.a = 0x1;
	x.obj.b = 0x00;
	x.obj.c = 0x00000;

	//unsigned int *ptr = &(x.a);

	printf ("%#010x\n", x.obj_1);

	return EXIT_SUCCESS;
}
