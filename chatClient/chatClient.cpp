#include<stdio.h>

#include <winsock2.h>//WSAStartup()需要的头文件
#include<graphics.h>
#pragma comment(lib, "ws2_32.lib")//WSAStartup()需要的动态库



/*socket2已经丢弃inet_ntoa()、inet_addr()这些老版本函数
当在Socket2上调用这些老版本(Socket1版本)函数时，warning变成error了,故添加#pragma warning(disable:4996)
*/
#pragma warning(disable:4996)
SOCKET serverSocket;
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

void mySend()
{
	//循环接受用户输入,发送给服务器
	char buff[56];
	while (1)
	{
		scanf("%s", buff);
		send(serverSocket, buff, strlen(buff), NULL);

	}
}

int main()
{
	initgraph(200, 400,SHOWCONSOLE);//创建一个easyx窗口,
	int len = 0;//当前显示位置

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

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//任何通信协议都有这一步,创建socket
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


	//4.直接连接服务器
	int r = connect(serverSocket, (sockaddr*)&addr, sizeof(addr));
	if (-1 == r)
	{
		printf("连接服务器失败:%d\n", GetLastError());
		//8.关闭socket
		closesocket(serverSocket);
		//9.清理协议版本信息即socket信息
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("连接服务器成功\n");

	//7.通信
	
	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mySend, NULL, NULL, NULL);
	char temp[60];

	while (1)
	{
		r = recv(serverSocket, temp, 59, NULL);
		if (r>0)
		{
			temp[r] = 0;
			outtextxy(1, len * 20, temp);
			len++;

		}
	}
	

	//8.关闭socket
	closesocket(serverSocket);

	//9.清理协议版本信息即socket信息
	WSACleanup();
	while (1);
	return 0;

}