/**
 * @file shapes.c
 * @author Boris Snäll (borissnaell@gmail.com)
 * @brief Answers to challenge "shapes_array.txt"
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

void print_arr2D(char *a, int sx, int sy)
{
    printf("[%d,%d]\n", sy, sx);

    for(int y=0; y<sy; y++)
    {
        for(int x=0; x<sx; x++)
            printf("%c", *a++);
        printf("\n");
    }

    printf("\n");
}

void square(char *a, int size)
{
    printf("square(%d)\n", size);

    for(int i=0, y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_top = y==0;
            bool is_bottom = y==size-1;
            bool is_left = x==0;
            bool is_right = x==size-1;
            bool is_x = is_left || is_top || is_right || is_bottom;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, size, size);
}

void diagonal_v1(char *a, int size)
{
    printf("diagonal_v1(%d)\n", size);

    for(int i=0, y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_x = x==y;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, size, size);
}


void diagonal_v2(char *a, int size)
{
    printf("diagonal_v2(%d)\n", size);

    for(int i=0, y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_x = x==size-y-1;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, size, size);
}

void diagonal_v3(char *a, int size)
{
    printf("diagonal_v3(%d)\n", size);

    for(int i=0, y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_x = x<=y;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, size, size);
}

void n_shape(char *a, int size)
{
    printf("n_shape(%d)\n", size);

    int left = 0;
    int right = size-1;

    for(int i=0, y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            int center = y;
            bool is_x = x==left || x==center || x==right;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, size, size);
}

void cross_v1(char *a, int size)
{
    printf("cross_v1(%d)\n", size);

    for(int i=0, y=0; y<size; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_leg1 = x==y;
            bool is_leg2 = x==size-y-1;
            bool is_x = is_leg1 || is_leg2;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, size, size);
}

void cross_v2(char *a, int sizeY, int rep)
{
    printf("cross_v2(%d,%d)\n", sizeY, rep);

    int sizeX = sizeY*rep;

    if(rep>0)
    for(int i=0, y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            bool is_leg1 = x%sizeY==y;
            bool is_leg2 = x%sizeY==sizeY-y-1;
            bool is_x = is_leg1 || is_leg2;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, sizeX, sizeY);
}

void cross_v3(char *a, int size, int offX)
{
    printf("cross_v3(%d,%d)\n", size, offX);

    offX %= size;

    for(int i=0, y=0; y<size; y++)
    {
        for(int x=-offX; x<size-offX; x++)
        {
            bool is_leg1 = FITIN(x,size)==y;
            bool is_leg2 = FITIN(x,size)==size-y-1;
            bool is_x = is_leg1 || is_leg2;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, size, size);
}

void cross_v4(char *a, int size, int offY)
{
    printf("cross_v4(%d,%d)\n", size, offY);

    offY %= size;

    for(int i=0, y=-offY; y<size-offY; y++)
    {
        for(int x=0; x<size; x++)
        {
            bool is_leg1 = x==FITIN(y,size);
            bool is_leg2 = x==size-FITIN(y,size)-1;
            bool is_x = is_leg1 || is_leg2;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, size, size);
}

void cross_v5(char *a, int size, int off)
{
    printf("cross_v5(%d,%d)\n", size, off);

    off %= size;

    for(int i=0, y=0; y<size; y++)
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
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, size, size);
}

void diagonals_v1(char *a, int sizeY, int rep)
{
    printf("diagonals_v1(%d,%d)\n", sizeY, rep);

    int sizeX = sizeY*rep;

    if(rep>0)
    for(int i=0, y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            bool is_x = x%sizeY==y;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, sizeX, sizeY);
}

void diagonals_v2(char *a, int sizeY, int rep, int distance)
{
    printf("diagonals_v2(%d,%d,%d)\n", sizeY, rep, distance);

    int sizeX = distance*(rep-1)+sizeY;

    if(rep>0 && distance>0)
    for(int i=0, y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            bool is_x = distance && x%distance==y;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, sizeX, sizeY);
}

void v_shape(char *a, int sizeY)
{
    printf("v_shape(%d)\n", sizeY);

    int sizeX = sizeY*2-1;

    for(int i=0, y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            bool is_left = x==y;
            bool is_right = x==sizeX-y-1;
            bool is_x = is_left || is_right;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, sizeX, sizeY);
}

void zig_zag(char *a, int sizeY)
{
    printf("zig_zag(%d)\n", sizeY);

    int sizeX = sizeY*3 - 2;

    for(int i=0, y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            bool is_left = x==y;
            bool is_right = x==(sizeY-1)*2+y;
            bool is_center = x==(sizeY-1)*2-y;
            bool is_x = is_left || is_center || is_right;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, sizeX, sizeY);
}

void w_shape(char *a, int sizeY)
{
    printf("w_shape(%d)\n", sizeY);

    int rep = 2; // two Vs
    int sizeX = MAX(sizeY*2-2, 1);
    int sizeW = sizeX*rep+1;

    for(int i=0, y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeW; x++)
        {
            bool is_left = x%sizeX==y;
            bool is_right = x%sizeX==sizeX-y;
            bool is_x = is_left || is_right;
            a[i++] =  is_x ? 'x' : '.';
        }
    }

    print_arr2D(a, sizeW, sizeY);
}

void w_shape_fancy(char *a, int sizeY)
{
    printf("w_shape_fancy(%d)\n", sizeY);

    int rep = 2; // two Vs
    int sizeX = MAX(sizeY*2-2, 1);
    int sizeW = sizeX*rep+1;

    for(int i=0, y=0; y<sizeY; y++)
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
            a[i++] = c;
        }
    }

    print_arr2D(a, sizeW, sizeY);
}

void zig_zag_pro(char *a, int sizeY, int seg)
{
    printf("zig_zag_pro(%d,%d)\n", sizeY, seg);
    // BUG: (1,5) or (1,6) // sizeY=1
    int sizeV = MAX(sizeY*2-2,1);
    int sizeX = sizeV*seg/2+1;
  //int sizeX = (sizeY-1)*seg+1;
    int nt = 0, nb = 1, dig = 0;

    if(seg>0)
    for(int i=0, y=0; y<sizeY; y++)
    {
        for(int x=0; x<sizeX; x++)
        {
            int i2 = i;
            bool is_top = y==0;
            bool is_bottom = y==sizeY-1;
            bool is_left = x%sizeV == y;
            bool is_right = x%sizeV == sizeV-y;
            bool is_x = is_left || is_right;
            bool is_n = is_x && (is_top || is_bottom);

            if (is_n)
            {
                if (is_top)
                {
                    a[i++] = '0' + nt % 10;
                    nt += 2;
                }
                if (is_bottom)
                {
                    a[i++] = '0' + nb % 10;
                    nb += 2;
                }
            }
            else if(is_x)
            {
                if(is_left) a[i++] = '\\';
                else        a[i++] = '/';
            }
            else
            {
                a[i++] = '.';
            }
        }
    }
    print_arr2D(a, sizeX, sizeY);
}

void separator()
{
    printf("*************************************************\n\n");
}

int main()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n");

    char arr[50][50];
    char *ptr = &arr[0][0];
    separator();

    square(ptr,5);
    square(ptr,3);
    // test 0 1 15
    //square(ptr,0);
    //square(ptr,1);
    //square(ptr,15);

    separator();
    diagonal_v1(ptr,5);
    diagonal_v1(ptr,3);
    // test 0 1 15
    //diagonal_v1(ptr,0);
    //diagonal_v1(ptr,1);
    //diagonal_v1(ptr,15);

    separator();
    diagonal_v2(ptr,5);
    diagonal_v2(ptr,3);
    // test 0 1 15
    //diagonal_v2(ptr,0);
    //diagonal_v2(ptr,1);
    //diagonal_v2(ptr,15);

    separator();
    diagonal_v3(ptr,5);
    diagonal_v3(ptr,3);
    // test 0 1 15
    //diagonal_v3(ptr,0);
    //diagonal_v3(ptr,1);
    //diagonal_v3(ptr,15);

    separator();
    n_shape(ptr,5);
    n_shape(ptr,4);
    n_shape(ptr,6);
    // test 0 1 15
    //n_shape(ptr,0);
    //n_shape(ptr,1);
    //n_shape(ptr,15);

    separator();
    cross_v1(ptr,6);
    cross_v1(ptr,5);
    cross_v1(ptr,3);
    // test 0 1 15
    //cross_v1(ptr,0);
    //cross_v1(ptr,1);
    //cross_v1(ptr,15);

    separator();
    cross_v2(ptr,5,3);
    cross_v2(ptr,6,4);
    // test 0 1 15
    //cross_v2(ptr,0,3);
    //cross_v2(ptr,6,0);
    //cross_v2(ptr,1,3);
    //cross_v2(ptr,6,1);
    //cross_v2(ptr,15,3);

    separator();
    diagonals_v1(ptr,5,3);
    diagonals_v1(ptr,3,5);
    // test 0 1 15
    //diagonals_v1(ptr,0,5);
    //diagonals_v1(ptr,3,0);
    //diagonals_v1(ptr,1,5);
    //diagonals_v1(ptr,3,1);
    //diagonals_v1(ptr,15,5);
    //diagonals_v1(ptr,3,15);

    separator();
    diagonals_v2(ptr,5,3,7);
    diagonals_v2(ptr,3,5,6);
    // test 0 1
    //diagonals_v2(ptr,0,5,6);
    //diagonals_v2(ptr,3,0,6);
    //diagonals_v2(ptr,3,5,0);
    //diagonals_v2(ptr,1,5,6);
    //diagonals_v2(ptr,3,1,6);
    //diagonals_v2(ptr,3,5,1);

    separator();
    cross_v3(ptr,9,2);
    cross_v3(ptr,12,3);
    // test 0 1 15
    //cross_v3(ptr,0,2);
    //cross_v3(ptr,9,0);
    //cross_v3(ptr,1,2);
    //cross_v3(ptr,9,1);
    //cross_v3(ptr,15,3);

    separator();
    cross_v4(ptr,9,2);
    cross_v4(ptr,12,3);
    // test 0 1 15
    //cross_v4(ptr,0,2);
    //cross_v4(ptr,9,0);
    //cross_v4(ptr,1,2);
    //cross_v4(ptr,9,1);
    //cross_v4(ptr,15,3);

    separator();
    v_shape(ptr,5);
    v_shape(ptr,4);
    // test 0 1
    //v_shape(ptr,0);
    //v_shape(ptr,1);

    separator();
    zig_zag(ptr,3);
    zig_zag(ptr,4);
    zig_zag(ptr,5);
    // test 0 1 15
    //zig_zag(ptr,0);
    //zig_zag(ptr,1);
    //zig_zag(ptr,15);

    separator();
    w_shape(ptr,5);
    w_shape(ptr,4);
    w_shape(ptr,3);
    // test 0 1 15
    //w_shape(ptr,0);
    //w_shape(ptr,1);
    //w_shape(ptr,15);

    separator();
    cross_v5(ptr,7,0);
    cross_v5(ptr,7,1);
    cross_v5(ptr,7,2);
    cross_v5(ptr,7,3);
    cross_v5(ptr,8,1);
    cross_v5(ptr,8,2);
    // test 0 1 15
    //cross_v5(ptr,0,2);
    //cross_v5(ptr,9,0);
    //cross_v5(ptr,1,2);
    //cross_v5(ptr,9,1);
    //cross_v5(ptr,15,3);

    separator();
    w_shape_fancy(ptr,5);
    w_shape_fancy(ptr,4);
    w_shape_fancy(ptr,3);
    // test 0 1 15
    //w_shape_fancy(ptr,0);
    //w_shape_fancy(ptr,1);
    //w_shape_fancy(ptr,15);

    separator();
    zig_zag_pro(ptr,5,3);
    zig_zag_pro(ptr,4,6);
    zig_zag_pro(ptr,2,1);
    zig_zag_pro(ptr,1,5);
    zig_zag_pro(ptr,1,6);
    zig_zag_pro(ptr,1,13);
    zig_zag_pro(ptr,4,10);
    //zig_zag_pro(ptr,4,13);
    zig_zag_pro(ptr,5,13);
    // test 0 1
    //zig_zag_pro(ptr,0,3);
    //zig_zag_pro(ptr,4,0);
    //zig_zag_pro(ptr,1,3);
    //zig_zag_pro(ptr,4,1);
    printf("\n"); 
    return 0;
}