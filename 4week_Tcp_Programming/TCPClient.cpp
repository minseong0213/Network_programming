#include "../source/repos/Winsocket/Winsocket/common.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

int main(int argc, char* argv[])
{
	int retval;

	// 명령행 인수가 있으면 IP 주소로 사용
	if (argc > 1) SERVERIP = argv[1];

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(50000);
	retval = bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// connect()
	//struct sockaddr_in serveraddr;
	//memset(&serveraddr, 0, sizeof(serveraddr));
	//serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	// char buf[BUFSIZE + 1];
	// int len;
	
	  // 데이터 통신에 사용할 변수
	int a, b;
	int numbers[BUFSIZE / sizeof(int)];
	int result;

	// 서버와 데이터 통신
	while (1) {

		/*
		
		숫자 -> 문자열 변환 데이터 송수신

		*/
		/*if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;*/

		//sprintf(buf, "%d %d", a, b); // 숫자를 문자열로 받아 buf에 넣음 

		//// '\n' 문자 제거
		//len = (int)strlen(buf);
		//if (buf[len - 1] == '\n')
		//	buf[len - 1] = '\0';
		//if (strlen(buf) == 0)
		//	break;

		// 데이터 보내기
		//retval = send(sock, buf, (int)strlen(buf), 0);
		//if (retval == SOCKET_ERROR) {
		//	err_display("send()");
		//	break;
		//}

		// 데이터 받기
		// retval = recvn(sock, buf, retval, 0);
	/*	retval = recv(sock, buf, retval, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;*/

			//// 받은 데이터 출력
			//buf[retval] = '\0';
			//printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
			//printf("[받은 데이터] %s\n", buf);

		/*
		
		바이너리 방식  송수신
		
		*/

		// 데이터 입력
		printf("\n[첫번째 숫자를 입력하세요] ");
		scanf("%d", &a);
		printf("%d\n", a);
		printf("\n[두번째 숫자를 입력하세요] ");
		scanf("%d", &b);
		printf("%d\n", b);

		numbers[0] = a;
		numbers[1] = b;

		//데이터 전송
		int data_size = 2 * sizeof(int);
		retval = send(sock, (char*)numbers, data_size, 0); // int 배열 전송
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 서버로부터 합계 데이터 수신
		retval = recv(sock, (char*)&result, sizeof(result), MSG_WAITALL); // 합계 받기
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) {
			printf("서버에서 연결이 종료되었습니다.\n");
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval); // 서버에서 보낸 바이트 수
		printf("[받은 데이터] 합계: %d\n", result); // result는 서버에서 보낸 데이터
	}

	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
