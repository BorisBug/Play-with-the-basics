#include <stdio.h>
#include <ctype.h>

void print_0to8()
{
    for(int y=0; y<3; y++)
    {
        for(int x=0; x<3; x++)
            printf("%d ", x*3+y);

        printf("\n");
    }

    printf("\n");
}

void print_dots(int n)
{
    while(n-->0)
        printf(".");
}

int digits(int n)
{
    int digits = 1;
    while(n>=10)
        n/=10, digits++;
    return digits;
}

void print_0to127()
{
    for(int y=0; y<32; y++)
    {
        for(int x=0; x<4; x++)
        {
            int n = x*32+y;
            print_dots(3-digits(n));
            printf("%d ", n);
        }

        printf("\n");
    }

    printf("\n");
}


void print_ASCII()
{
    for(int y=0; y<32; y++)
    {
        for(int x=0; x<4; x++)
        {
            int n = x*32+y;
            print_dots(3-digits(n));

            char c = n;
            if(!isprint(c)) c = '?';
            printf("%d: %c    ", n, c);
        }

        printf("\n");
    }

    printf("\n");
}

int main()
{
    print_0to8();
    print_0to127();
    print_ASCII();
    return 0;
}