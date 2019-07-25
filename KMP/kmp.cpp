/****************************************************************************************************************
 * KMP字符串匹配算法
 * void prefix_table(const std::string& pattern,int prefix[]);  //生成前缀表
 * void move_prefix_table(int prefix[],int n);  //将前缀表整体后移一位，第一位置为-1，原来的前缀表最后一位被覆盖
 * void kmp_search(const std::string& text,const std::string& pattern); //KMP搜索算法
 ****************************************************************************************************************/

#include <iostream>
#include <string>

void prefix_table(const std::string& pattern,int prefix[])
{
    prefix[0]=0;    //只有一个字符时前缀后缀相同个数为0
    int len=0;  //前后缀相等的字符个数
    int i=1;
    while(i<pattern.size())
    {
        if(pattern[i]==pattern[len])
        {
            ++len;
            prefix[i]=len;
            ++i;
        }
        else
        {
            if(len>0) len=prefix[len-1];
            else
            {
                prefix[i]=len;
                ++i;
            }
        }
    }
}

void move_prefix_table(int prefix[],int n)
{
    for(int i=n-1;i>0;--i)  prefix[i]=prefix[i-1];
    prefix[0]=-1;
}

void kmp_search(const std::string& text,const std::string& pattern)
{
    int n=pattern.length();
    int m=text.length();
    int* prefix=new int(sizeof(int)*n);
    
    prefix_table(pattern,prefix);
    move_prefix_table(prefix,n);

    int i=0;
    int j=0;
    while(i<m)
    {
        if(j==n-1&&text[i]==pattern[j]) //当匹配到pattern的最后一位时执行输出操作，并将j设成前缀表对应位置，继续匹配
        {
            std::cout<<"Found pattern at "<<i-j<<std::endl;
            j=prefix[j];
        }
        if(text[i]==pattern[j])
        {
            ++i;
            ++j;
        }
        else
        {
            j=prefix[j];    //当匹配不上时，将j设置成前缀表对应位置再往后匹配
            if(j==-1)   //当j为-1时，表示pattern的第一位与当前对应的text位置不匹配，双方都往后移动一位
            {
                ++i;
                ++j;
            }
        }
    }

    delete prefix;
}

int main()
{
    std::string pattern="ABABCABAA";
    std::string text="ABABABCABAABABABABABABCABAA";
    kmp_search(text,pattern);
    return 0;
}