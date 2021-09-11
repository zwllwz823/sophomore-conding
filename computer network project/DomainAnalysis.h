#pragma once
#include "Header.h"

enum ADDR_TYPE { 
	BLOCKED = 100, 
	CACHED, 
	ADDR_NOT_FOUND 
};

typedef struct host_item {
	u_int ip_addr;
	char* webaddr;
	ADDR_TYPE type;
}host_item;

typedef struct cache_item {
	u_int ip_addr;
	char* webaddr;
	int ttl;
	int occupied;
}cache_item;

void loadHostsFromTxt();
ADDR_TYPE getAddrType(char*, u_int*);
char* getDNSResult(DNSPacket* ori_packet, int old_id, u_int ip_addr, ADDR_TYPE addr_type, int& sendbuflen);