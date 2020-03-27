#include<iostream>
#include<Windows.h>
#include<string>
#include<queue>
#include<fstream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")//网络的库
class CHttp
{
private:
	string myhost;
	string myobject;
	SOCKET mysocket;
	bool htmlSaveVersion=true;
	bool AnalyseUrl(string url);//解析URL http
	bool AnalyseUrl2(string url);//解析URL https
	bool init();	//初始化套接字
	bool Connect();		//连接web服务器
public:
	CHttp();
	~CHttp();
	string FetchGet(string url);	//通过Get方式获取网页
	void AnalyseHtml(string html);	//解析网页，获得图片地址和其他链接
};

