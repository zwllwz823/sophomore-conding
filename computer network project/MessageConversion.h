#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Header.h"
using namespace std;
//����
DNSHeader* fromDNSHeader(char*, char**);
DNSQuery* fromDNSQuery(char*, char**);
DNSResponse* fromDNSResponse(char*, char*, char**);
//����
char* toDNSHeader(DNSHeader*);
char* toDNSQuery(DNSQuery*);
char* toDNSResponse(DNSResponse*);
char* packDNSPacket(DNSPacket*, int&);

DNSPacket* unpackDNSPacket(char*);

