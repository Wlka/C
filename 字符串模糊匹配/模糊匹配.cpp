#include <iostream>
#include <cstring>

using namespace std;

bool result_match(const char* key,const char* str)
{
	int i=0,j=0,k=0;	//k为匹配到的字符数量
	bool flag=false;
	int len_key=strlen(key);	//strlen包含在cstring头文件里
	int len_str=strlen(str);
	while(i<len_str&&j<len_key)
	{
		//匹配到一个相同的字符就继续向后匹配
		if(str[i]==key[j])
		{
			i++; j++; k++;	//当匹配到相同字符时，i、j、k同步递增
			if(k==len_key)
			{
				flag=true;
				break;
			}
		}
		//回到最初的起点
		else
		{
			j-=k;	//由于j、k同步递增，这里让j归零
			i=i-k+1;	//让i减去已匹配到的字符数量后加1，从后面继续匹配
			k=0;	//k归零，达到j、k同步的目的
		}
	}
	return flag;
}

int main()
{
	cout<<result_match("tegr","testafgr")<<endl;
	return 0;
}

