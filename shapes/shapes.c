/**
 * @file shapes.c
 * @author Boris Snäll (borissnaell@gmail.com)
 * @brief Answers to challenge "shapes.txt"
            - Understanding of patterns
            - Understanding of boundaries
            - Understanding of % operator
            - Thinking the best "if" (or "ifs")
 * @version 0.1
 * @date 2021-11-02
 */
#include <stdio.h>
#include <stdbool.h>

#define MAX(a,b)        ((a)>(b)?(a):(b))
#define FITIN(a,size)   (((a)<0?(a)+(size):(a))%(size)) // to use with offsets

void square(int size)
{
    printf("square(%d)\n", size);

    for(int y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_top = y==0;
            bool is_bottom = y==size-1;
            bool is_left = x==0;
            bool is_right = x==size-1;
            bool is_x = is_left || is_top || is_right || is_bottom;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void diagonal_v1(int size)
{
    printf("diagonal_v1(%d)\n", size);

    for(int y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_x = x==y;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}


void diagonal_v2(int size)
{
    printf("diagonal_v2(%d)\n", size);

    for(int y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_x = x==size-y-1;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void diagonal_v3(int size)
{
    printf("diagonal_v3(%d)\n", size);

    for(int y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_x = x<=y;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void n_shape(int size)
{
    printf("n_shape(%d)\n", size);

    int left = 0;
    int right = size-1;

    for(int y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            int center = y;
            bool is_x = x==left || x==center || x==right;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void cross_v1(int size)
{
    printf("cross_v1(%d)\n", size);

    for(int y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_leg1 = x==y;
            bool is_leg2 = x==size-y-1;
            bool is_x = is_leg1 || is_leg2;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void cross_v2(int size, int rep)
{
    printf("cross_v2(%d,%d)\n", size, rep);

    if(rep>0)
    for(int y=0; y<size; y++)
    {
        for(int x=0; x<size*rep; x++)
        {
            bool is_leg1 = x%size==y;
            bool is_leg2 = x%size==size-y-1;
            bool is_x = is_leg1 || is_leg2;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void cross_v3(int size, int offX)
{
    printf("cross_v3(%d,%d)\n", size, offX);

    offX %= size;

    for(int y=0; y<size; y++)
    {
        for(int x=-offX; x<size-offX; x++)
        {
            bool is_leg1 = FITIN(x,size)==y;
            bool is_leg2 = FITIN(x,size)==size-y-1;
            bool is_x = is_leg1 || is_leg2;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void cross_v4(int size, int offY)
{
    printf("cross_v4(%d,%d)\n", size, offY);

    offY %= size;

    for(int y=-offY; y<size-offY; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_leg1 = x==FITIN(y,size);
            bool is_leg2 = x==size-FITIN(y,size)-1;
            bool is_x = is_leg1 || is_leg2;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void cross_v5(int size, int off)
{
    printf("cross_v5(%d,%d)\n", size, off);

    off %= size;

    for(int y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_leg1=false;
            bool is_leg2=false;

            // first half
            if(FITIN(y,size)<=size/2)
            {
                is_leg1 |= FITIN(x-off,size)==y;
                is_leg2 |= FITIN(x-off,size)==size-y-1;
            }
            // second half
            if(FITIN(y,size)>=size/2)
            {
                is_leg1 |= FITIN(x+off,size)==y;
                is_leg2 |= FITIN(x+off,size)==size-y-1;
            }
            bool is_x = is_leg1 || is_leg2;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void diagonals_v1(int sizeY, int rep)
{
    printf("diagonals_v1(%d,%d)\n", sizeY, rep);

    int sizeX = sizeY*rep;

    if(rep>0)
    for(int y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            bool is_x = x%sizeY==y;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void diagonals_v2(int sizeY, int rep, int distance)
{
    printf("diagonals_v2(%d,%d,%d)\n", sizeY, rep, distance);

    int sizeX = distance*(rep-1)+sizeY;

    if(rep>0 && distance>0)
    for(int y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            bool is_x = distance && x%distance==y;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void v_shape(int sizeY)
{
    printf("v_shape(%d)\n", sizeY);

    int sizeX = sizeY*2-1;

    for(int y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            bool is_left = x==y;
            bool is_right = x==sizeX-y-1;
            bool is_x = is_left || is_right;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void zig_zag(int sizeY)
{
    printf("zig_zag(%d)\n", sizeY);

    int sizeX = sizeY*3 - 2;

    for(int y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            bool is_left = x==y;
            bool is_right = x==(sizeY-1)*2+y;
            bool is_center = x==(sizeY-1)*2-y;
            bool is_x = is_left || is_center || is_right;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void w_shape(int sizeY)
{
    printf("w_shape(%d)\n", sizeY);

    int rep = 2; // two Vs
    int sizeX = MAX(sizeY*2-2, 1);
    int sizeW = sizeX*rep+1;

    for(int y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeW; x++)
        {
            bool is_left = x%sizeX==y;
            bool is_right = x%sizeX==sizeX-y;
            bool is_x = is_left || is_right;
            printf("%c", is_x ? 'x' : '.');
        }

        printf("\n");
    }

    printf("\n");
}

void w_shape_fancy(int sizeY)
{
    printf("w_shape_fancy(%d)\n", sizeY);

    int rep = 2; // two Vs
    int sizeX = MAX(sizeY*2-2, 1);
    int sizeW = sizeX*rep+1;

    for(int y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeW; x++)
        {
            char c = '.';
            bool is_left = x%sizeX==y;
            bool is_right = x%sizeX==sizeX-y;
            bool is_x = is_left || is_right;
            if(is_left && is_right) c = 'V';
            else if(y==0 && x==sizeX) c = 'A';
            else if(y==0 && x==sizeW-1) c = '/';
            else if(is_left) c = '\\';
            else if(is_right) c = '/';
            printf("%c", c);
        }

        printf("\n");
    }

    printf("\n");
}

void zig_zag_pro(int sizeY, int seg)
{
    printf("zig_zag_pro(%d,%d)\n", sizeY, seg);

    int sizeV = MAX(sizeY*2-2,1);
    int sizeX = sizeV*seg/2+1;
  //int sizeX = (sizeY-1)*seg+1;
    int nt = -2, nb = -1, dig = 0;

    if(seg>0)
    for(int y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            char c = '.';
            bool is_top = y==0;
            bool is_bottom = y==sizeY-1;
            bool is_left = x%sizeV == y;
            bool is_right = x%sizeV == sizeV-y;
            bool is_x = is_left || is_right;
            bool is_n = is_x && (is_top || is_bottom);

            if(is_n)
            {
                if(is_top)      dig = printf("%d", nt+=2);
                if(is_bottom)  {if(x==sizeX-1 && sizeY==1 && seg%2==0 && is_top)
                                dig = printf(""); // special case with sizeY==1 and last seg // zig_zag_pro(1,6)
                                else
                                dig = printf("%d", nb+=2);}
                if(x==sizeX-1)  dig = 0; // to generate a new line
            }
            else if(is_x)
            {
                if(is_left) c = '\\';
                else c = '/';
            }

            if(!is_n)
            {
                if(dig<2) printf("%c", c);
                if(dig)   dig--;//dig = MAX(0,dig-1);
            }
        }

        printf("\n");
    }

    printf("\n");
}

void separator()
{
    printf("*************************************************\n\n");
}

int main()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n");

    separator();

    square(5);
    square(3);
    // test 0 1 15
    //square(0);
    //square(1);
    //square(15);

    separator();
    diagonal_v1(5);
    diagonal_v1(3);
    // test 0 1 15
    //diagonal_v1(0);
    //diagonal_v1(1);
    //diagonal_v1(15);

    separator();
    diagonal_v2(5);
    diagonal_v2(3);
    // test 0 1 15
    //diagonal_v2(0);
    //diagonal_v2(1);
    //diagonal_v2(15);

    separator();
    diagonal_v3(5);
    diagonal_v3(3);
    // test 0 1 15
    //diagonal_v3(0);
    //diagonal_v3(1);
    //diagonal_v3(15);

    separator();
    n_shape(5);
    n_shape(4);
    n_shape(6);
    // test 0 1 15
    //n_shape(0);
    //n_shape(1);
    //n_shape(15);

    separator();
    cross_v1(6);
    cross_v1(5);
    cross_v1(3);
    // test 0 1 15
    //cross_v1(0);
    //cross_v1(1);
    //cross_v1(15);

    separator();
    cross_v2(5,3);
    cross_v2(6,4);
    // test 0 1 15
    //cross_v2(0,3);
    //cross_v2(6,0);
    //cross_v2(1,3);
    //cross_v2(6,1);
    //cross_v2(15,3);

    separator();
    diagonals_v1(5,3);
    diagonals_v1(3,5);
    // test 0 1 15
    //diagonals_v1(0,5);
    //diagonals_v1(3,0);
    //diagonals_v1(1,5);
    //diagonals_v1(3,1);
    //diagonals_v1(15,5);
    //diagonals_v1(3,15);

    separator();
    diagonals_v2(5,3,7);
    diagonals_v2(3,5,6);
    // test 0 1
    //diagonals_v2(0,5,6);
    //diagonals_v2(3,0,6);
    //diagonals_v2(3,5,0);
    //diagonals_v2(1,5,6);
    //diagonals_v2(3,1,6);
    //diagonals_v2(3,5,1);

    separator();
    cross_v3(9,2);
    cross_v3(12,3);
    // test 0 1 15
    //cross_v3(0,2);
    //cross_v3(9,0);
    //cross_v3(1,2);
    //cross_v3(9,1);
    //cross_v3(15,3);

    separator();
    cross_v4(9,2);
    cross_v4(12,3);
    // test 0 1 15
    //cross_v4(0,2);
    //cross_v4(9,0);
    //cross_v4(1,2);
    //cross_v4(9,1);
    //cross_v4(15,3);

    separator();
    v_shape(5);
    v_shape(4);
    // test 0 1
    //v_shape(0);
    //v_shape(1);

    separator();
    zig_zag(3);
    zig_zag(4);
    zig_zag(5);
    // test 0 1 15
    //zig_zag(0);
    //zig_zag(1);
    //zig_zag(15);

    separator();
    w_shape(5);
    w_shape(4);
    w_shape(3);
    // test 0 1 15
    //w_shape(0);
    //w_shape(1);
    //w_shape(15);

    separator();
    cross_v5(7,0);
    cross_v5(7,1);
    cross_v5(7,2);
    cross_v5(7,3);
    cross_v5(8,1);
    cross_v5(8,2);
    // test 0 1 15
    //cross_v5(0,2);
    //cross_v5(9,0);
    //cross_v5(1,2);
    //cross_v5(9,1);
    //cross_v5(15,3);

    separator();
    w_shape_fancy(5);
    w_shape_fancy(4);
    w_shape_fancy(3);
    // test 0 1 15
    //w_shape_fancy(0);
    //w_shape_fancy(1);
    //w_shape_fancy(15);

    separator();
    zig_zag_pro(5,3);
    zig_zag_pro(4,6);
    zig_zag_pro(2,1);
    zig_zag_pro(1,5);
    zig_zag_pro(1,6);
    zig_zag_pro(1,13);
    zig_zag_pro(4,10);
    /*zig_zag_pro(4,13);
    zig_zag_pro(5,13);
     // test 0 1
    zig_zag_pro(0,3);
    zig_zag_pro(4,0);
    zig_zag_pro(1,3);
    zig_zag_pro(4,1);*/

    printf("\n"); 
    return 0;
}