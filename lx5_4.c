//编写函数strend(s,t)。如果字符串t出现在字符串s的尾部，该函数返回1；否则返回0.
#include<stdio.h>
int strend(char *s,char *t)
{
    char *bs = s;
    char *bt = t;
    for ( ; *s; s++)
        ;
    for ( ; *t; t++)
        ;
    for ( ; *s == *t; s--,t--)
        if ( t == bt || s == bs)
            break;
    if ( *s == *t && t == bt && *s != '\0')
        return 1;
    else 
        return 0;
}
char *a = "aecdecabc";
char *b = "abc";
int main()
{
    int c = strend(a,b);
    printf("%d\n",c);
}