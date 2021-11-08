#include <stdio.h>
#include <ctype.h>

int count_chars(const char *str)
{
    int i=0;
    for(i=0; str[i]; i++) {;}
    return i;
}

int find_first_char(const char *str, char find)
{
    if(str)
    for(int i=0; str[i]; i++)
        if(str[i]==find)
            return i;

    return -1;
}

int find_last_char(const char *str, char find)
{
    if(str)
    for(int i=count_chars(str)-1; i>=0; i--)
        if(str[i]==find)
            return i;

    return -1;
}

void change_char(char *str, char from, char to)
{
    if(str)
    for(int i=0; str[i]; i++)
        if(str[i]==from)
            str[i] = to;
}

unsigned string_copy(char *dest, const char *source)
{
    int i=-1;

    if(dest && source)
    {
        for(i=0; source[i]; i++)
            dest[i]=source[i];

        dest[i]=0;
    }

    return i;
}

void only_alpha(char *str)
{
    if(str)
    {
        int j=0;
        for(int i=0; str[i]; i++)
            if(isalpha(str[i]))
                str[j++] = str[i];
        str[j]=0;
    }
}


void only_numbers(char *str)
{
    if(str)
    {
        int j=0;
        for(int i=0; str[i]; i++)
            if(isdigit(str[i]))
                str[j++] = str[i];
        str[j]=0;
    }
}

void reverse_string(char *str)
{
    int r = count_chars(str);
    for(int l=0; l<r; l++, r--)
    {
        char c = str[l];
        str[l] = str[r-1];
        str[r-1] = c;
    }
}

void shift_left(char *str)
{
    if(str)
    {
        int i=0;
        char c=str[0];
        for(i=0; str[i]; i++)
            str[i]=str[i+1];
        str[i-1] = c;
    }
}

void shift_left_n(char *str, unsigned amount)
{
    int len = count_chars(str);
    char cpy[len];
    string_copy(cpy, str);

    for(int i=0; i<len; i++)
        str[i] = str[(i+amount)%len];
}

void string_trim_left(char *str)
{
    if(str)
    {
        int i, shift = 0;
        for(i=0; str[i]; i++)
            if(isspace(str[i]))
                shift++;
            else break;

        for(i=0; str[i+shift]; i++)
            str[i]=str[i+shift];

        str[i] = 0;
    }
}

void string_trim_right(char *str)
{
    if(str)
    {
        int len = count_chars(str);
        for(int i=len-1; i>=0; i--)
            if(isspace(str[i]))
                str[i] = 0;
            else break;
    }
}

void string_trim(char *str)
{
    string_trim_left(str);
    string_trim_right(str);
}

void string_trim_all(char *str)
{
    if(str)
    {
        string_trim(str);

        int j=0, spc=0;
        for(int i=0; str[i]; i++)
        {
            if(isspace(str[i]))
            {
                if(!spc)
                {
                    str[j++] = str[i];
                    spc = 1;
                }
            }
            else
            {
                str[j++] = str[i];
                spc = 0;
            }
        }
        str[j] = 0;
    }
}

void capitalize(char *str)
{
    string_trim_all(str);

    int first = 1;
    for(int i=0; str[i]; i++)
    {
        if(first)
        {
            str[i] = toupper(str[i]);
            first = 0;
        }
        else
        {
            if(isspace(str[i]))
                first = 1;

            str[i] = tolower(str[i]);
        }
    }
}

unsigned int string_to_uint(const char *str)
{
    unsigned int r = 0;

    if(str)
    {
        int i=0;
        for(i=0; str[i]; i++)
            if(!isspace(str[i]))
                break;

        for(; str[i]; i++)
        {
            if(isdigit(str[i]))
                r = r*10 + str[i]-'0';
            else break;
        }
    }

    return r;
}

int string_to_int(const char *str)
{
    int r = 0, plus=0, minus=0;

    if(str)
    {
        int i=0;
        for(i=0; str[i]; i++)
            if(!isspace(str[i]))
                break;

        if(str[i]=='-')
            minus=1;
        else
        if(str[i]=='+')
            plus=1;

        for(i+=minus+plus; str[i]; i++)
        {
            if(isdigit(str[i]))
                r = r*10 + str[i]-'0';
            else break;
        }

        if(minus)
            r = -r;
    }

    return r;
}

double string_to_double(const char *str)
{
    double r = 0., div=10.;
    int plus=0, minus=0, point=0;

    if(str)
    {
        int i=0;
        for(i=0; str[i]; i++)
            if(!isspace(str[i]))
                break;

        if(str[i]=='-')
            minus=1;
        else
        if(str[i]=='+')
            plus=1;

        for(i+=minus+plus; str[i]; i++)
        {
            if(isdigit(str[i]))
            {
                if(point)
                {
                    r += (str[i]-'0')/div;
                    div *= 10;
                }
                else
                {
                    r = r*10 + str[i]-'0';
                }
            }
            else 
            if(str[i]=='.')
                point = 1;
            else
                break;
        }

        if(minus)
            r = -r;
    }

    return r;
}


int main()
{
    char str[100] = "Hello World!";

    printf("count_chars(\"%s\") = %d\n", str, count_chars(str));
    
    printf("find_first_char(\"%s\", '%c') = %d\n", str, 'W', find_first_char(str, 'W'));
    
    printf("find_last_char(\"%s\", '%c') = %d\n", str, 'o', find_last_char(str, 'o'));
    
    printf("change_char(\"%s\", '%c', '%c') = ", str, '!', '?');
    change_char(str, '!', '?');
    printf("\"%s\"\n", str);

    char copy[100] = "";
    printf("string_copy(\"%s\", \"%s\") = ", "", str);
    int copy_n = string_copy(copy, str);
    printf("%d -> \"%s\"\n", copy_n, copy);

    char oa[] = "123AbCdE45.;#€!XYZ0";
    printf("only_alpha(\"%s\") = ", oa);
    only_alpha(oa);
    printf("\"%s\"\n", oa);

    char on[] = "123AbCdE45.;#€!XYZ0";
    printf("only_numbers(\"%s\") = ", on);
    only_numbers(on);
    printf("\"%s\"\n", on);

    char rev[] = "abcd_.1234";
    printf("reverse_string(\"%s\") = ", rev);
    reverse_string(rev);
    printf("\"%s\"\n", rev);

    char shl[] = "abcd_1234";
    printf("shift_left(\"%s\") = ", shl);
    shift_left(shl);
    printf("\"%s\"\n", shl);

    char shln[] = "abcd_1234";
    printf("shift_left_n(\"%s\", %d) = ", shln, 3);
    shift_left_n(shln, 3);
    printf("\"%s\"\n", shln);

    char triml[] = "  ABC  D  E  F  XYZ  ";
    printf("string_trim_left(\"%s\") = ", triml);
    string_trim_left(triml);
    printf("\"%s\"\n", triml);

    char trimr[] = "  ABC  D  E  F  XYZ  ";
    printf("string_trim_right(\"%s\") = ", trimr);
    string_trim_right(trimr);
    printf("\"%s\"\n", trimr);

    char trim[] = "  ABC  D  E  F  XYZ  ";
    printf("string_trim(\"%s\") = ", trim);
    string_trim(trim);
    printf("\"%s\"\n", trim);

    char trima[] = "  ABC  D  E  F  XYZ  ";
    printf("string_trim_all(\"%s\") = ", trima);
    string_trim_all(trima);
    printf("\"%s\"\n", trima);

    char cap[] = "  aBcD eee FFF 123   _x_ a./=b  ";
    printf("capitalize(\"%s\") = ", cap);
    capitalize(cap);
    printf("\"%s\"\n", cap);

    char *uint_str = " 123";
    printf("string_to_uint(\"%s\") = %u\n", uint_str, string_to_uint(uint_str));

    char *int_str = " -123";
    printf("string_to_int(\"%s\") = %d\n", int_str, string_to_int(int_str));

    char *double_str = " -00123.45009";
    printf("string_to_double(\"%s\") = %f\n", double_str, string_to_double(double_str));

    return 0;
}
