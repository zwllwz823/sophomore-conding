#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Header.h"
using namespace std;
//Éú³É
DNSHeader* fromDNSHeader(char*, char**);
DNSQuery* fromDNSQuery(char*, char**);
DNSResponse* fromDNSResponse(char*, char*, char**);
//½âÎö
char* toDNSHeader(DNSHeader*);
char* toDNSQuery(DNSQuery*);
char* toDNSResponse(DNSResponse*);
char* packDNSPacket(DNSPacket*, int&);

DNSPacket* unpackDNSPacket(char*);

