#include<iostream>
#include<Windows.h>
#include<string>
#include<queue>
#include<fstream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")//����Ŀ�
class CHttp
{
private:
	string myhost;
	string myobject;
	SOCKET mysocket;
	bool htmlSaveVersion=true;
	bool AnalyseUrl(string url);//����URL http
	bool AnalyseUrl2(string url);//����URL https
	bool init();	//��ʼ���׽���
	bool Connect();		//����web������
public:
	CHttp();
	~CHttp();
	string FetchGet(string url);	//ͨ��Get��ʽ��ȡ��ҳ
	void AnalyseHtml(string html);	//������ҳ�����ͼƬ��ַ����������
};

