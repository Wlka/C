#include <iostream>
#include <cstring>

using namespace std;

bool result_match(const char* key,const char* str)
{
	int i=0,j=0,k=0;	//kΪƥ�䵽���ַ�����
	bool flag=false;
	int len_key=strlen(key);	//strlen������cstringͷ�ļ���
	int len_str=strlen(str);
	while(i<len_str&&j<len_key)
	{
		//ƥ�䵽һ����ͬ���ַ��ͼ������ƥ��
		if(str[i]==key[j])
		{
			i++; j++; k++;	//��ƥ�䵽��ͬ�ַ�ʱ��i��j��kͬ������
			if(k==len_key)
			{
				flag=true;
				break;
			}
		}
		//�ص���������
		else
		{
			j-=k;	//����j��kͬ��������������j����
			i=i-k+1;	//��i��ȥ��ƥ�䵽���ַ��������1���Ӻ������ƥ��
			k=0;	//k���㣬�ﵽj��kͬ����Ŀ��
		}
	}
	return flag;
}

int main()
{
	cout<<result_match("tegr","testafgr")<<endl;
	return 0;
}

