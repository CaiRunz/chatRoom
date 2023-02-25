#include<stdio.h>

#include <winsock2.h>//WSAStartup()��Ҫ��ͷ�ļ�
#include<Windows.h>
#pragma comment(lib, "ws2_32.lib")//WSAStartup()��Ҫ�Ķ�̬��



#define NUM 1024
SOCKET clientSockert[NUM];
int count;


/*socket2�Ѿ�����inet_ntoa()��inet_addr()��Щ�ϰ汾����
����Socket2�ϵ�����Щ�ϰ汾(Socket1�汾)����ʱ��warning���error��,�����#pragma warning(disable:4996)
*/
#pragma warning(disable:4996)

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

void communicate(int idx)
{
	//7.ͨ��
	int r;
	char buff[56];
	char temp[80];

	while (1)
	{
		r = recv(clientSockert[idx], buff, 55, NULL);
		if (r > 0)
		{
			buff[r] = 0;//��ӽ�������
			printf(">>%s\n", buff);
			memset(temp, 0, 80);
			sprintf(temp, "%d:%s", idx, buff);

			//���͸���ǰ���Ϸ����������пͻ���
			//����,���ȵ�֪���ж��ٸ�

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

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//�κ�ͨ��Э�鶼����һ��,����socket
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


//4.��
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof addr);
	if (-1 == r)
	{
		printf("��ʧ��:%d\n", GetLastError());
		//8.�ر�socket
		closesocket(serverSocket);
		//9.����Э��汾��Ϣ��socket��Ϣ
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("�󶨳ɹ�\n");

	//5.����
	r = listen(serverSocket, 10);
	if (-1 == r)
	{
		printf("����ʧ��:%d\n", GetLastError());
		//8.�ر�socket
		closesocket(serverSocket);
		//9.����Э��汾��Ϣ��socket��Ϣ
		WSACleanup();
		system("pause");
		return -1;
	}
	printf("�����ɹ�\n");

	//6.���ܿͻ�������

	for (int i = 0; i < NUM; i++)
	{
		clientSockert[i] = accept(serverSocket, (sockaddr*)NULL, NULL);
		if (-1 == r)
		{
			printf("����������:%d\n", GetLastError());
			//8.�ر�socket
			closesocket(serverSocket);
			//9.����Э��汾��Ϣ��socket��Ϣ
			WSACleanup();
			system("pause");
			return -1;
		}
		printf("�ͻ�������\n");

		count++;

		CreateThread(NULL, NULL,
			(LPTHREAD_START_ROUTINE)communicate, (LPVOID)i, NULL, NULL);

		
	}


	

	//8.�ر�socket
	closesocket(serverSocket);

	//9.����Э��汾��Ϣ��socket��Ϣ
	WSACleanup();
	while (1);
	return 0;

}