#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/video/background_segm.hpp>
#include<opencv2/video/tracking.hpp>


#pragma comment(lib, "ws2_32.lib")
#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480






//void* display(void* ptr);
void* display(SOCKET ptr);
//void audio(SOCKET ptr);

int capDev = 0;

cv::VideoCapture cap(capDev); // open the default camera




int main()
{

	cv::Mat cameraFeed;

	int height, width, IM_HEIGHT, IM_WIDTH, imgSize;


	//VideoCapture cap(0);



	const char* ip = "127.0.0.1";
	int port = 8080;
	int e;




	//initilize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);

	if (wsok != 0)
	{
		std::cerr << "Can't Initilize winsock! Quitting" << std::endl;
		return 0;

	}
	//Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create a socket! Qutting" << std::endl;
		return 0;
	}
	//Bind the socket to an ip address and port to a socket
	SOCKADDR_IN hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	//hint.sin_addr.S_un.S_addr = INADDR_ANY; //Could also use inet_pton....

	inet_pton(AF_INET, ip, &(hint.sin_addr));



	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	//wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST]; //client's remote name
	char service[NI_MAXHOST]; //service (i.e port) the client is connect on

	ZeroMemory(host, NI_MAXHOST);//same as memset(host, 0, NI_MAXHOST)
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << "Connected on port" << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << "Connected" << ntohs(client.sin_port) << std::endl;

	}


	//close listening socket
	closesocket(listening);



	//pthread_t thread_id;
	//pthread_create(&thread_id, NULL, display, &clientSocket);

	//DWORD thread_id;
	//CreateThread(NULL, 0, funThread, (void*)limit, 0, &ThreadId);
	//CreateThread(0, NULL, &clientSocket ,(void*)display,0,&thread_id);

	display(clientSocket);
	//audio(clientSocket);




	while (1)
	{
		cv::Mat frame, image;
		if (!cap.read(cameraFeed))
		{
			std::cout << "Video Ended" << std::endl;
			break;
		}
	}

	height = cameraFeed.rows;
	width = cameraFeed.cols;
	IM_HEIGHT = FRAME_HEIGHT;
	IM_WIDTH = FRAME_WIDTH;

	resize(cameraFeed, cameraFeed, cv::Size(IM_WIDTH, IM_HEIGHT));
	imgSize = cameraFeed.total() * cameraFeed.elemSize();
	send(clientSocket, reinterpret_cast<const char*> (cameraFeed.data), imgSize, 0);

	imshow("Server", cameraFeed);
	if (cv::waitKey() == 27)
	{
		closesocket(clientSocket);
		std::cout << "Client disconnected" << std::endl;
		return 0;
	}

	return 0;


}







void* display(SOCKET ptr) {
	//int socket = *(int*)ptr;
	SOCKET socket = ptr;
	//OpenCV Code
	//----------------------------------------------------------
	
	cv::Mat img, imgGray;
	img = cv::Mat::zeros(480, 640, CV_8UC3);
	//make it continuous
	if (!img.isContinuous()) {
		img = img.clone();
	}

	int imgSize = img.total() * img.elemSize();
	int bytes = 0;
	int key;


	//make img continuos
	if (!img.isContinuous()) {
		img = img.clone();
		imgGray = img.clone();
	}

	std::cout << "Image Size:" << imgSize << std::endl;

	while (1) {

		/* get a frame from camera */
		cap >> img;

		
		
		//do video processing here 
		//cvtColor(img, imgGray, COLOR_BGR2GRAY);
		//cvtColor(img, imgGray, CV_BGR2GRAY);

		//send processed image
		if ((bytes = send(socket,(const char*)img.data, imgSize, 0)) < 0) {
			std::cerr << "bytes = " << bytes << std::endl;
			break;
		}
	}
	return img.data;
	
}






