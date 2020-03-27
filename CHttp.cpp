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
	if (pos == string::npos)	//���ļ���
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

//����URL \https
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
	//1 ����汾Э��
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		cout << "����Э��汾ʧ�ܣ�\n";
		return false;
	}
	//cout << "����Э��ɹ���\n";
	//2 ����socket
	mysocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == mysocket)
	{
		cout << "����socketʧ�ܣ�\n";
		WSACleanup();
		return false;
	}
	//cout<<"����socket�ɹ���\n";
	return true;
}

//����web������
bool CHttp::Connect()
{
	//DNS��������������������IP��ַ
	hostent *p = gethostbyname(myhost.c_str());
	if (p == NULL)
		return false;
	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	if (htmlSaveVersion)
		sa.sin_port = htons(443);	//https��Ĭ�϶˿�
	else
		sa.sin_port = htons(80);	//http��Ĭ�϶˿�
	memcpy(&sa.sin_addr, p->h_addr, 4);

	if (-1 == connect(mysocket, (SOCKADDR*)&sa, sizeof(sa)))
	{
		cout << "����������ʧ��" << endl;
		return false;
	}
	else
	{
		cout << "���������ӳɹ�" << endl;
		return true;
	}
}

string CHttp::FetchGet(string url)	//ͨ��Get��ʽ��ȡ��ҳ
{
	string html;
	
	//����url
	if (false == AnalyseUrl(url))
	{
		if (false == AnalyseUrl2(url))
		{
			cout << "Html����ʧ��" << endl;
			return "";
		}
	}
	cout << "������" << myhost << "\t\t" << "��Դ��" << myobject << endl;
	if (false == init())//��ʼ���׽���
	{
		return "";
	}
	if (false == Connect())//���ӷ�����
	{
		return "";
	}
	
	//����Get���� Get��������
	string request = "GET " + myobject +
		" HTTP/1.1\r\nHost:" + myhost +
		"\r\nConnection: Close\r\n\r\n";

	if (SOCKET_ERROR == send(mysocket, request.c_str(), request.size(), 0))
	{
		cout << "send request error" << endl;
		closesocket(mysocket);
		return "";
	}

	//��������
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

//�ж��Ƿ���ʲô��β
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

void CHttp::AnalyseHtml(string html)//������ҳ�����ͼƬ��ַ����������
{
	int startIndex = 0;
	int endIndex = 0;
	//�ҵ����е�ͼƬ
	for (int pos = 0; pos < html.length();)
	{
		startIndex = html.find("src=\"", startIndex);
		if (startIndex == -1)
		{
			break;
		}
		startIndex += 5;
		endIndex = html.find("\"", startIndex);
		//�ҵ���Դ����
		string src = html.substr(startIndex, endIndex - startIndex);
		char *src1 = (char*)src.c_str();
		//�ж������Ƿ�����Ҫ����Դ
		const char *strend = ".jpg";
		if (hasEnding(src1, strend) == true)
		{
			//if (-1 != src.find("t_s1920x1080c5"))			//�ҵ� "t_s1920x1080c5" ?
			//{
				cout << src << endl;
				//�½�һ���߳�������ͼƬ
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