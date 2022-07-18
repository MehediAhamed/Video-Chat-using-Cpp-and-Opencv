


#include <iostream>
#include <WS2tcpip.h>
#include<WinSock2.h>
#include<sys/types.h>
#include<stdlib.h>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/video/background_segm.hpp>
#include<opencv2\video\tracking.hpp>

#include<SFML/Network.hpp>
#include<SFML/Audio.hpp>
#include <SFML/System.hpp>
#include<Windows.h>

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")
#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480



using namespace cv;


int imgSize;
int bytes = 0;
bool running = true;
char key;

int main()
{


	String ipAddress = "192.168.163.54";			// IP Address of the server
	int port =8080;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		//return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() <<std:: endl;
		WSACleanup();
		//return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		//return;
	}
	std::cerr << "connected to server" << std::endl;


	//while (running)
	//{
				//int bytesReceived = recv(sock, buf, 4096, 0);
		
		//int IM_HEIGHT = FRAME_HEIGHT;
		//int IM_WIDTH = FRAME_WIDTH;

		//Mat img = Mat::zeros(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);
		//imgSize = img.total() * img.elemSize();

		//const int ah = 921600; //921600 is an imgSize value


		//char sockData[9];
		//Mat img;
		//for (int i = 0; i < imgSize; i += bytes) {
		//	bytes = recv(connectSock, sockData + i, imgSize - i, 0));
		//}







		//char sockData[ah] = {0};

		//for (int i = 0; i < imgSize; i += bytes)
		//{
		//	bytes = recv(sock, sockData + i, imgSize - i, 0);
			
				//cout << "Recieved" << endl;
		
		//}



		//int ptr = 0;

		//for (int i = 0; i < img.rows; i++) {
		//	for (int j = 0; j < img.cols; j++) {
//
		//		img.at<Vec3b>(i, j) = Vec3b(sockData[ptr + 0], sockData[ptr + 1], sockData[ptr + 2]);
		//		ptr = ptr + 3;
		//	}
		//}


		//namedWindow("Client", WINDOW_AUTOSIZE);
		//imshow("Client", img);
	   


	




		Mat img;
		img = Mat::zeros(480, 640, CV_8UC3);
		int imgSize = img.total() * img.elemSize();
		uchar* iptr = img.data;
		int bytes = 0;
		int bytesAud = 0;
	

		//make img continuos
		if (!img.isContinuous()) {
			img = img.clone();
		}

		std::cout << "Image Size:" << imgSize << std::endl;


		namedWindow("CV Video Client", 1);

		

		while (key != 'q') {

			if ((bytes = recv(sock,(char*)iptr, imgSize, MSG_WAITALL)) == -1) {
				std::cerr << "recv failed, received bytes = " << bytes << std::endl;
			}

			cv::imshow("CV Video Client", img);

			if (key = cv::waitKey(5) >= 0) break;
		
			
			
		
		}

		closesocket(sock);

		return 0;
}










	//	key = waitKey();

		//running = key;
		//if (key == 27)
		//{
		//	closesocket(sock);
		//std:: cout << "Server disconnected" << std::endl;
		//	running = false;

		//}
	//}
	
	//return 0;
//}














