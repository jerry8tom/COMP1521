// BigNum.c ... LARGE positive integer values
// Mukul Raj Sharma
// 09.06.2019
// z5220980

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BigNum.h"

// Initialise a BigNum to N bytes, all zero
void initBigNum (BigNum *bn, int Nbytes)
{
	bn->bytes = malloc(Nbytes*sizeof(Byte));
	assert(bn->bytes);

	for(int i = 0; i < Nbytes; i++)
	{
		bn->bytes[i] = 0;
	}

	bn->nbytes = Nbytes;

}

// Add two BigNums and store result in a third BigNum
void addBigNums (BigNum bnA, BigNum bnB, BigNum *res)
{
	int lenA = bnA.nbytes;
	int lenB = bnB.nbytes;

	int max_len = lenA;
	int min_len = lenB;
	// check input number digit is larger
	if (lenA > lenB)
	{
		Byte *tmp = realloc(res->bytes, (sizeof(unsigned char)*(lenA+1)));
		res->bytes = tmp;
		res->nbytes = lenA;
	}
	else
	{
		Byte *tmp = realloc(res->bytes, (sizeof(unsigned char)*(lenB+1)));
		res->bytes = tmp;
		max_len = lenB;
		min_len = lenA;
	}

	int i = 0;
	int carry = 0;
	//printf("maxlen is %d min len is %d\n", max_len, min_len);
	if (max_len == lenA)
	{
		//printf("enter 1\n");
		for (i = 0; i < max_len; i++)
		{
			if (i < min_len)
			{
				int sum = (int)(bnA.bytes[i] + (int)bnB.bytes[i])-'0'-'0'+carry;
				res->bytes[i] = (sum%10);
				carry = sum/10;
				//printf("sum of %d and %d is %d, carry %d\n", (int)(bnA.bytes[i])-'0', (int)(bnB.bytes[i])-'0', res->bytes[i], carry);
			}
			else
			{
				int sum = (int)(bnA.bytes[i]) - '0' + carry;
				res->bytes[i] = (sum%10);
				carry = sum/10;
				//printf("sum of %d and %d is %d, carry %d\n", (int)(bnA.bytes[i])-'0', 0, res->bytes[i], carry);
			}
		}
	}
	else
	{
		//printf("enter 2\n");
		for (i = 0; i < max_len; i++)
		{
			if (i < min_len)
			{
				int sum = (int)(bnA.bytes[i] + (int)bnB.bytes[i])-'0'-'0'+carry;
				res->bytes[i] = (sum%10);
				carry = sum/10;
				//printf("sum of %d and %d is %d, carry %d\n", (int)(bnA.bytes[i])-'0', (int)(bnB.bytes[i])-'0', res->bytes[i], carry);
			}
			else
			{
				int sum = (int)(bnB.bytes[i]) - '0' + carry;
				res->bytes[i] = (sum%10);
				carry = sum/10;
				//printf("sum of %d and %d is %d, carry %d\n", 0, (int)(bnB.bytes[i])-'0', res->bytes[i], carry);
			}
		}
	}
	if(carry != 0)
	{
		//printf("enter carry\n");
		res->bytes[i] = carry;
		res->nbytes = i+1;
	}
	else
	{
		res->nbytes = i;
	}
	// convert res string from int to char
	for (int j = 0; j <= res->nbytes; j++)
	{
		res->bytes[j] = res->bytes[j] + '0';
	}

	return;
}

int scanBigNum (char *s, BigNum *bn)
{	
	int s_len = strlen(s);
	int count_dig = 0;

	if (s_len > bn->nbytes)
	{
		//printf("enter realloc\n");
		Byte *tmp = realloc(bn->bytes, (sizeof(Byte)*s_len));
		bn->bytes = tmp;
		assert(bn->bytes);
	}

	for (int i = s_len-1; i >= 0; i--)
	{
		if (isdigit(s[i]))
		{
			bn->bytes[count_dig] = s[i];
			count_dig++;
		}
	}
	
	bn->nbytes = count_dig;

	if (count_dig > 0)
		return 1;
	else 
		return 0;
}

// Display a BigNum in decimal format
void showBigNum (BigNum bn)
{
	int flag;

	for (int j = bn.nbytes; j >= 0; j--)
	{
		if(((int)bn.bytes[j]-'0') > 0)
		{
			flag = j;
			break;
		}
	}
	for (int j = flag; j >= 0; j--)
	{
		printf("%c", bn.bytes[j]);
	}
	return;
}
