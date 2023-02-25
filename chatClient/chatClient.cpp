#include<stdio.h>

#include <winsock2.h>//WSAStartup()��Ҫ��ͷ�ļ�
#include<graphics.h>
#pragma comment(lib, "ws2_32.lib")//WSAStartup()��Ҫ�Ķ�̬��



/*socket2�Ѿ�����inet_ntoa()��inet_addr()��Щ�ϰ汾����
����Socket2�ϵ�����Щ�ϰ汾(Socket1�汾)����ʱ��warning���error��,�����#pragma warning(disable:4996)
*/
#pragma warning(disable:4996)
SOCKET serverSocket;
/*
����������:
	1.�Ͷ���ͻ�������
	2.�ڵȴ����ӵ�ͬʱҲҪ�ܽ��ܿͻ�������
�������:
	�첽IO
	IO��·����(���Cnext,pool,epool)
	�ź�
	���߳�,�����ǲ���(������)	����4.,7.



*/

void mySend()
{
	//ѭ�������û�����,���͸�������
	char buff[56];
	while (1)
	{
		scanf("%s", buff);
		send(serverSocket, buff, strlen(buff), NULL);

	}
}

int main()
{
	initgraph(200, 400,SHOWCONSOLE);//����һ��easyx����,
	int len = 0;//��ǰ��ʾλ��

	//1.Windows��Ҫȷ������Э��汾
	//	Linux����ʡ������Э��汾
	//��ʱ������û�к�����ʾ����Ϊû��include��Ӧ��ͷ�ļ�
	//������д�԰�F1�������ߵİ����ĵ�
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		printf("ȷ������Э��汾ʧ��:%d\n", GetLastError());
		system("pause");
		return -1;

	}
	printf("ȷ������Э��汾�ɹ�\n");

	//2.����socket

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//�κ�ͨ��Э�鶼����һ��,����socket
	if (SOCKET_ERROR == serverSocket)
	{

		printf("����socketʧ��:%d\n", GetLastError());


		//9.����Э��汾��Ϣ��socket��Ϣ
		WSACleanup();

		system("pause");
		return -1;
	}
	printf("����socket�ɹ�\n");


	//3.ȷ��������Э���ַ��
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//ip��ַ
	addr.sin_port = htons(9527);//�˿ں�


	//4.ֱ�����ӷ�����
	int r = connect(serverSocket, (sockaddr*)&addr, sizeof(addr));
	if (-1 == r)
	{
		printf("���ӷ�����ʧ��:%d\n", GetLastError());
		//8.�ر�socket
		closesocket(serverSocket);
		//9.����Э��汾��Ϣ��socket��Ϣ
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("���ӷ������ɹ�\n");

	//7.ͨ��
	
	
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
	

	//8.�ر�socket
	closesocket(serverSocket);

	//9.����Э��汾��Ϣ��socket��Ϣ
	WSACleanup();
	while (1);
	return 0;

}