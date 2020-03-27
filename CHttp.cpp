#include "CHttp.h"


CHttp::CHttp()
{
}


CHttp::~CHttp()
{
	closesocket(mysocket);
	WSACleanup();
}

bool CHttp::AnalyseUrl(string url)
{
	if (string::npos == url.find("http://"))
		return false;
	if (url.length() <= 7)
		return false;
	int pos = url.find('/', 7);
	if (pos == string::npos)	//无文件名
	{
		myhost = url.substr(7);
		myobject = '/';
	}
	else
	{
		myhost = url.substr(7, pos - 7);
		myobject = url.substr(pos);
	}
	if (myhost.empty())
		return false;
	htmlSaveVersion = false;
	return true;
}

//解析URL \https
bool CHttp::AnalyseUrl2(string url)
{
	if (string::npos == url.find("https://"))
		return false;
	if (url.length() <= 8)
		return false;
	int pos = url.find('/', 8);
	if (pos == string::npos)
	{
		myhost = url.substr(8, pos - 8);
		myobject = '/';
	}
	else
	{
		myhost = url.substr(8, pos - 8);
		myobject = url.substr(pos);
	}
	if (myhost.empty())
		return false;
	htmlSaveVersion = true;
	return true;
}

bool CHttp::init()
{
	//1 请求版本协议
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		cout << "请求协议版本失败！\n";
		return false;
	}
	//cout << "请求协议成功！\n";
	//2 创建socket
	mysocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == mysocket)
	{
		cout << "创建socket失败！\n";
		WSACleanup();
		return false;
	}
	//cout<<"创建socket成功！\n";
	return true;
}

//连接web服务器
bool CHttp::Connect()
{
	//DNS服务器：将域名解析成IP地址
	hostent *p = gethostbyname(myhost.c_str());
	if (p == NULL)
		return false;
	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	if (htmlSaveVersion)
		sa.sin_port = htons(443);	//https的默认端口
	else
		sa.sin_port = htons(80);	//http的默认端口
	memcpy(&sa.sin_addr, p->h_addr, 4);

	if (-1 == connect(mysocket, (SOCKADDR*)&sa, sizeof(sa)))
	{
		cout << "服务器连接失败" << endl;
		return false;
	}
	else
	{
		cout << "服务器连接成功" << endl;
		return true;
	}
}

string CHttp::FetchGet(string url)	//通过Get方式获取网页
{
	string html;
	
	//解析url
	if (false == AnalyseUrl(url))
	{
		if (false == AnalyseUrl2(url))
		{
			cout << "Html解析失败" << endl;
			return "";
		}
	}
	cout << "主机名" << myhost << "\t\t" << "资源名" << myobject << endl;
	if (false == init())//初始化套接字
	{
		return "";
	}
	if (false == Connect())//连接服务器
	{
		return "";
	}
	
	//发送Get请求 Get请求数据
	string request = "GET " + myobject +
		" HTTP/1.1\r\nHost:" + myhost +
		"\r\nConnection: Close\r\n\r\n";

	if (SOCKET_ERROR == send(mysocket, request.c_str(), request.size(), 0))
	{
		cout << "send request error" << endl;
		closesocket(mysocket);
		return "";
	}

	//接收数据
	char ch;
	while (recv(mysocket, &ch, 1, 0))
	{
		html += ch;
	}
	//cout << html;
	ofstream out("buffer.txt", ios::out);
	out << html;
	out.close();
	return html;
}

//判断是否以什么结尾
bool hasEnding(char *& strFull, const char*& strEnd)
{
	char * pFull = strFull;
	while (*pFull != 0)
		pFull++;

	const char * pEnd = strEnd;
	while (*pEnd != 0)
		pEnd++;

	while (1)
	{
		pFull--;
		pEnd--;
		if(*pEnd==0)
		{
			break;
		}
		if (*pFull != *pEnd)
		{
			return false;
		}
	}
	return true;
}

void CHttp::AnalyseHtml(string html)//解析网页，获得图片地址和其他链接
{
	int startIndex = 0;
	int endIndex = 0;
	//找到所有的图片
	for (int pos = 0; pos < html.length();)
	{
		startIndex = html.find("src=\"", startIndex);
		if (startIndex == -1)
		{
			break;
		}
		startIndex += 5;
		endIndex = html.find("\"", startIndex);
		//找到资源链接
		string src = html.substr(startIndex, endIndex - startIndex);
		char *src1 = (char*)src.c_str();
		//判断连接是否是想要的资源
		const char *strend = ".jpg";
		if (hasEnding(src1, strend) == true)
		{
			//if (-1 != src.find("t_s1920x1080c5"))			//找到 "t_s1920x1080c5" ?
			//{
				cout << src << endl;
				//新建一个线程来下载图片
				extern queue<string> p;
				p.push(src);
			//}
			//system("pause");
		}
		
		startIndex = endIndex + 1;
		//system("pause");
	}
	extern void loadImage();
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)loadImage,
		NULL, NULL, NULL);
	startIndex = 0;
	
}