#include<stdio.h>

#include <winsock2.h>//WSAStartup()需要的头文件
#include<Windows.h>
#pragma comment(lib, "ws2_32.lib")//WSAStartup()需要的动态库



#define NUM 1024
SOCKET clientSockert[NUM];
int count;


/*socket2已经丢弃inet_ntoa()、inet_addr()这些老版本函数
当在Socket2上调用这些老版本(Socket1版本)函数时，warning变成error了,故添加#pragma warning(disable:4996)
*/
#pragma warning(disable:4996)

/*
网络聊天室:
	1.和多个客户端连接
	2.在等待连接的同时也要能接受客户端连接
解决方案:
	异步IO
	IO多路复用(最常用Cnext,pool,epool)
	信号
	多线程,本质是并发(本程序)	所以4.,7.



*/

void communicate(int idx)
{
	//7.通信
	int r;
	char buff[56];
	char temp[80];

	while (1)
	{
		r = recv(clientSockert[idx], buff, 55, NULL);
		if (r > 0)
		{
			buff[r] = 0;//添加结束符号
			printf(">>%s\n", buff);
			memset(temp, 0, 80);
			sprintf(temp, "%d:%s", idx, buff);

			//发送给当前连上服务器的所有客户端
			//遍历,首先得知道有多少个

			for (int i = 0; i < count; i++)
			{
				send(clientSockert[i], temp, strlen(temp), NULL);
			}

		}
	}


}

int main()
{

	count = 0;

	//1.Windows需要确定网络协议版本
	//	Linux可以省略网络协议版本
	//有时候输入没有函数提示是因为没有include相应的头文件
	//函数名写对按F1出现在线的帮助文档
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		printf("确定网络协议版本失败:%d\n", GetLastError());
		system("pause");
		return -1;

	}
	printf("确定网络协议版本成功\n");

	//2.创建socket

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//任何通信协议都有这一步,创建socket
	if (SOCKET_ERROR == serverSocket)
	{

		printf("创建socket失败:%d\n", GetLastError());


		//9.清理协议版本信息即socket信息
		WSACleanup();

		system("pause");
		return -1;
	}
	printf("创建socket成功\n");


	//3.确定服务器协议地址簇
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//ip地址
	addr.sin_port = htons(9527);//端口号


//4.绑定
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof addr);
	if (-1 == r)
	{
		printf("绑定失败:%d\n", GetLastError());
		//8.关闭socket
		closesocket(serverSocket);
		//9.清理协议版本信息即socket信息
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("绑定成功\n");

	//5.监听
	r = listen(serverSocket, 10);
	if (-1 == r)
	{
		printf("监听失败:%d\n", GetLastError());
		//8.关闭socket
		closesocket(serverSocket);
		//9.清理协议版本信息即socket信息
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("监听成功\n");

	//6.接受客户端连接

	for (int i = 0; i < NUM; i++)
	{
		clientSockert[i] = accept(serverSocket, (sockaddr*)NULL, NULL);
		if (-1 == r)
		{
			printf("服务器奔溃:%d\n", GetLastError());
			//8.关闭socket
			closesocket(serverSocket);
			//9.清理协议版本信息即socket信息
			WSACleanup();
			system("pause");
			return -1;
		}
		printf("客户端连接\n");

		count++;

		CreateThread(NULL, NULL,
			(LPTHREAD_START_ROUTINE)communicate, (LPVOID)i, NULL, NULL);

		
	}


	

	//8.关闭socket
	closesocket(serverSocket);

	//9.清理协议版本信息即socket信息
	WSACleanup();
	while (1);
	return 0;

}