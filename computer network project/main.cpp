#include "DomainAnalysis.h"
#include "Header.h"
#include "MessageConversion.h"
#include "RequestPool.h"

host_item* hosts_list[MAX_HOST_ITEM];
cache_item* cached_list[MAX_CACHED_ITEM];

ReqPool* request_pool;
mutex id_mutex, pool_mutex, req_counter_mutex, time_mutex, cache_mutex;
int req_counter = 0, host_counter = 0;
thread* dns_consulting_threads[MAX_THREAD];

int main() {
	dnsRelay();
	return 0;
}

int dnsRelay() {//监听端口报文信息并加入请求池
	loadHostsFromTxt();
	for (int i = 0; i < MAX_CACHED_ITEM; i++){
		cached_list[i] = (cache_item*)malloc(sizeof(cache_item));
		cached_list[i]->webaddr = (char*)malloc(DEFAULT_BUFLEN);
		cached_list[i]->occupied = false;
	}
	int i_result = 0;
	SOCKET Listen_Socket;
	i_result = initDNSServer(&Listen_Socket);
	if (i_result == 1) return 255;
	request_pool = (ReqPool*)malloc(sizeof(ReqPool) * MAX_REQ);
	if (request_pool == NULL) {
		printf("Fail to initialize request pool.\n");
		exit(100);
	}
	for (int i = 0; i < MAX_REQ; i++)
		request_pool[i].available = 1;
	for (int i = 0; i < MAX_THREAD; i++)
		dns_consulting_threads[i] = new thread(ConsultThreadHandle, UPPER_DNS, Listen_Socket, i);
	printf("Initialize completed.\n");
	while (1) {
		char* recvbuf = (char*)malloc(sizeof(char) * DEFAULT_BUFLEN);
		struct sockaddr_in clt_addr;
		int clt_addr_len = sizeof(clt_addr);
		if (sizeof(recvbuf) <= 0) {
			printf("Fail to allocate buff.\n");
			exit(100);
		}
		memset(recvbuf, 0, sizeof(recvbuf));
		ZeroMemory(&clt_addr, clt_addr_len);
		//收到客户端发来的DNS请求
		if (recvfrom(Listen_Socket, recvbuf, DEFAULT_BUFLEN, 0, (struct sockaddr*)&clt_addr, &clt_addr_len) == SOCKET_ERROR) 
			printf("[Listen_Socket]: recvfrom client error with: %d\n\n", WSAGetLastError());//一直等待数据的到来
		else {
			if (strcmp("127.0.0.1", inet_ntoa(clt_addr.sin_addr)) != 0){
				printf("Receive bad message!\n");
				continue;
			}
			printf("[Listen_Socket]: %d Bytes received from IP(%s): %d\n",sizeof(recvbuf),inet_ntoa(clt_addr.sin_addr), i_result);
			DNSRequest* new_req = (DNSRequest*)malloc(sizeof(DNSRequest));
			new_req->packet = unpackDNSPacket(recvbuf);
			new_req->processed = 0;
			new_req->client_addr = clt_addr;
			new_req->client_addr_len = clt_addr_len;
			if (addDNSRequestPool(new_req) == -1) {
				printf("[Listen_Socket]:Now on traffic,wait patiently.\n");
				Sleep(1000);
			}
		}
	}
}

int initDNSServer(SOCKET* ret_socket) {//初始化socket
	int i_result = 0;
	WSADATA wsaData;
	i_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (i_result != 0)
		return 255;
	i_result = 0;
	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (ListenSocket == INVALID_SOCKET) {
		WSACleanup();
		return 1;
	}
	struct sockaddr_in hints;
	hints.sin_family = AF_INET;
	hints.sin_addr.s_addr = INADDR_ANY;
	hints.sin_port = htons(DNS_PORT);
	i_result = ::bind(ListenSocket, (struct sockaddr*)&hints, sizeof(hints));//绑定socket和端口号
	if (i_result == SOCKET_ERROR) {
		WSACleanup();
		return 1;
	}
	*ret_socket = ListenSocket;
	return 0;
}

void ConsultThreadHandle(const char* upper_DNS_addr, SOCKET listen_socket, int t_id) {
	printf("[Consulting Thread %d]: Created.\n", t_id);
	char* sendbuf = (char*)malloc(DEFAULT_BUFLEN);
	char* dnsbuf = (char*)malloc(DEFAULT_BUFLEN);
	int i_result = 0;
	//上层DNS服务器的地址
	struct sockaddr_in servaddr;
	ZeroMemory(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(DNS_PORT);
	inet_pton(AF_INET, upper_DNS_addr, &servaddr.sin_addr);

	struct sockaddr_in myaddr;
	ZeroMemory(&myaddr, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(10000 + t_id);

	// 初始化上层DNS线程
	SOCKET upper_dns_socket = socket(AF_INET, SOCK_DGRAM, 0);
	::bind(upper_dns_socket, (struct sockaddr*)&myaddr, sizeof(myaddr));
	thread return_thread = thread(UpperThreadHandle, upper_dns_socket, listen_socket, t_id);
	while (1) {
		DNSRequest* req = NULL;
		while (req == NULL) {
			Sleep(20);
			req = getDNSRequest();
			time_t endTime = time(NULL);
			if (time_mutex.try_lock()) {
				for (int i = 0; i < MAX_REQ; i++) {
					if (!request_pool[i].available){
						if (request_pool[i].req->processed == true) {
							if (difftime(endTime, request_pool[i].startTime) > MAX_REQ_TTL) {
								DNSRequest* req = request_pool[i].req;
								DNSPacket* recv_packet = req->packet;
								finishDNSRequest(req->new_id);
							}
							break;
						}
					}
				}
				time_mutex.unlock();
			}
		}
		printf("[Consulting Thread %d]:Got DNSReq %d\n", t_id, req->new_id);
		DNSPacket* recv_packet = req->packet;
		u_int ip_addr = 0;
		ADDR_TYPE addr_type = getAddrType(recv_packet->p_qpointer[0]->q_qname, &ip_addr);
		printf("[Consulting Thread %d]:Search type finished, type: %d\n", t_id, addr_type);
		switch (addr_type) {
		case BLOCKED:
		case CACHED:
		{
			int sendbuflen;
			sendbuf = getDNSResult(recv_packet, req->old_id, ip_addr, addr_type, sendbuflen);
			printf("[Consulting Thread %d]:Start sending result to client\n\n", t_id);

			i_result = sendto(listen_socket, sendbuf, sendbuflen, 0, (struct sockaddr*)&(req->client_addr), req->client_addr_len);
			if (i_result == SOCKET_ERROR)
				printf("[Consulting Thread %d]:sendto() failed with error code : %d\n", t_id, WSAGetLastError());
			else
				printf("[Consulting Thread %d]:Bytes send back to 127.0.0.1: %d\n", t_id, i_result);
			finishDNSRequest(req->new_id);
		}
		break;
		case ADDR_NOT_FOUND:
		{
			int packet_length;
			ushort p_id = req->new_id;
			recv_packet->p_header->h_id = p_id;
			char* send_string = packDNSPacket(recv_packet, packet_length);
			printf("[Consulting Thread %d]:Start consulting Upper DNS: %s\n", t_id, upper_DNS_addr);
			if (sendto(upper_dns_socket, send_string, packet_length, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR)
				printf("[Consulting Thread %d]:sendto() failed with error code : %d\n", t_id, WSAGetLastError());
		}
		break;
		}
	}
}
void UpperThreadHandle(SOCKET upper_dns_socket, SOCKET listen_packet, int t_id) {
	int i_result = 0, sleeptime = 20;
	struct sockaddr_in servaddr;
	int servaddrlen = sizeof(servaddr);
	char* dnsbuf = (char*)malloc(DEFAULT_BUFLEN);
	while (true) {
		i_result = recvfrom(upper_dns_socket, dnsbuf, DEFAULT_BUFLEN, 0, (struct sockaddr*)&servaddr, &servaddrlen);
		if (i_result == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {//接受或发送缓存已满
				Sleep(sleeptime);
				continue;
			}
			else {
				printf("[DNS Thread %d]:! recvfrom_server() failed with error code : %d\n", t_id, WSAGetLastError());
				break;
			}
		}
		else {
			printf("[DNS Thread %d]:Bytes received from DNS_SERVER: %d\n", t_id, DEFAULT_BUFLEN);
			int p_id = ntohs(*(ushort*)dnsbuf);
			if (request_pool[p_id].available == true) continue;
			DNSPacket* return_pack = unpackDNSPacket(dnsbuf);
			//存入缓存
			
			if (return_pack->p_rpointer[0]->r_rdata != NULL && return_pack->p_qpointer[0]->q_qtype == 1){
				u_int* rdata_pointer = (u_int*)return_pack->p_rpointer[0]->r_rdata;
				u_int ip_uint = (*rdata_pointer);
				in_addr inaddr;
				inaddr.S_un.S_addr = ip_uint;
				char* ipaddr = inet_ntoa(inaddr);
				//if (strcmp(ipaddr,"3.119.119.119") == 0) continue;
				char* webaddr = (char*)malloc(DEFAULT_BUFLEN);
				strcpy(webaddr, return_pack->p_qpointer[0]->q_qname);
				// 将不可见的字符转化为.方便对比
				for (int i = 0; i < strlen(webaddr); i++){
					if (webaddr[i] < 0x20)
						webaddr[i] = '.';
					else if (webaddr[i] >= 'A' && webaddr[i] <= 'Z')
						webaddr[i] -= 'A' - 'a';
				}
				printf("[DNS Thread %d]:Domain: %s,IP: %s saved in CACHE\n", t_id, webaddr, ipaddr);
				cache_mutex.lock();
				for (int i = 0; i < MAX_CACHED_ITEM; i++){
					if (cached_list[i]->occupied) continue;
					cached_list[i]->occupied = true;
					inet_pton(AF_INET, ipaddr, &cached_list[i]->ip_addr);
					strcpy(cached_list[i]->webaddr, webaddr + 1);
					break;
				}
				cache_mutex.unlock();
			}
			
			DNSRequest* req = finishDNSRequest(p_id);
			*(ushort*)dnsbuf = htons(req->old_id);
			i_result = sendto(listen_packet, dnsbuf, DEFAULT_BUFLEN, 0, (struct sockaddr*)&(req->client_addr), req->client_addr_len);
			char* client_ipaddr = inet_ntoa(req->client_addr.sin_addr);
			if (i_result == SOCKET_ERROR)
				printf("[DNS Thread %d]:sendto() failed with error code : %d\n", t_id, WSAGetLastError());
			else
				printf("[DNS Thread %d]:Bytes send to %s : %d\n", t_id, client_ipaddr, i_result);
		}
	}
}