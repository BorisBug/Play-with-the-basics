#include <stdint.h>
#include <stdio.h>

void printbyte(uint8_t b)
{
	for(int i=7; i>=0; i--, b<<=1)
		printf("%c", b&0x80 ? '1' : '0');
}

void printmem(char *lbl, void *p, int size)
{
	printf("%s ", lbl);

	uint8_t *b = p;
	for(int i=size-1; i>=0; i--)
	{
		printbyte(b[i]);
		printf(" ");
	}

	printf("\n");
}

uint64_t make_unsigned(int64_t val, uint8_t bits)
{
	uint64_t mask = ~0ull>>(64-bits);
	printmem("mk un - before mask", &val, sizeof(uint64_t));
	val &= mask;
	printmem("mk un - after mask ", &val, sizeof(uint64_t));
	return val;
}

int64_t make_signed(uint64_t val, uint8_t bits)
{
	uint64_t mask = ~0ull>>(64-bits);
	printmem("mk sg - before mask", &val, sizeof(uint64_t));
	int64_t res = val & mask; // mask out extra bits
	printmem("mk sg - after mask ", &res, sizeof(uint64_t));
	if(val & (1ull<<(bits-1))) // if negative bit is set..
		res = -((~val+1)&mask); // undo the complement-1
	printmem("mk sg - after test ", &res, sizeof(uint64_t));
	return res;
}

/*
TEST: unsigned VS signed
       u 3 s   u 2 s   u 1 s
       -----   -----   -----
000    0   0   0   0   0   0
001    1   1   1   1   1  -1 >> what do we do with this case?
010    2   2   2  -2   
011    3   3   3  -1
100    4  -4   uns[0..3] in 2 bits
101    5  -3   sig[-2..1]
110    6  -2
111    7  -1
       uns[0..7] in 3 bits
       sig[-4..3]
*/	

int main()
{
	int64_t s1=4, s2=-4;
	uint64_t u1=0, u2=0;
	uint8_t bits = 3;

    u1 = make_unsigned(s1, bits);
    u2 = make_unsigned(s2, bits);
    printf("from s1=%lld to u1:%llu\n", s1, u1);
    printf("from s2=%lld to u2:%llu\n", s2, u2);

  	s1 = make_signed(u1, bits);
    s2 = make_signed(u2, bits);
	printf("from u1=%llu to s1:%lld\n", u1, s1);
    printf("from u2=%llu to s2:%lld\n", u2, s2);
    return 0;
}
