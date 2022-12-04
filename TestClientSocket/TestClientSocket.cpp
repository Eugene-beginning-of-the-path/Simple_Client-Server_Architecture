#define WIN32_LEAN_AND_MEAN //Макрос для компиляции сетевых заголовков - требование Micro 

#include <iostream>
#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>


using std::cout;
using std::endl;

int main()
{
	cout << "Client is starting..." << endl;

	//------------------------------------------
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) //просто открыли интерфесы для работы с сокетами windows
	{											   //в будущем никак не юзаем это
		cout << "Error in initialization of WinSock interface" << endl;
		return 1; //возвращаем в main код ошибки, если интерфейсы не открылись 
	}
	//------------------------------------------



	//----класс ADDRINFO хранит информацию о сокете, который мы будем искать----
	ADDRINFO hints; //выставляем параметры для поиска сокета на стороне сервера 
	ZeroMemory(&hints, sizeof(hints)); //"зануляем" поля ADDRINFO, чтобы не указывать все параметы
	hints.ai_family = AF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM; //Потоковый сокет для передачи данных
	hints.ai_protocol = IPPROTO_TCP; 
	//--------------------------------------------------------------------------



	//--------------------------------------------------------------------------
	ADDRINFO* addrResult;
	//ищем на стороне сервера сокет, с нужными нам параметрами, переданными через hints
	//addrResult получит сокет(-ы), удовлетворяющие нашему запросу 
	int errorStat = getaddrinfo("localhost", "666", &hints, &addrResult);
	if (errorStat != 0)
	{
		cout << "getaddrinfo failed with error" << endl;
		WSACleanup(); //этим мы закрываем WSAStartup();
		return 1;
	}
	//--------------------------------------------------------------------------

	SOCKET toServerSocket = INVALID_SOCKET; //создаем сокет и временно его закрываем 
	toServerSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); //создаем сокет на базе найденного Сервер-сокета addrResult
	if (toServerSocket == INVALID_SOCKET)
	{
		cout << "Socket creation failled" << endl;
		WSACleanup();
		freeaddrinfo(addrResult);
		return 1;
	}

	errorStat = connect(toServerSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (errorStat == SOCKET_ERROR)
	{
		cout << "Unable connect to server";
		closesocket(toServerSocket);
		toServerSocket = INVALID_SOCKET;

		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	const char* sendBuffer = "Hello from Client";
	errorStat = send(toServerSocket, sendBuffer, (int)strlen(sendBuffer), 0); //send() возвращает кол-во переданных данных/SOCKET_ERROR
	if (errorStat == SOCKET_ERROR)
	{
		cout << "Unable send a message to Server";
		closesocket(toServerSocket);
		toServerSocket = INVALID_SOCKET;

		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	cout << "Bytes sent to Server " << errorStat << " bytes with the message - " << sendBuffer << " (1)" << endl; //выводим размер отправленного сообщения

	errorStat = shutdown(toServerSocket, SD_SEND);
	//SD_SEND - передача данных через сокет прекращена, а прием на сокет продолжает работать - отключили сокет от передачи данных
	if (errorStat == SOCKET_ERROR)
	{
		cout << "Shutdown error";
		closesocket(toServerSocket);
		toServerSocket = INVALID_SOCKET;

		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	char recvBuffer[512];
	//т.к. сокет теперь работает в режиме только прослушки (не отправляет инфу на сервер), мы лишь бесконечно получаем инфу с сервера, пока цикл не прервётся
	do
	{
		ZeroMemory(recvBuffer, sizeof(recvBuffer));
		errorStat = recv(toServerSocket, recvBuffer, 512, 0);
		if (errorStat > 0) //Все умпешно принимается
		{
			cout << "Received a message from Server (4):" << endl;
			cout << "Received " << errorStat << " bytes" << endl;
			cout << "Received data: " << recvBuffer << endl;
		}
		else if (errorStat == 0)
			cout << "Connection closed" << endl;
		else
			cout << "recv failed" << endl;
	} while (errorStat > 0);

	closesocket(toServerSocket);
	toServerSocket = INVALID_SOCKET;

	freeaddrinfo(addrResult);
	WSACleanup();
	return 0;

}