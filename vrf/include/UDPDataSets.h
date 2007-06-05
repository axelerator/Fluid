#ifndef UDP_DATA_SETS
#define UDP_DATA_SETS

#include <stdlib.h>
#include <stdio.h>

/* Socket-Includes */
#ifdef WIN32
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <errno.h>
#endif

/* Emulation von Windows-Sockets unter Linux*/
#ifndef WIN32
	#define TRUE 1
	#define FALSE 0
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET 0
	#define WSAGetLastError() errno
	#define SOCKET int
	#define SOCKADDR_IN sockaddr_in
	#define SOCKADDR sockaddr
#endif

/* Kommunikations-Defines */
#define OPT_BBA_PORT 19998
#define OPT_VRF_PORT 19999
#define BBA_IP "127.0.0.1"
#define VRF_IP "127.0.0.1"
#define UDP_PORT 19000

typedef enum {DEFAULT, BOOLMATRIX, CENTROIDMATRIX} DATASET;
typedef char* IP;
typedef unsigned int Port;
typedef void* Data;

#endif
