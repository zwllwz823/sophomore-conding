#pragma once
#include "Header.h"

using namespace std;

DNSRequest* getDNSRequest();
int addDNSRequestPool(DNSRequest*);
DNSRequest* finishDNSRequest(int);