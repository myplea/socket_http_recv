#include"CHttp.h"
#include<urlmon.h>

#pragma warning(disable:4996)
#pragma comment(lib,"urlmon.lib")

queue<string> q;//url����
queue<string> p;//ͼƬurl����

void StartCatch(string url);
int main()
{
	
	//����һ���ļ��У����ʾ��ǰĿ¼
	CreateDirectory("./image", NULL);

	//�Ӽ�������һ����ʼurl
	string url;
	//cout << "������url��";
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
		//ȡ��url
		string currenturl = q.front();
		q.pop();

		CHttp http;
		//����һ��Get����
		string html = http.FetchGet(currenturl);
		//cout << html;
		cout << endl;
		http.AnalyseHtml(html);
	}
}

//����ͼƬ���߳�
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
			if (num == 100)	//����1000�žͽ����������û����ô��hh
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