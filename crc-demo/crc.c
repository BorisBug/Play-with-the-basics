/**
 * @file crc.c
 * @author Boris Snäll (borissnaell@gmail.com)
 * @brief Simple algorithm to visually show the calculation of CRC-3 on
 *        a 32 bit register. NOTE: the algorithm looks complex because of the
 *        visual priority. The real algorithm is way more simple than this.
 * @version 0.3
 * @date 2021-10-25
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

// change HERE to use different examples, or create your own values!
#define POLY_BITS 3 // 3=default, are presetted 4,5,12 

// diferent examples fitting in 32 bits
#if POLY_BITS==4
    #define CRC_W POLY_BITS
    #define MESSAGE_W 28
    #define MESSAGE 0b1001001011110111010011101100UL
    #define POLY 0b10101UL
#elif POLY_BITS==5
    #define CRC_W POLY_BITS
    #define MESSAGE_W 24
    #define MESSAGE 0b100100101110111010101011UL
    #define POLY 0b110101UL
#elif POLY_BITS==12
    #define CRC_W POLY_BITS
    #define MESSAGE_W 20
    #define MESSAGE 0b10010010111011101010UL
    #define POLY 0b1101110100101UL
#else // Default-> Example in PDF: CRC 3, MESSAGE 14, POLY 0b1011, REG: 0b11010011101100
    #define CRC_W 3
    #define MESSAGE_W 14
    #define MESSAGE 0b11010011101100UL
    #define POLY 0b1011UL
#endif

#define if_printf      if(print) printf
#define if_print_bin   if(print) print_bin
#define if_print_line  if(print) print_line

// print the binary representation of 32 bits with
// a special format for this specific exercise
void print_bin(uint32_t x, int spaces, int start, int amount, bool value)
{
    assert(start>=0 && start<=31); // index
    assert(amount>=1 && start<=32); // amount

    while(spaces-->0)
        printf(".");

    uint32_t mask = 1UL<<start;

    int i=0;
    for(i=start; i>=0 && amount>0; amount--, i--, mask>>=1)
    {
        if(i==CRC_W-1)
            printf(" ");
        printf("%c", x & mask?'1':'0');
    }

    for(; i>=0; i--)
    {
        if(i==CRC_W-1)
            printf(" ");
        printf(":");
    }

    if(value)
        printf(" 0x%08X", x);

    printf("\n");
}

void print_line(char c, int count)
{
    while(count-->0)
        printf("%c", c);
    printf("\n");
}

// calculate the CRC on a register (MSG+CRC <= 32 bits)
long calc_crc(uint32_t reg, uint32_t poly, uint32_t crc, bool print)
{
    // sizes must be >= 1 and total <=32
    assert(CRC_W>=1 && MESSAGE_W>=1 && CRC_W+MESSAGE_W<=32);
    // the LSB of the poly MUST be a 1
    assert(poly&1UL);
    // the MSB of the poly MUST be a 1
    assert(poly&(1UL<<CRC_W));

    // add zeros on the right
    reg <<= CRC_W;

    // initiate the CRC bits
    reg |= crc;

    // sync the LEFT of the poly with the LEFT of the message
    poly <<= MESSAGE_W-1;

    uint32_t mask = 1UL << (MESSAGE_W+CRC_W-1);

    if_printf("Reg:  ");
    if_print_bin(reg, 0, MESSAGE_W+CRC_W-1, MESSAGE_W+CRC_W, true);

    if_print_line('-',MESSAGE_W+CRC_W+18);

    for(int i=MESSAGE_W+CRC_W-1, spaces=0; i>=CRC_W; i--, spaces++)
    {
        if_printf("Reg:  ");
        if_print_bin(reg, spaces, i, CRC_W+1, true); 

 //     if_printf("Mask: ");
 //     if_print_bin(mask, spaces, i, 1);

        if(reg&mask)
        {
            if_printf("Poly: ");
            if_print_bin(poly, spaces, i, CRC_W+1, false); 
            reg ^= poly;
        }
        else
        {
            if_printf("--->: ");
            if_print_bin(0, spaces, i, CRC_W+1, false); 
        }

        poly>>=1UL;
        mask>>=1UL;
    }

    return reg;
}

int main()
{
    printf("\n********\n\n");

    uint32_t reg = MESSAGE;
    uint32_t poly = POLY;
    uint32_t crc = calc_crc(reg, poly, 0, true);
    printf("Tx side: CRC = 0x%02X (%02u)\n", crc, crc);

    // add noise
    // reg |= 0b00100000000000UL;
    
    crc = calc_crc(reg, poly, crc, false);
    printf("Rx side: CRC = 0x%02X (%02u)\n", crc, crc);
    printf("The message is %s\n", crc?"NOT VALID":"VALID");
    return 0;
}