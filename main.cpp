#include"CHttp.h"
#include<urlmon.h>

#pragma warning(disable:4996)
#pragma comment(lib,"urlmon.lib")

queue<string> q;//url队列
queue<string> p;//图片url队列

void StartCatch(string url);
int main()
{
	
	//创建一个文件夹，点表示当前目录
	CreateDirectory("./image", NULL);

	//从键盘输入一个起始url
	string url;
	//cout << "请输入url：";
	//cin >> url;
	url = "http://desk.zol.com.cn/1920x1080/";
	//url = "http://www.gov.cn/";
	StartCatch(url);
	system("pause");
	return 0;
}

void StartCatch(string url)
{
	q.push(url);

	while (!q.empty())
	{
		//取出url
		string currenturl = q.front();
		q.pop();

		CHttp http;
		//发送一个Get请求
		string html = http.FetchGet(currenturl);
		//cout << html;
		cout << endl;
		http.AnalyseHtml(html);
	}
}

//下载图片的线程
static int num = 0;
void loadImage()
{
	while (!p.empty())
	{
		string currenturl = p.front();
		p.pop();
		char Name[20] = { 0 };
		num++;
		sprintf(Name, "./image/%d.jpg", num);
		if (S_OK == URLDownloadToFile(NULL, currenturl.c_str(), Name, 0, 0))
		{
			cout << "download ok" << endl;
			if (num == 100)	//下载1000张就结束（大概率没有那么多hh
			{
				exit(0);
			}
		}
		else
		{
			cout << "download error" << endl;
		}
	}
}