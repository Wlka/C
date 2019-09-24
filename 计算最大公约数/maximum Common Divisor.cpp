#include <bits/stdc++.h>

using namespace std;

int getGreatestCommonDivisor_1(int a,int b)
{
    /**************************************************
     * 辗转相除法(欧几里得算法)
     * 对于两个正整数a和b(a>b)
     * 它们的最大公约数等于a除以b的余数c和b之间的最大公约数
    **************************************************/
    if(a<b)
        swap(a,b);
    if(a%b==0)
        return b;
    return getGreatestCommonDivisor_1(b,a%b);
}

int getGreatestCommonDivisor_2(int a,int b)
{
    /**************************************************
     * 更相减损术
     * 对于两个正整数a和b(a>b)
     * 它们的最大公约数等于a-b的差值c和b的最大公约数
    **************************************************/
    if(a==b)
        return a;
    if(a<b)
        swap(a,b);
    return getGreatestCommonDivisor_2(a-b,b);
}

int getGreatestCommonDivisor_3(int a,int b)
{
    /**************************************************
     * 对于较大的整数来说，取模操作运算性能会比较差
     * 对于相差悬殊的整数来说，更相减损术要进行的相减操作较多
     * 这里通过结合二者来进行最优求解
     * 当a和b均为偶数时，fun(a,b)=2*fun(a/2,b/2)=2*fun(a>>1,b>>1)
     * 当a为偶数，b为奇数时，fun(a,b)=fun(a/2,b)=fun(a>>1,b)
     * 当a为奇数，b为偶数时，fun(a,b)=fun(a,b/2)=fun(a,b>>1)
     * 当a和b均为奇数时，先利用更相减损术运算一次，fun(a,b)=fun(b,a-b)
     * 此时a-b必然是偶数，然后又可以进行移位运算
    **************************************************/
    if(a==b)
        return a;
    if((a&1)==0 && (b&1)==0)    //a和b均为偶数
        return getGreatestCommonDivisor_3(a>>1,b>>1)<<1;
    else if((a&1)==0 && (b&1)!=0)   //a为偶数,b为奇数
        return getGreatestCommonDivisor_3(a>>1,b);
    else if((a&1)!=0 && (b&1)==0)   //a为奇数,b为偶数
        return getGreatestCommonDivisor_3(a,b>>1);
    else
    {
        if(a<b)
            swap(a,b);
        return getGreatestCommonDivisor_3(b,a-b);
    }
}

int main()
{
    cout<<getGreatestCommonDivisor_1(2500,140)<<endl;
    cout<<getGreatestCommonDivisor_2(1000,170)<<endl;
    cout<<getGreatestCommonDivisor_2(152334,75774)<<endl;
    return 0;
}