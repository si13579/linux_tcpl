/*在求二进制补码时，表达式x&=(x-1)可以删除x中最右边值为1的二进制位。
请解释这样做的道理。用这一方法重写bitcount函数,以加快其执行速度*/
int bitcount(unsigned x)
{
    int b;
    for ( b = 0;x != 0; x &= x - 1)
        ++b;
    return b;
}