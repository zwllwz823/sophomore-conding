#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <time.h>
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

#define DNS_PORT 53				//DNS serves on port 53
#define DEFAULT_BUFLEN 4096
#define DNS_HEADER_LEN 12
#define MAX_HOST_ITEM 1200
#define MAX_CACHED_ITEM 200
#define MAX_REQ 1000
#define UPPER_DNS "10.3.9.44"
#define MAX_THREAD 5
#define MAX_REQ_TTL 10

typedef unsigned short ushort;
typedef unsigned int u_int;

enum Query_QR {
	Q_QUERY = 0, Q_RESPONSE 
};

typedef struct DnsHeader
{
	ushort h_id;
	bool h_qr;
	ushort h_opcode;
	bool h_aa;
	bool h_tc;
	bool h_rd;
	bool h_ra;
	char h_rcode;
	ushort h_QDCount;
	ushort h_ANCount;
	ushort h_NSCount;
	ushort h_ARCount;
}DNSHeader;

typedef struct DnsQuery
{
	char *q_qname;
	ushort q_qtype;
	ushort q_qclass;
}DNSQuery;

typedef struct DnsResponse
{
	char *r_name;
	ushort r_type;
	ushort r_class;
	int r_ttl;
	ushort r_rdlength;
	char* r_rdata;
}DNSResponse;

typedef struct DnsPacket
{
	Query_QR p_qr;
	DnsHeader *p_header;
	DnsQuery *p_qpointer[50];
	DnsResponse *p_rpointer[50];
}DNSPacket;

typedef struct DnsRequest
{
	bool processed;
	int old_id;//用户发来ID
	int new_id;//发给DNS服务器ID
	DNSPacket* packet;
	struct sockaddr_in client_addr;//客户端地址信息以应答报文发给客户
	int client_addr_len;
}DNSRequest;

typedef struct RequestPool//请求池表
{
	bool available;
	DNSRequest* req;
	time_t startTime;
}ReqPool;

int dnsRelay();
int initDNSServer(SOCKET *);
void ConsultThreadHandle(const char*, SOCKET, int);
void UpperThreadHandle(SOCKET, SOCKET, int);