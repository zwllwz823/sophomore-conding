#pragma once
#pragma warning(disable : 4996)
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h>
#include <windows.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <list>
#include <fstream>
#include <tchar.h>
#include <winsock2.h>

#include "commodity.h"
#include "user.h"

#define CAP 7
#define TLEN 12
#define ALEN 30
#define BUF_SIZE 1000

using std::ofstream;
using std::ifstream;
using namespace::std;

class trade
{                                                 //�����࣬������Ʒ����
private:
    list<Book> dataList1;                                     //��Ʒ��������
    list<Cloth> dataList2;
    list<Food> dataList3;

public:
    float GetGoodsPrice(char shangname[]){
        for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
            if (strcmp(shangname, iter->name) == 0)
                return iter->getsellValue();
        }
        for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
            if (strcmp(shangname, iter->name) == 0)
                return iter->getsellValue();
        }
        for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
            if (strcmp(shangname, iter->name) == 0)
                return iter->getsellValue();
        }
        return 0;
    }
    int getint(SOCKET servSock, char ab[])                    //�ӿͻ��˽������ͣ�����ȥ��һ��ָʾ
    {
        SOCKADDR clntAddr;
        int notsend = 0;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //���տͻ�������

        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);      //���տͻ��˷���������
        if (strlen(ab) == 0 || strcmp(ab, "!\0") == 0) ab[0] = '!', ab[1] = '\0', notsend = 1, cout << "ֻ����";
        //puts(ab);
        cout << "from client:"; puts(buffer);
        int wrong = 0;
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (buffer[i] > '9' || buffer[i] < '0')
                wrong = 1;
            if (wrong == 1 && buffer[0] == '-' && i == 0) wrong = 0;//-�����
        }
        int n = atoi(buffer);
        //puts(ab);

        char bc[100] = { "�����ѽ���\0" };
        if (notsend == 0)
            strcpy(buffer, ab);
        else
            strcpy(buffer, bc);
        //puts(buffer);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                      //�����ݷ���
        closesocket(clntSock);                                  //�ر��׽���
        memset(buffer, 0, BUF_SIZE);                            //���û�����
        return n;
    }
    void getstr(SOCKET servSock, char ab[], char recvstr[])    //�ӿͻ��˽����ַ���������ȥ��һ��ָʾ
    {
        SOCKADDR clntAddr;
        int notsend = 0;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        if (strlen(ab) == 0 || strcmp(ab, "!\0") == 0)
            notsend = 1, ab[0] = '!', ab[1] = '\0', cout << "ֻ���н���\n" << endl;
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //���տͻ�������
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);       //���տͻ��˷���������
        cout << "from client:"; puts(buffer); strcpy(recvstr, buffer);
        //int n = atoi(buffer);
        char bc[100] = { "�����ѽ���\0" };
        if (notsend == 0)
            strLen = strlen(ab), send(clntSock, ab, strLen, 0);
        else
            strLen = strlen(bc), send(clntSock, buffer, strLen, 0);  //�����ݷ���

        closesocket(clntSock);                                   //�ر��׽���
        memset(buffer, 0, BUF_SIZE);                             //���û�����
        //return buffer;
    }
    void intab(char ab[], int a)                                //���ַ���ab�����������
    {
        char sen[10]; itoa(a, sen, 10);
        strcat(ab, sen);
    }
    void floatab(char ab[], float a)                            //���ַ���ab�����С����
    {
        char sen[1000] = { '\0' }; gcvt(a, 8, sen);
        strcat(ab, sen);
    }
    void senddrawing(SOCKET servSock, char ab[])
    {
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //���տͻ�������
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);  //���տͻ��˷���������
        puts(buffer);
        puts(ab);
        strcpy_s(buffer, ab);// puts(buffer);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                      //������ԭ������

        closesocket(clntSock);                                  //�ر��׽���
        memset(buffer, 0, BUF_SIZE);                            //���û�����
    }
    char theuser[30];                                           //��¼���û���
    void operate(SOCKET& servSock)
    {

        cout << "�밴���������" << endl;
        char ab[BUF_SIZE] = { "�밴���������" };
        senddrawing(servSock, ab);
        //getchar();
        system("cls");
    }
    void drawIndex(SOCKET& servSock)                            //����Ŀ¼
    {
        system("cls");
        printf("\t===================================================================\n");
        printf("\t                     *****��ƷĿ¼*****                             \n");
        printf("\t===================================================================\n");
        char ab[BUF_SIZE] = {
            "\t===================================================================\n"
            "\t                     *****��ƷĿ¼*****                             \n"
            "\t===================================================================\n\0"
        };
        senddrawing(servSock, ab);
    }
    int Updatestock(int index, int n)
    {
        for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++)
        {
            if (iter->getID() == index)
            {
                iter->UpdateStock(n);
                return 1;
            }
        }
        for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++)
        {
            if (iter->getID() == index)
            {
                iter->UpdateStock(n);
                return 2;
            }
        }
        for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++)
        {
            if (iter->getID() == index)
            {
                iter->UpdateStock(n);
                return 3;
            }
        }
        return 0;
    }
    bool GetInformathion(int index, SOCKET servSock);           //��ȡ�������Ʒ��Ϣ
    void GetIndex(SOCKET servSock);                             //��ȡ�������ƷĿ¼
    bool init1();                                               //�ӱ����ļ�������Ϣ
    bool init2();
    bool init3();
    void save1();
    void save2();
    void save3();                                               //����Ʒ��Ϣ���浽�����ļ���
    bool Buy(int ID, int count);                                //������Ʒ�Ĳ��������ݼ��
    bool Sell(int n, int ID);                                   //������Ʒ�Ĳ��������ݼ��
    void AddNew(char owner[], char name[], float bayValue, float sellValue, int leibie, char introduce[]);//�������Ʒ
    void save() {
        this->save1();
        this->save2();
        this->save3();
    }
    char* Getname(int index);                                   //�õ���Ʒ������
    char* Getowner(int index);                                  //�õ���Ʒ�������̼�
    float Getmoney(int index);                                  //����Ʒ�ļ�Ǯ
    int Getleibie(int index);                                   //�õ���Ʒ�����
    int Getstock(int index);                                    //�õ���Ʒ�Ŀ��
    int Getit(int index) {                                      //�鿴�Ƿ��и���Ʒ
        for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++)
        {
            if (iter->getID() == index)
                return 1;
        }
        for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++)
        {
            if (iter->getID() == index)
                return 2;
        }
        for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++)
        {
            if (iter->getID() == index)
                return 3;
        }
        return 0;
    }
    void manage(char name[], SOCKET& servSock);                  //�̼ҹ���ϵͳ
    bool init()
    {
        bool i1, i2, i3;
        i1 = this->init1(); i2 = init2(); i3 = init3();
        if (i1 || i2 || i3) return true;
        else return false;
    };                                                          //�ӱ����ļ���ȡ��Ʒ��Ϣ
    void newstr(char str[], int index, char name[], float BuyValue, float SellValue, int Stock, char owner[], char introduce[])
    {                                                           //����һ���µ��ַ���
        char ab[BUF_SIZE] = { "\n��Ʒ��ţ�\0" }, bc[BUF_SIZE];
        intab(ab, index);
        strcat(ab, "\n ��Ʒ���ƣ�\0");
        strcat(ab, name);
        strcat(ab, "\n ����۸�\0");
        floatab(ab, BuyValue);
        strcat(ab, "\n ���ۼ۸�\0");
        floatab(ab, SellValue);
        strcat(ab, "\n ʣ�ࣺ\0");
        intab(ab, Stock);
        strcat(ab, "\n �̼ң�\0");
        strcat(ab, owner);
        strcat(ab, "\n ���ܣ�");
        strcat(ab, introduce);
        strcpy(str, ab);

    }
    void search(SOCKET servSock)                                //��������
    {
        char ab[BUF_SIZE] = { "������Ҫ���ҵ���Ʒ����:\0" };
        char bc[100] = { "!\0" };
        getstr(servSock, ab, bc);
        cout << "������Ҫ���ҵ���Ʒ����:\0" << endl;
        char name[30];
        getstr(servSock, bc, name);
        int find = 0;                                           //����������Ʒ������Ѱ��Ʒ��������Ϣ
        for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++)
        {
            if (strcmp(iter->getName(), name) == 0) {
                find += 1;
                char str[BUF_SIZE];
                this->newstr(str, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
                getstr(servSock, str, bc);
                printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n",
                    iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner());
            }

        }
        for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++)
        {
            if (strcmp(iter->getName(), name) == 0)
            {
                find += 1;
                char str[BUF_SIZE];
                this->newstr(str, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
                getstr(servSock, str, bc);

                printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n",
                    iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner());
            }
        }
        for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++)
        {
            if (strcmp(iter->getName(), name) == 0)
            {
                find += 1;
                char str[BUF_SIZE];
                this->newstr(str, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
                getstr(servSock, str, bc);
                printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n",
                    iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner());
            }
        }
        if (find == 0) {
            strcpy_s(ab, "ƽ̨û�и���Ʒ����Ǹ\0");
            getstr(servSock, ab, bc);
            cout << "ƽ̨û�и���Ʒ����Ǹ" << endl;
        }

        else {
            cout << "���ҵ�" << find << "" << endl;
            strcpy_s(ab, "���ҵ�\0");
            intab(ab, find);
            strcat(ab, "�������Ʒ\0");
            getstr(servSock, ab, bc);
        }
    }
};
class order
{                                                               //������
public:
    int type; int ID;
    char name[30];                                              //��Ʒ��
    char owner[30];                                             //�̼�
    int count;
    float tolmoney;                                             //��������������Ǯ
public:
    order(int _type, int ID, char _name[], char _owner[], int count, float tolmoney)
    {                                                           //���캯��
        this->type = _type; this->ID = ID;
        this->count = count;
        strcpy_s(this->name, _name);
        strcpy_s(this->owner, _owner);
        //  strcpy_s(this->cust, _cust);
        this->tolmoney = tolmoney;
    }

};
class orderrecord                                               //������¼��
{
public:
    int state;                                                    //0δ֧����1֧�����
    int dingID;                                                   //�������
    int Goodsshu;                                              //��������Ʒ����
    int  Goodsamount[30] = { 0 };                              //ÿ����Ʒ������
    char customer[30];                                            //����������������
    int GoodsID[30] = { 0 };                                   //������ÿ����Ʒ��ID
    float Goodsmoney[30] = { 0 };                              //����ÿ����Ʒ����Ŀ�����Ǯ
    char owner[30][30];                                           //������ÿ����Ʒ�������̼�
    orderrecord()
    {
        this->state = 0;
        this->dingID = 0;
        this->Goodsshu = 0;
    }
    float gettotal()                                           //�����ܽ�Ǯ
    {
        float money = 0;
        for (int i = 0; i < this->Goodsshu; i++)
            money += this->Goodsmoney[i];
        return money;
    }
    void showrecord(SOCKET servSock, trade myTrade)              //չʾ������¼�е�����
    {
        cout << "������ţ�" << this->dingID << endl;
        char ab[BUF_SIZE];
        char bc[BUF_SIZE] = { "\0" };
        strcpy_s(ab, "������ţ�\0"); myTrade.intab(ab, this->dingID);
        strcat(ab, "\t��������Ʒ����:\0"); myTrade.intab(ab, this->Goodsshu);
        cout << "\t��������Ʒ����:\0" << this->Goodsshu << endl;
        for (int i = 0; i < this->Goodsshu; i++)
        {
            strcat(ab, "\n��ƷID:\0");  myTrade.intab(ab, this->GoodsID[i]);
            strcat(ab, "\t������\0");  myTrade.intab(ab, this->Goodsamount[i]);
            strcat(ab, "\t�̼ң�\0"); strcat(ab, this->owner[i]);
            cout << "��ƷID:" << this->GoodsID[i] << "\n"
                << "������" << this->Goodsamount[i] << "\n"
                << "�̼�:" << this->owner[i] << endl;
        }
        myTrade.getstr(servSock, ab, bc);
    }
};
class shoppingsystem                                            //����ϵͳ��
{
private:
    int static log;                                             //0δ��¼1�˿�2�̼�                             
    int static lognumber;   //-1δ��¼��0,1,2.��������ָʾ����Ӧ�����е�¼�û���λ��
    list<customer> custList;
    list<merchant> merList;
    //list<order> orderlist;
    list<orderrecord> record;
public:
    void removerecord()                                         //ɾ����֧���Ķ�����¼
    {
        auto iter = record.begin();
        for (; iter != record.end();)
        {
            if (iter->state == 1)   iter = record.erase(iter);
            else iter++;
        }

    }
    int amountofrecords(int ID)                                 //���������ɶ������ض�����Ʒ����
    {
        int amount = 0;
        for (auto iter = record.begin(); iter != record.end(); iter++) {
            for (int i = 0; i < iter->Goodsshu && iter->GoodsID >= 0; i++)
                if (iter->GoodsID[i] == ID && iter->state == 0) 
                    amount += iter->Goodsamount[i];
        }
        return amount;
    }
    void senddrawing(SOCKET servSock, char ab[]) {                  //���ͽ���
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //���տͻ�������
        strcpy_s(buffer, ab); puts(buffer);
        int strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                       //������ԭ������
        closesocket(clntSock);                                   //�ر��׽���
        memset(buffer, 0, BUF_SIZE);                             //���û�����
    }
    void intab(char ab[], int a)                                 //���ַ���ab�����������
    {
        char sen[10]; itoa(a, sen, 10);
        strcat(ab, sen);
    }
    void floatab(char ab[], float a)                              //���ַ���ab�����С����
    {
        char sen[10]; gcvt(a, 8, sen);
        strcat(ab, sen);
    }
    int getint(SOCKET servSock, char ab[])                         //�ӿͻ��˽������ͣ�����ȥ��һ��ָʾ
    {
        SOCKADDR clntAddr;
        int notsend = 0;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);//���տͻ�������

        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);               //���տͻ��˷���������
        if (strlen(ab) == 0 || strcmp(ab, "!\0") == 0) ab[0] = '!', ab[1] = '\0', notsend = 1, cout << "ֻ����";
        //puts(ab);
        cout << "from client:"; puts(buffer);
        int wrong = 0;
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (buffer[i] > '9' || buffer[i] < '0')
                wrong = 1;
            if (wrong == 1 && buffer[0] == '-' && i == 0) wrong = 0;//-�����
        }
        int n = atoi(buffer);
        char bc[100] = { "�����ѽ���\0" };
        if (notsend == 0)
            strcpy(buffer, ab);
        else
            strcpy(buffer, bc);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                     //�����ݷ���
        closesocket(clntSock);                                 //�ر��׽���
        memset(buffer, 0, BUF_SIZE);                           //���û�����
        return n;
    }
    void  getstr(SOCKET servSock, char ab[], char recvstr[])   //�ӿͻ��˽����ַ���������ȥ��һ��ָʾ
    {
        SOCKADDR clntAddr;
        int notsend = 0;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        if (strlen(ab) == 0 || strcmp(ab, "!\0") == 0) notsend = 1, ab[0] = '!', ab[1] = '\0', cout << "ֻ���н���\n" << endl;
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //���տͻ�������
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);        //���տͻ��˷���������
        cout << "from client:"; puts(buffer);
        char bc[100] = { "�����ѽ���\0" };
        strcpy(recvstr, buffer);
        if (notsend == 0)
            strLen = strlen(ab), send(clntSock, ab, strLen, 0);
        else
            strLen = strlen(bc), send(clntSock, buffer, strLen, 0);     //�����ݷ���
        closesocket(clntSock);                                     //�ر��׽���
        memset(buffer, 0, BUF_SIZE);                               //���û�����
    }
    void operate(SOCKET& servSock){
        cout << "�밴���������" << endl;
        char ab[BUF_SIZE] = { "�밴���������" };
        senddrawing(servSock, ab);
        //getchar();
        system("cls");
    }
    void drawIndex(SOCKET& servSock)
    {
        system("cls");
        printf("\t===================================================================\n");
        printf("\t                     *****��ƷĿ¼*****                             \n");
        printf("\t===================================================================\n");
        char ab[BUF_SIZE] = {
            "\t===================================================================\n"
            "\t                     *****��ƷĿ¼*****                             \n"
            "\t===================================================================\n\0"
        };
        senddrawing(servSock, ab);
    }
    void drawLine(SOCKET& servSock)
    {
        printf("\t===================================================================\n");
        char ab[BUF_SIZE] = {
            "\t===================================================================\n\0"

        };
        senddrawing(servSock, ab);
    }
    void DrawMainMenu(SOCKET& servSock)
    {
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //���տͻ�������
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);  //���տͻ��˷���������
        puts(buffer);
        char ab[BUF_SIZE] = {
            "\t =================================================================\n"
            "\t|                         ��ӭʹ��                               |\n"
            "\t ================================================================= \n"
            "\t                  1.��¼                                          \n"
            "\t                  2.�鿴��Ʒ                                      \n"
            "\t                  3.������Ʒ                                      \n"
            "\t                  4.�����˺�                                      \n"
            "\t                  �˳����밴0��                                   \n"
            "\t =================================================================\n""����������ѡ��:\0"
        };
        puts(ab);
        strcpy_s(buffer, ab); puts(buffer);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);  //������ԭ������
        send(clntSock, buffer, strLen, 0);  //������ԭ������
        closesocket(clntSock);  //�ر��׽���
        memset(buffer, 0, BUF_SIZE);  //���û�����
    }
    void DrawMenu1(SOCKET& servSock)                            //�����߽���
    {
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //���տͻ�������
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);       //���տͻ��˷���������
        puts(buffer);
        char ab[BUF_SIZE] = {
            "\t =================================================================\n"
            "\t|                         ��ӭʹ��                               |\n"
            "\t ================================================================= \n"
            "\t                     1.�ǳ�                                        \n"
            "\t                     2.�鿴��Ʒ                                    \n"
            "\t                     3.������Ʒ                                    \n"
           "\t                      4.�޸���Ϣ                                      \n"
            "\t                     5.���ﳵ                                         \n"
            "\t                     �˳����밴0��                                 \n"
            "\t =================================================================\n""����������ѡ��:\0"
        };
        puts(ab);
        strcpy_s(buffer, ab); puts(buffer);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                     //������ԭ������
        send(clntSock, buffer, strLen, 0);                     //������ԭ������
        closesocket(clntSock);                                 //�ر��׽���
        memset(buffer, 0, BUF_SIZE);                           //���û�����
    }
    void DrawMenu2(SOCKET& servSock)                           //�̼ҽ���
    {
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //���տͻ�������
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);      //���տͻ��˷���������
        puts(buffer);
        char ab[BUF_SIZE] = {
           "\t =================================================================\n"
            "\t|                         ��ӭʹ��                               |\n"
            "\t ================================================================= \n"
            "\t                     1.�ǳ�                                        \n"
            "\t                     2.�鿴��Ʒ                                    \n"
            "\t                     3.������Ʒ                                    \n"
            "\t                     4.�޸���Ϣ                                      \n"
            "\t                     5.������Ʒ                                      \n"
            "\t                     6.������Ʒ                                      \n"
            "\t                     �˳����밴0��                                 \n"
            "\t =================================================================\n""����������ѡ��:\0"
        };
        puts(ab);
        strcpy_s(buffer, ab); puts(buffer);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                     //������ԭ������
        send(clntSock, buffer, strLen, 0);                     //������ԭ������
        closesocket(clntSock);                                 //�ر��׽���
        memset(buffer, 0, BUF_SIZE);                           //���û�����
    }
    bool createaccount(SOCKET& sevSocket);                      //�����˺�
    int login(trade& myTrade, SOCKET& sevSocket);               //��¼
    void logout(trade& myTrade, SOCKET& sevSocket);             //�ǳ�
    bool init1();                                               //�����ļ��ĳ�ʼ��
    bool init2();
    void save1();                                               //�����ļ��ı���
    void save2();
    int ismer(char user[]);                                      //�ж��û��Ƿ�Ϊ�̼�
    int iscust(char user[]);                                     //�ж��û��Ƿ�Ϊ������
    int shoppingcar(trade& myTrade, SOCKET& sevSocket);
    void save()                                                  //�洢�û���Ϣ�������ļ�
    {
        save1();
        save2();
    }
    bool init()                                                  //�ӱ����ļ��г�ʼ��
    {
        int ok = 0, ko = 0;
        this->log = 0;
        this->lognumber = -1;
        if (this->init1()) { ok = 1; cout << "1chushi" << endl; }
        if (this->init2()) { ko = 1; cout << "2chushi" << endl; }
        if (ok || ko) return true;
        else return false;
    }
    int information(trade& myTrade, SOCKET& sevSocket);          //��ȡ�޸ĵ�ǰ��¼�û���Ϣ��
    int information1(trade& myTrade, SOCKET& sevSocket);
    int mainsystem(SOCKET& sevSocket);                           //��ϵͳ
    int custsystem(trade& myTrade, SOCKET& sevSocket);           //������ϵͳ
    int merchantsystem(trade& mytrade, SOCKET& sevSocket);       //�̼�ϵͳ
    void mermanage(trade& myTrade, SOCKET& sevSocket);           //�̼ҹ������
};
int shoppingsystem::ismer(char user[])
{
    int find = 0;
    for (auto iter = merList.begin(); iter != merList.end(); iter++)
        if (strcmp(user, iter->username) == 0) find = 1;
    return find;
}
int shoppingsystem::iscust(char user[])
{
    int find = 0;
    for (auto iter = custList.begin(); iter != custList.end(); iter++)
        if (strcmp(user, iter->username) == 0) find = 1;
    return find;
}
bool shoppingsystem::createaccount(SOCKET& servSock)
{
    char ab[BUF_SIZE] = { "�������û����2Ϊ�̼ң�1Ϊ������:\0" }, bc[100] = { "!\0" };
    cout << "�������û����2Ϊ�̼ң�1Ϊ������:\0" << endl;
    getstr(servSock, ab, bc);
    int type; //cin >> type;
    type = getint(servSock, bc);
    strcpy_s(ab, "��ʼ�����û����������û���:\0");
    getstr(servSock, ab, bc);
    cout << "��ʼ�����û����������û���:\0" << endl;
    char user[30] = { "\0" };
    bc[0] = '\0';
    getstr(servSock, bc, user);
    //cin >> user; getchar();
    auto iter1 = merList.begin(); auto iter2 = custList.begin();
    if (type == 2)                                              //�ж����
    {
        if (this->ismer(user) == 0 && this->iscust(user) == 0) {
            cout << "���������룺" << endl;
            strcpy_s(ab, "����������:\0");
            getstr(servSock, ab, bc);
            char pass[30] = { "\0" };
            ab[0] = '\0';
            getstr(servSock, ab, pass);
            strcpy(ab, "�������˻����:\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            cout << "�������˻���" << endl;
            float money; //cin >> money;
            //getchar(); 
            ab[0] = '!', ab[1] = '\0';
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            money = atof(bc);
            merList.push_back(merchant(user, pass, money, 0));

        }
        else {
            cout << "�û����ظ��������´���" << endl;
            strcpy_s(ab, "�û����ظ��������´���\0");
            getstr(servSock, ab, bc);
            return false;
        }
    }
    if (type == 1)
    {
        if (this->ismer(user) == 0 && this->iscust(user) == 0) {
            cout << "���������룺" << endl;
            strcpy(ab, "����������:\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            char pass[30] = { "\0" }; ab[0] = '\0';
            getstr(servSock, ab, pass);
            //cin >> pass; getchar();
            strcpy_s(ab, "�������˻����:\0");
            getstr(servSock, ab, bc);
            cout << "�������˻���" << endl;
            float money;// cin >> money;getchar();
            ab[0] = '!', ab[1] = '\0';
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            money = atof(bc);
            custList.push_back(customer(user, pass, money, 0));
        }
        else {
            strcpy_s(ab, "�û����ظ��������´���\0");
            getstr(servSock, ab, bc);
            cout << "�û����ظ��������´���" << endl;
            return false;
        }
    }
    return false;
}
int shoppingsystem::login(trade& myTrade, SOCKET& servSock){
    char pass[30]; int num = 0;
    int find = 0;//find==0,δ�ҵ�;find==1���ҵ�customer;find==2�ҵ�merchant��
    char ab[BUF_SIZE] = { "�������û���:\0" };
    char bc[100] = { "\0" };
    this->getstr(servSock, ab, bc);
    cout << "�������û���:\0" << endl;
    char user[30] = { "\0" }; //cin >> user;
    //getchar();
    bc[0] = '!\0';
    this->getstr(servSock, bc, user);
    if (this->log != 0) {
        cout << "�����˳���ǰ�û�" << endl;
        strcpy_s(ab, "�����˳���ǰ�û�\0");
        bc[0] = '!\0';
        getstr(servSock, ab, bc);
        return 0;
    }                                                 //�ظ���¼
    auto iter1 = custList.begin();
    auto iter2 = merList.begin();
    for (auto iter = custList.begin(); find == 0 && iter != custList.end(); iter++, num++)
        if (strcmp(user, iter->username) == 0) {
            find = 1; iter1 = iter; this->lognumber = num; num = 0;
        }
    if (find == 0)
        for (auto iter = merList.begin(); find == 0 && iter != merList.end(); iter++, num++) {
            if (strcmp(user, iter->username) == 0) {
                find = 2; iter2 = iter; this->lognumber = num; num = 0;
            }//cout << user << iter->username;
        }
    if (find == 0) {
        cout << "δ�������û�����½ʧ��" << endl;
        strcpy_s(ab, "δ�������û�����½ʧ��\0");                   //��½ʧ��
        bc[0] = '\0';
        getstr(servSock, ab, bc);
        this->log = 0; this->lognumber = -1; return 0;
    };
    if (find == 1) {                                            //�����û���ʼ��������
        cout << "���������룺" << endl;
        strcpy_s(ab, "����������:\0"); bc[0] = '\0';
        getstr(servSock, ab, bc);
        bc[0] = '\0'; pass[0] = '\0';
        getstr(servSock, bc, pass);
        //cin >> pass;
        //getchar();
        if (strcmp(iter1->password, pass) == 0)                 //������ȷ��½�ɹ�
        {
            cout << "��½�ɹ�" << endl;
            strcpy_s(ab, "��½�ɹ�\0");
            getstr(servSock, ab, bc);
            iter1->state = 1; this->log = 1;                    //��ǵ�¼
            strcpy_s(myTrade.theuser, user);
            return 1;
        }
        else {                                                  //��������½ʧ��
            cout << "�������" << endl;
            strcpy_s(ab, "�������\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            this->lognumber = -1; this->log = 0;
            return 0;
        }
    }
    if (find == 2) {
        cout << "���������룺" << endl;
        // cin >> pass;
         //getchar();
        strcpy_s(ab, "����������:\0");
        pass[0] = '\0';
        getstr(servSock, ab, pass);
        pass[0] = '\0'; ab[0] = '\0';
        getstr(servSock, ab, pass);
        if (strcmp(iter2->password, pass) == 0)
        {
            cout << "��½�ɹ�" << endl;
            strcpy_s(ab, "��½�ɹ�\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            iter2->state = 1; this->log = 2;//��ǵ�¼
            strcpy_s(myTrade.theuser, user);
            return 2;
        }
        else {
            cout << "�������" << endl;
            strcpy_s(ab, "�������\0"); bc[0] = '\0';
            getstr(servSock, ab, bc);
            this->lognumber = -1; this->log = 0;
            return 0;
        }
    }
    return 0;
}
void shoppingsystem::logout(trade& myTrade, SOCKET& servSock)
{                                                              //�ǳ����޸�ϵͳ״̬
    for (auto iter = custList.begin(); iter != custList.end(); iter++)
        if (strcmp(iter->username, myTrade.theuser) == 0) 
            iter->state = 0;
    for (auto iter = merList.begin(); iter != merList.end(); iter++)
        if (strcmp(iter->username, myTrade.theuser) == 0)
            iter->state = 0;
    char none[30] = "\0";
    this->log = 0; 
    this->lognumber = -1; 
    strcpy_s(myTrade.theuser, none);
}
int shoppingsystem::information(trade& myTrade, SOCKET& servSock)//�鿴�޸ĵ�¼�û�����Ϣ��
{
    int over = 0;
    if (this->log == 0 && this->lognumber == -1){
        cout << "δ��¼" << endl;
        char ab[100] = { "δ��¼\0" };
        char bc[100] = { "!\0" };
        getstr(servSock, ab, bc);
        return 0;
    }
    else if (this->log == 1 && this->lognumber != -1){
        auto iter = custList.begin();
        for (int i = 0; i < this->lognumber && iter != custList.end(); i++)
            iter++;
        if (iter->state == 1){
            char ab[3000] = { "�û�����\0" }; char bc[BUF_SIZE] = { "!\0" };
            strcat(ab, iter->username);
            strcat(ab, "\n�û����ͣ�������\0");
            strcat(ab, "\n���룺\0");
            strcat(ab, iter->password);
            strcat(ab, "\n�˺���\0");
            this->floatab(ab, iter->money);
            puts(ab);
            this->getstr(servSock, ab, bc);

            cout << "�û�����" << iter->username << endl;
            cout << "�û����ͣ�������" << endl;
            cout << "���룺" << iter->password << endl;
            cout << "�˺���" << iter->money << endl;
        }
        for (int order = 0; order != 3;){
            char ab[BUF_SIZE] = { "�鿴������0����ֵ������1���޸�����������2���˳�������3:\0" };
            char bc[100] = { "!\0" };
            getstr(servSock, ab, bc);
            cout << "�鿴������0����ֵ������1���޸�����������2���˳�������3:" << endl;
            //cin >> order; getchar();
            order = getint(servSock, bc);
            if (order == 0) {
                strcat(ab, iter->username);
                strcat(ab, "\n�û����ͣ�������\0");
                strcat(ab, "\n���룺\0");
                strcat(ab, iter->password);
                strcat(ab, "\n�˺���\0");
                this->floatab(ab, iter->money);
                this->getstr(servSock, ab, bc);
                cout << "�û�����" << iter->username << endl;
                cout << "�û����ͣ�������" << endl;
                cout << "���룺" << iter->password << endl;
                cout << "�˺���" << iter->money << endl;
            }
            if (order == 1) {
                strcpy_s(ab, "�������ֵ���:\0");
                getstr(servSock, ab, bc);
                cout << "�������ֵ��" << endl;
                float money; //cin >> money;
                ab[0] = '!'; ab[1] = '\0';
                this->getstr(servSock, ab, bc);
                money = atof(bc);
                iter->money += money; //getchar();
                cout << "��ֵ�ɹ�" << endl;
                strcpy_s(ab, "��ֵ�ɹ�\0");
                getstr(servSock, ab, bc);
            }
            if (order == 2) {
                char pass[30];
                strcpy_s(ab, "������������:\0");
                getstr(servSock, ab, bc);
                cout << "�����������룺" << endl;
                //cin >> pass;
                ab[0] = '!'; ab[1] = '\0';
                getstr(servSock, ab, pass);
                strcpy_s(iter->password, pass);
                cout << "���޸�" << endl;
                strcpy_s(ab, "���޸�");
                getstr(servSock, ab, bc);
                //getchar();
            }
            if (order == 3) over = 1;
        }
        return 1;
    }
    char ab[100] = { "δ��¼\0" };
    char bc[100] = { "!\0" };
    strcpy_s(ab, "�˳��޸�\n\0");
    getstr(servSock, ab, bc);
    printf("�˳��޸�\n");
    return over;
}
int shoppingsystem::information1(trade& myTrade, SOCKET& servSock)//�鿴�޸ĵ�¼�û�����Ϣ��
{
    int over = 0;

    if (this->log == 0 && this->lognumber == -1)
    {
        char ab[100] = { "δ��¼\0" };
        char bc[100] = { "!\0" };
        getstr(servSock, ab, bc);
        cout << "δ��¼" << endl; return 0;
    }
    else if (this->log == 2 && this->lognumber != -1){
        auto iter = merList.begin();
        auto it = iter;
        for (; iter != merList.end(); iter++)     
            if (strcmp(iter->username, myTrade.theuser) == 0) it = iter;
        char ab[3000] = { "�û�����" }; char bc[1000] = { "!\0" };
        strcat(ab, it->username);
        strcat(ab, "\n�û����ͣ��̼�\0");
        strcat(ab, "\n���룺\0");
        strcat(ab, it->password);
        strcat(ab, "\n�˺���\0");
        this->floatab(ab, it->money);
        this->getstr(servSock, ab, bc);
        cout << "�û�����" << it->username << endl;
        cout << "�û����ͣ��̼�" << endl;
        cout << "���룺" << it->password << endl;
        cout << "�˺���" << it->money << endl;
        for (int order = 0; order != 3;){
            char ab[BUF_SIZE] = { "�鿴������0����ֵ������1���޸�����������2���˳�������3:\0" };
            char bc[1000] = { "!\0" };
            getstr(servSock, ab, bc);
            cout << "�鿴������0����ֵ������1���޸�����������2���˳�������3:" << endl;
            order = getint(servSock, bc);
            if (order == 0) {
                strcpy_s(ab, "\n�û�����"); char bc[3] = { "!\0" };
                strcat(ab, it->username);
                strcat(ab, "\n�û����ͣ��̼�\0");
                strcat(ab, "\n���룺\0");
                strcat(ab, it->password);
                strcat(ab, "\n�˺���\0"); floatab(ab, it->money);
                bc[0] = '\0';
                getstr(servSock, ab, bc);
                cout << "�û�����" << it->username << endl;
                cout << "�û����ͣ��̼�" << endl;
                cout << "���룺" << it->password << endl;
                cout << "�˺���" << it->money << endl;
            }
            if (order == 1) {
                cout << "�������ֵ��" << endl;
                float money; //cin >> money;
                strcpy_s(ab, "�������ֵ���:\0");
                this->getstr(servSock, ab, bc);
                bc[0] = '\0';
                ab[0] = '!'; ab[1] = '\0';
                this->getstr(servSock, ab, bc);
                money = atof(bc);
                it->money += money;// getchar();
                cout << "��ֵ�ɹ�" << endl;
                strcpy_s(ab, "��ֵ�ɹ�\0");
                getstr(servSock, ab, bc);
            }
            if (order == 2) {
                char pass[30] = { "\0" };
                cout << "�����������룺" << endl;
                strcpy_s(ab, "������������:\0");
                getstr(servSock, ab, bc);
                //cin >> pass;
                ab[0] = '\0';
                getstr(servSock, ab, pass);
                strcpy_s(it->password, pass);
                cout << "���޸�" << endl;
                strcpy_s(ab, "���޸�");
                getstr(servSock, ab, bc);
                //getchar();
            }
            if (order == 3) over = 1;
        }
        return 2;
    }
    char ab[BUF_SIZE], bc[BUF_SIZE];
    strcpy_s(ab, "�˳��޸�\n\0");
    getstr(servSock, ab, bc);
    printf("�˳��޸�\n");
    return over;
}
void shoppingsystem::save2(){
    ofstream   file2;
    file2.open("merchant.txt");
    if (!file2.fail()){
        file2 << "=========================================================================\n";
        file2 << "                         *****�̼���Ϣ*****                               \n";
        file2 << "=========================================================================\n";
        for (auto iter = merList.begin(); iter != merList.end(); iter++)
        {
            file2 << "USER:" << iter->username
                << "\tPASSWORD:" << iter->password
                << "\tMONEY:" << iter->money
                << "\n";
        }

    }
    else printf("��¼�ļ�����ʧ��");
}
void shoppingsystem::save1(){
    ofstream   file1;
    file1.open("customer.txt");
    if (!file1.fail())
    {
        file1 << "=========================================================================\n";
        file1 << "                      *****��������Ϣ*****                               \n";
        file1 << "=========================================================================\n";
        for (auto iter = custList.begin(); iter != custList.end(); iter++)
        {
            file1 << "USER:" << iter->username
                << "\tPASSWORD:" << iter->password
                << "\tMONEY:" << iter->money
                << "\n";
        }

    }
    else printf("��¼�ļ�����ʧ��\n");
}

bool shoppingsystem::init1(){
    ifstream file1("customer.txt");
    if (!file1.fail()){
        char titles[3][TLEN] = { "USER:" ,"PASSWORD:" ,"MONEY:" };
        char saves[3][ALEN] = {};
        int tIndex = 0;
        char buf[128];
        int kinds = 0;
        for (int i = 0; i < 3; i++)                             //���Ա���
            file1.getline(buf, 128);
        while (!file1.eof()){
            char temp[TLEN] = "";                               //��ȡ�ļ����ݵ��ַ�����
            for (int i = 0; i < TLEN && !file1.eof(); i++) {      //for NO.1
                file1.get(temp[i]);
                if (strcmp(temp, titles[tIndex]) == 0){
                    for (int j = 0; j < ALEN && !file1.eof(); j++) {//NO.2
                        char c;
                        file1.get(c); cout << c << endl;
                        if (c != '\t' && c != '\n'){
                            saves[tIndex][j] = c; 
                            saves[tIndex][j + 1] = '\0';
                        }
                        else if (c != '\n') {
                            if (tIndex > 2)                      
                                return false;
                            tIndex++;
                            break;
                        }
                        else {
                            custList.push_back(customer(saves[0], saves[1], atof(saves[2]), 0));
                            tIndex = 0;
                            break;
                        }
                        if (j == ALEN - 1)
                            return false;                       //�����������ȣ���ʼ��ʧ��
                    }
                    break;
                }
                if (i == TLEN - 1)
                    return false;                               //δƥ�䵽�������ƣ�ʧ��
            }
        }
        return true;
    }
    return false;                                               //�ļ�������
}
bool shoppingsystem::init2(){
    ifstream file2("merchant.txt");
    if (!file2.fail()){
        cout << "The file opens." << endl;
        char titles[3][TLEN] = { "USER:" ,"PASSWORD:" ,"MONEY:" };
        char saves[3][ALEN] = {};
        int tIndex = 0;
        char buf[128];
        int kinds = 0;
        for (int i = 0; i < 3; i++)//���Ա���
            file2.getline(buf, 128);
        while (!file2.eof()){
            char temp[TLEN] = "";//��ȡ�ļ����ݵ��ַ�����
            for (int i = 0; i < TLEN && !file2.eof(); i++){//for NO.1
                file2.get(temp[i]);
                if (strcmp(temp, titles[tIndex]) == 0) {               
                    for (int j = 0; j < ALEN && !file2.eof(); j++) {//NO.2
                        char c;
                        file2.get(c);
                        if (c != '\t' && c != '\n'){
                            saves[tIndex][j] = c; 
                            saves[tIndex][j + 1] = '\0';
                        }
                        else if (c != '\n'){
                            if (tIndex > 2)
                                return false;
                            tIndex++;
                            break;
                        }
                        else {
                            merList.push_back(merchant(saves[0], saves[1], atof(saves[2]), 0));
                            tIndex = 0;
                            break;
                        }
                        if (j == ALEN - 1)                        
                            return false;//�����������ȣ���ʼ��ʧ��                        
                    }
                    break;
                }
                if (i == TLEN - 1)               
                    return false;//δƥ�䵽�������ƣ�ʧ��                
            }
        }
        return true;
    }
    return false;//�ļ�������
}
int commodity::kinds = 0;                                      //��ʼʱ��Ʒ����Ϊ0
int shoppingsystem::log = 0;                                   //��¼״̬Ϊ0
int shoppingsystem::lognumber = -1;                            //��½����
void commodity::UpdateStock(int n){stock += n;}
int commodity::getStock(){return stock;}
int commodity::getID(){return ID;}
float commodity::getBuyValue(){return buyValue;}
float commodity::getsellValue(){return sellValue;}
char* commodity::getName(){return name;}
char* commodity::getowner(){return owner;}

bool trade::Buy(int ID, int count){
    int find = 0;
    if (find == 0)
        for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
            if (iter->getID() == ID){
                iter->UpdateStock(count);
                return true; find = 1;
            }
        }
    else if (find == 0)
        for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
            if (iter->getID() == ID){
                iter->UpdateStock(count);
                return true; find = 1;
            }
        }
    else if (find == 0)
        for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
            if (iter->getID() == ID){
                iter->UpdateStock(count);
                return true;
            }
        }
    return false;
}
bool trade::Sell(int ID, int count){
    for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
        if (iter->getID() == ID && !(iter->getStock() - count < 0)){
            iter->UpdateStock(-count);
            return true;
        }
    }
    for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
        if (iter->getID() == ID && !(iter->getStock() - count < 0)){
            iter->UpdateStock(-count);
            return true;
        }
    }
    for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
        if (iter->getID() == ID && !(iter->getStock() - count < 0)){
            iter->UpdateStock(-count);
            return true;
        }
    }
    return false;
}
void trade::AddNew(char owner[], char name[], float buyValue, float sellValue, int leibie, char introduce[])//����µ���Ʒ����
{                                                              //�̼������Ʒ
    switch (leibie) {
    case 1:dataList1.push_back(Book(owner, name, buyValue, sellValue, introduce)); break;
    case 2:dataList2.push_back(Cloth(owner, name, buyValue, sellValue, introduce)); break;
    case 3:dataList3.push_back(Food(owner, name, buyValue, sellValue, introduce)); break;
    default: break;
    }
}
void trade::GetIndex(SOCKET recvSock)                           //�õ���ƷĿ¼
{
    for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
        char ab[100] = { "\t ��Ʒ��ţ�" }; char bc[100] = { "\n ��Ʒ���ƣ� " };
        this->intab(ab, iter->getID());
        strcat(ab, bc);
        strcat(ab, iter->getName());
        getstr(recvSock, ab, bc);
        printf("\t ��Ʒ��ţ� %i   ��Ʒ���ƣ�%s\n", iter->getID(), iter->getName());
    }
    for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
        char ab[100] = { "\t ��Ʒ��ţ�" }; char bc[100] = { "\n ��Ʒ���ƣ� " };
        this->intab(ab, iter->getID());
        strcat(ab, bc);
        strcat(ab, iter->getName());
        getstr(recvSock, ab, bc);
        printf("\t ��Ʒ��ţ� %i   ��Ʒ���ƣ�%s\n", iter->getID(), iter->getName());
    }
    for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
        char ab[100] = { "\t ��Ʒ��ţ�" }; char bc[100] = { "\n ��Ʒ���ƣ� " };
        this->intab(ab, iter->getID());
        strcat(ab, bc);
        strcat(ab, iter->getName());
        getstr(recvSock, ab, bc);
        printf("\t ��Ʒ��ţ� %i   ��Ʒ���ƣ�%s\n", iter->getID(), iter->getName());
    }
}
bool trade::GetInformathion(int index, SOCKET servSock) {         //�����Ʒ�Ľ���
    for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
        if (iter->getID() == index){
            char ab[BUF_SIZE] = { "��Ʒ��ţ�\0" }, bc[BUF_SIZE];
            intab(ab, index);
            strcat(ab, "\n ��Ʒ���ƣ�\0");
            strcat(ab, iter->getName());
            strcat(ab, "\n ����۸�\0");
            floatab(ab, iter->getBuyValue());
            strcat(ab, "\n ���ۼ۸�\0");
            floatab(ab, iter->getsellValue());
            strcat(ab, "\n ʣ�ࣺ\0");
            intab(ab, iter->getStock());
            strcat(ab, "\n �̼ң�\0");
            strcat(ab, iter->getowner());
            strcat(ab, "\n ���ܣ�\0");
            strcat(ab, iter->getIntroduce());
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n ���ܣ�%s\n",
                index, iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
            return true;
        }
    }
    for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
        if (iter->getID() == index){
            char ab[BUF_SIZE] = { "��Ʒ��ţ�\0" }, bc[BUF_SIZE];
            intab(ab, index);
            strcat(ab, "\n ��Ʒ���ƣ�\0");
            strcat(ab, iter->getName());
            strcat(ab, "\n ����۸�\0");
            floatab(ab, iter->getBuyValue());
            strcat(ab, "\n ���ۼ۸�\0");
            floatab(ab, iter->getsellValue());
            strcat(ab, "\n ʣ�ࣺ\0");
            intab(ab, iter->getStock());
            strcat(ab, "\n �̼ң�\0");
            strcat(ab, iter->getowner());
            strcat(ab, "\n ���ܣ�\0");
            strcat(ab, iter->getIntroduce());
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n ���ܣ�%s\n",
                index, iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
            return true;
        }
    }
    for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
        if (iter->getID() == index){
            char ab[BUF_SIZE] = { "��Ʒ��ţ�\0" }, bc[BUF_SIZE];
            intab(ab, index);
            strcat(ab, "\n ��Ʒ���ƣ�\0");
            strcat(ab, iter->getName());
            strcat(ab, "\n ����۸�\0");
            floatab(ab, iter->getBuyValue());
            strcat(ab, "\n ���ۼ۸�\0");
            floatab(ab, iter->getsellValue());
            strcat(ab, "\n ʣ�ࣺ\0");
            intab(ab, iter->getStock());
            strcat(ab, "\n �̼ң�\0");
            strcat(ab, iter->getowner());
            strcat(ab, "\n ���ܣ�\0");
            strcat(ab, iter->getIntroduce());
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n ���ܣ�%s\n",
                index, iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
            return true;
        }
    }
    return false;
}
char* trade::Getname(int index){
    for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
        if (iter->getID() == index)
            return iter->getName();
    }
    for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
        if (iter->getID() == index)
            return iter->getName();
    }
    for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
        if (iter->getID() == index){
            return iter->getName();
        }
    }

}
char* trade::Getowner(int index){
    for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
        if (iter->getID() == index)
            return iter->getowner();
    }
    for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
        if (iter->getID() == index)
            return iter->getowner();
    }
    for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
        if (iter->getID() == index)
            return iter->getowner();
    }
}
int trade::Getleibie(int index){
    for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
        if (iter->getID() == index)
            return 1;
    }
    for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
        if (iter->getID() == index)
            return 2;
    }
    for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
        if (iter->getID() == index)
            return 3;
    }
    return 0;
}
float trade::Getmoney(int index){
    for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
        if (iter->getID() == index)
            return iter->getsellValue();
    }
    for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
        if (iter->getID() == index)
            return iter->getsellValue();
    }
    for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
        if (iter->getID() == index)
            return iter->getsellValue();
    }
    return 0;
}
int trade::Getstock(int index){
    for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
        if (iter->getID() == index)
            return iter->getStock();
        
    }
    for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
        if (iter->getID() == index)
            return iter->getStock();
    }
    for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
        if (iter->getID() == index)
            return iter->getStock();
    }
    return 0;
}

void trade::save1(){
    ofstream   file1;
    file1.open("stock1.txt");
    if (!file1.fail()){
        file1 << "=========================================================================\n";
        file1 << "                         *****��Ʒ��Ϣ*****                               \n";
        file1 << "=========================================================================\n";
        for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
            file1 << "ID:" << iter->getID()
                << "\towner:" << iter->getowner()
                << "\tNAME:" << iter->getName()
                << "\tCOST:" << iter->getBuyValue()
                << "\tVALUE:" << iter->getsellValue()
                << "\tSTOCK:" << iter->getStock()
                << "\tINTRODUCE:" << iter->getIntroduce()
                << "\n";
        }
        file1.close();
    }
    else printf("��¼�ļ�����ʧ��\n");
}
void trade::save2(){
    ofstream   file1;
    file1.open("stock2.txt");
    if (!file1.fail()){
        file1 << "=========================================================================\n";
        file1 << "                         *****��Ʒ��Ϣ*****                               \n";
        file1 << "=========================================================================\n";
        for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
            file1 << "ID:" << iter->getID()
                << "\towner:" << iter->getowner()
                << "\tNAME:" << iter->getName()
                << "\tCOST:" << iter->getBuyValue()
                << "\tVALUE:" << iter->getsellValue()
                << "\tSTOCK:" << iter->getStock()
                << "\tINTRODUCE:" << iter->getIntroduce()
                << "\n";
        }
        file1.close();
    }
    else printf("��¼�ļ�����ʧ��\n");
}
void trade::save3(){
    ofstream   file1;
    file1.open("stock3.txt");
    if (!file1.fail()){
        file1 << "=========================================================================\n";
        file1 << "                         *****��Ʒ��Ϣ*****                               \n";
        file1 << "=========================================================================\n";
        for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
            file1 << "ID:" << iter->getID()
                << "\towner:" << iter->getowner()
                << "\tNAME:" << iter->getName()
                << "\tCOST:" << iter->getBuyValue()
                << "\tVALUE:" << iter->getsellValue()
                << "\tSTOCK:" << iter->getStock()
                << "\tINTRODUCE:" << iter->getIntroduce()
                << "\n";
        }
        file1.close();
    }
    else printf("��¼�ļ�����ʧ��\n");
}
bool trade::init1(){
    ifstream file3("stock1.txt");
    if (!file3.fail()){
        char titles[CAP][TLEN] = { "ID:","owner:" ,"NAME:","COST:","VALUE:","STOCK:","INTRODUCE:" };
        char saves[CAP][ALEN] = {};
        int tIndex = 0;
        char buf[128];
        int kinds = 0;
        for (int i = 0; i < 3; i++)//���Ա���
            file3.getline(buf, 128);
        while (!file3.eof()){
            char temp[TLEN] = "";//��ȡ�ļ����ݵ��ַ�����
            for (int i = 0; i < TLEN && !file3.eof(); i++)//for NO.1
            {
                file3.get(temp[i]);
                if (strcmp(temp, titles[tIndex]) == 0){
                    for (int j = 0; j < ALEN && !file3.eof(); j++)//NO.2
                    {
                        char c;
                        file3.get(c);
                        if (c != '\t' && c != '\n'){
                            saves[tIndex][j] = c;
                            saves[tIndex][j + 1] = '\0';
                        }
                        else if (c != '\n'){
                            if (tIndex > 6)                      
                                return false;                           
                            tIndex++;
                            break;
                        }
                        else {
                            dataList1.push_back(Book(atoi(saves[0]), saves[1], saves[2], atof(saves[3]), atof(saves[4]), atoi(saves[5]), saves[6]));
                            tIndex = 0; kinds++;
                            break;
                        }
                        if (j == ALEN - 1)
                            return false;//�����������ȣ���ʼ��ʧ��
                    }
                    break;
                }
                if (i == TLEN - 1)
                    return false;//δƥ�䵽�������ƣ�ʧ��
            }
        }
        file3.close();
        commodity::kinds = kinds;
        return true;
    }
    return false;//�ļ�������
}
bool trade::init2(){
    ifstream file4("stock2.txt");
    if (!file4.fail()){
        char titles[CAP][TLEN] = { "ID:","owner:" ,"NAME:","COST:","VALUE:","STOCK:" ,"INTRODUCE:" };
        char saves[CAP][ALEN] = {};
        int tIndex = 0;
        char buf[128];
        int kinds = 0;
        for (int i = 0; i < 3; i++)//���Ա���
            file4.getline(buf, 128);
        while (!file4.eof()){
            char temp[TLEN] = "";//��ȡ�ļ����ݵ��ַ�����
            for (int i = 0; i < TLEN && !file4.eof(); i++)//for NO.1
            {
                file4.get(temp[i]);
                if (strcmp(temp, titles[tIndex]) == 0){
                    for (int j = 0; j < ALEN && !file4.eof(); j++)//NO.2
                    {
                        char c;
                        file4.get(c);
                        if (c != '\t' && c != '\n') {
                            saves[tIndex][j] = c; 
                            saves[tIndex][j + 1] = '\0';
                        }                          
                        else if (c != '\n'){
                            if (tIndex > 6)
                                return false;
                            tIndex++;
                            break;
                        }
                        else {
                            dataList2.push_back(Cloth(atoi(saves[0]), saves[1], saves[2], atof(saves[3]), atof(saves[4]), atoi(saves[5]), saves[6]));
                            tIndex = 0; kinds++;
                            break;
                        }
                        if (j == ALEN - 1)
                            return false;//�����������ȣ���ʼ��ʧ��
                    }
                    break;
                }
                if (i == TLEN - 1)
                    return false;//δƥ�䵽�������ƣ�ʧ��
            }
        }
        file4.close();
        commodity::kinds += kinds;
        return true;
    }
    return false;//�ļ�������
}
bool trade::init3(){
    ifstream file5("stock3.txt");
    if (!file5.fail()){
        char titles[CAP][TLEN] = { "ID:","owner:" ,"NAME:","COST:","VALUE:","STOCK:" ,"INTRODUCE:" };
        char saves[CAP][ALEN] = {};
        int tIndex = 0;
        char buf[128];
        int kinds = 0;
        for (int i = 0; i < 3; i++)//���Ա���
            file5.getline(buf, 128);
        while (!file5.eof()){
            char temp[TLEN] = "";//��ȡ�ļ����ݵ��ַ�����
            for (int i = 0; i < TLEN && !file5.eof(); i++)//for NO.1
            {
                file5.get(temp[i]);
                if (strcmp(temp, titles[tIndex]) == 0){
                    for (int j = 0; j < ALEN && !file5.eof(); j++)//NO.2
                    {
                        char c;
                        file5.get(c);
                        if (c != '\t' && c != '\n'){
                            saves[tIndex][j] = c; 
                            saves[tIndex][j + 1] = '\0';
                        }
                        else if (c != '\n'){
                            if (tIndex > 6)                          
                                return false;
                            tIndex++;
                            break;
                        }
                        else {
                            dataList3.push_back(Food(atoi(saves[0]), saves[1], saves[2], atof(saves[3]), atof(saves[4]), atoi(saves[5]), saves[6]));
                            tIndex = 0; kinds++;
                            break;
                        }
                        if (j == ALEN - 1)
                            return false;//�����������ȣ���ʼ��ʧ��
                    }
                    break;
                }
                if (i == TLEN - 1)
                    return false;//δƥ�䵽�������ƣ�ʧ��
            }
        }
        file5.close();
        commodity::kinds += kinds;
        return true;
    }
    return false;//�ļ�������
}

int shoppingsystem::mainsystem(SOCKET& servSock)
{
    char ab[BUF_SIZE] = { "!\0" };
    char bc[BUF_SIZE] = { "!\0" };
    trade myTrade; int select0;
    if (!myTrade.init())
        myTrade = trade();
    bool quitFlag = false;
    system("cls");
    while (!quitFlag){
        this->DrawMainMenu(servSock);
        printf("����������ѡ��:\0");
        select0 = getint(servSock, bc);
        switch (select0){
        case 2://�鿴
            this->drawIndex(servSock);
            myTrade.GetIndex(servSock);
            cout << "��ѡ����ϸ�鿴����Ʒ��ţ�" << endl;
            int index; //char bc[100] = { "��ѡ����ϸ�鿴����Ʒ���:\0" };
            strcpy_s(bc, "��ѡ����ϸ�鿴����Ʒ���:\0");
            getstr(servSock, bc, ab);
            index = getint(servSock, ab);
            if (!myTrade.GetInformathion(index, servSock)) {
                char ab[100] = { "��Ч����Ʒ���\0" };
                cout << "��Ч����Ʒ���" << endl; this->getstr(servSock, ab, bc);
                this->operate(servSock); break;
            }
            this->operate(servSock);
            break;
        case 3://����
            myTrade.search(servSock);
            this->operate(servSock);
            break;
        case 0://�˳�
            quitFlag = true;
            break;
        case 1://��¼
            this->login(myTrade, servSock);
            if (this->log == 0) break;
            else   if (this->log == 1)  this->custsystem(myTrade, servSock);
            else   if (this->log == 2) this->merchantsystem(myTrade, servSock);
            if (this->log != 0)
                quitFlag = true;
            this->operate(servSock);
            break;
        case 4://�����˺�
            this->createaccount(servSock);
            this->operate(servSock);
            break;
        default:
        {
            system("cls");
            char ab[100] = { "��Ч��ѡ��\0" };
            // char bc[3] = { "!\0" };
            getstr(servSock, ab, bc);
            cout << "��Ч��ѡ��" << endl;
            this->operate(servSock);
        }
        }
    }
    myTrade.save();
    this->save1();
    this->save2();
    return 0;
}
int shoppingsystem::custsystem(trade& myTrade, SOCKET& servSock){
    char ab[BUF_SIZE] = { "\0" };
    char bc[1000] = { "\0" };
    bool quitFlag1 = false; int select1;
    while (!quitFlag1){
        this->DrawMenu1(servSock);
        printf("����������ѡ�");
        bc[0] = '\0';
        select1 = getint(servSock, bc);
        //cin >> select1; getchar();
        switch (select1){
        case 2://�鿴
            drawIndex(servSock);
            myTrade.GetIndex(servSock);
            strcpy_s(ab, "��ѡ����ϸ�鿴����Ʒ���:\0");
            getstr(servSock, ab, bc);
            cout << "��ѡ����ϸ�鿴����Ʒ��ţ�" << endl;
            int index;
            ab[1] = '\0', ab[0] = '!';
            index = getint(servSock, bc);
            if (!myTrade.GetInformathion(index, servSock)) {
                cout << "��Ч����Ʒ���" << endl;
                strcpy_s(ab, "��Ч����Ʒ���\0");
                getstr(servSock, ab, bc);
            }
            this->operate(servSock);
            break;
        case 3://����
            myTrade.search(servSock);
            this->operate(servSock);
            break;
        case 0://�˳�
            quitFlag1 = true;
            break;
        case 1://�˳���¼
            this->logout(myTrade, servSock); quitFlag1 = true;
            this->operate(servSock);
            break;
        case 4://�޸���Ϣ
            this->information(myTrade, servSock);
            this->operate(servSock);
            break;
        case 5://���ﳵ
            this->shoppingcar(myTrade, servSock);
            this->operate(servSock);
            break;
        default:
            system("cls");
            cout << "��Ч��ѡ��" << endl;
            strcpy_s(ab, "��Ч��ѡ��\0");
            getstr(servSock, ab, bc);
            this->operate(servSock);
        }
    }
    return select1;
}
int shoppingsystem::shoppingcar(trade& myTrade, SOCKET& servSock)//���ﳵ����
{
    list<order> car;
    list<order> ding;
    bool quit = false;
    while (!quit){
        char ab[BUF_SIZE] = { "0.�˳�\n"
            "1.�����Ʒ�����ﳵ\n"
             "2.�����ﳵ\n"
             "3.���ɶ���\n"
             "4.֧������\n"
             "����������Ĳ���:\0" };
        char bc[1000] = { "\0" };
        cout << "0.�˳�\n"
            << "1.�����Ʒ�����ﳵ\n"
            << "2.�����ﳵ\n"
            << "3.���ɶ���\n"
            << "4.֧������\n"
            << "����������Ĳ�����\n"
            << endl;
        getstr(servSock, ab, bc);
        int select; bc[0] = '\0';
        select = getint(servSock, bc);
        switch (select)
        {
        case 0://�˳�
            quit = true;
            break;
        case 1://�����Ʒ�����ﳵ��
        {
            myTrade.GetIndex(servSock);
            cout << "��������빺�ﳵ����ƷID��" << endl;
            strcpy_s(ab, "��������빺�ﳵ����ƷID:\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            int ID, amount;
            ID = getint(servSock, bc);
            int times = 0;
            //        cin >> ID; getchar();
            while (myTrade.Getit(ID) == 0 && times < 5) {
                cout << "��������ȷ�ı�ţ�" << endl;
                strcpy_s(ab, "��������ȷ�ı��:\0"); getstr(servSock, ab, bc);
                //cin >> ID; getchar();
                bc[0] = '\0';
                ID = getint(servSock, bc);
                times++;
            }
            if (times >= 5) break;
            cout << "�������⹺�����Ʒ������" << endl;
            strcpy_s(ab, "�������⹺�����Ʒ����:\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            bc[0] = '\0';
            amount = getint(servSock, bc);
            //cin >> amount;
            //getchar();
            car.push_back(order(myTrade.Getleibie(ID), ID, myTrade.Getname(ID), myTrade.Getowner(ID), amount, amount * myTrade.Getmoney(ID)));
            cout << "���빺�ﳵ�ɹ���" << endl; strcpy(ab, "���빺�ﳵ�ɹ�\0"); getstr(servSock, ab, bc);
            this->operate(servSock); break;
        }
        case 2://�����ﳵ
        {
            for (auto iter = car.begin(); iter != car.end(); iter++)
            {
                if (iter->count) {
                    strcpy(ab, "ID:\0");
                    intab(ab, iter->ID);
                    strcat(ab, ", name:\0 ");
                    strcat(ab, iter->name);
                    strcat(ab, ", ���\0");
                    intab(ab, iter->type);
                    strcat(ab, ", amount:\0 ");
                    intab(ab, iter->count);
                    strcat(ab, ", �۸�\0");
                    floatab(ab, iter->tolmoney);
                    getstr(servSock, ab, bc);
                    printf("ID:%d, name:%s, ���%d, amount:%d, �۸�:%f\n", iter->ID, iter->name, iter->type, iter->count, iter->tolmoney);
                }
            }
            int action;
            bool quit1 = false;
            while (!quit1)
            {
                cout << "0.�˳�\n"
                    << "1.�ӹ��ﳵ��ɾ��"
                    << "2.��������"
                    << "����������Ĳ�����\n" << endl;
                strcpy_s(ab, "\t0.�˳�\n""\t1.�ӹ��ﳵ��ɾ��\n""\t2.��������\n""\t����������Ĳ���:\0");
                bc[0] = '\0';
                getstr(servSock, ab, bc);
                bc[0] = '\0';
                action = getint(servSock, bc);
                switch (action)
                {
                case 0:
                    quit1 = true;
                    break;
                case 1://�ӹ��ﳵ��ɾ��
                {
                    for (auto iter = car.begin(); iter != car.end(); iter++){
                        if (iter->count) {
                            strcpy(ab, "ID:\0");
                            intab(ab, iter->ID);
                            strcat(ab, ", name: ");
                            strcat(ab, iter->name);
                            strcat(ab, ", ���");
                            intab(ab, iter->type);
                            strcat(ab, ", amount: ");
                            intab(ab, iter->count);
                            strcat(ab, ", �۸�");
                            floatab(ab, iter->tolmoney);
                            getstr(servSock, ab, bc);
                            printf("ID:%d, name:%s, ���%d, amount:%d, �۸�:%f\n", iter->ID, iter->name, iter->type, iter->count, iter->tolmoney);
                        }
                    }
                    cout << "��ѡ��ɾ������Ʒ���:" << endl;
                    strcpy_s(ab, "��ѡ��ɾ������Ʒ���:\0"); getstr(servSock, ab, bc);
                    int a;    bc[0] = '\0';
                    a = getint(servSock, bc);
                    //cin >> a; getchar();
                    for (auto iter = car.begin(); iter != car.end(); iter++)
                    {
                        if (iter->ID == a)
                            iter->count = 0, iter->tolmoney = 0;
                    }
                    cout << "��ɾ��" << endl; strcpy_s(ab, "��ɾ��\0"); getstr(servSock, ab, bc);
                    this->operate(servSock); break;
                }
                case 2://������Ʒ����
                {
                    for (auto iter = car.begin(); iter != car.end(); iter++){
                        if (iter->count) {
                            strcpy(ab, "ID:\0");
                            intab(ab, iter->ID);
                            strcat(ab, ", name: ");
                            strcat(ab, iter->name);
                            strcat(ab, ", ���");
                            intab(ab, iter->type);
                            strcat(ab, ", amount: ");
                            intab(ab, iter->count);
                            strcat(ab, ", �۸�");
                            floatab(ab, iter->tolmoney);
                            getstr(servSock, ab, bc);
                            printf("ID:%d, name:%s, ���%d, amount:%d, �۸�:%f\n", iter->ID, iter->name, iter->type, iter->count, iter->tolmoney);
                        }            
                    }
                    cout << "��ѡ��������������Ʒ���:" << endl;
                    strcpy_s(ab, "��ѡ��������������Ʒ���:\0"); getstr(servSock, ab, bc);
                    int b, find; find = 0;
                    b = getint(servSock, bc);
                    for (auto iter = car.begin(); iter != car.end(); iter++){
                        if (iter->ID == b){
                            int c; 
                            cout << "����������������(.../-..." << endl;
                            strcpy_s(ab, "����������������(.../-...):\0"); 
                            getstr(servSock, ab, bc);
                            c = getint(servSock, bc);
                            if (iter->count + c >= 0) {
                                iter->count += c;
                                iter->tolmoney += c * (myTrade.GetGoodsPrice(iter->name));
                                cout << "�Ѹı�" << endl; strcpy_s(ab, "�Ѹı�\0");
                                getstr(servSock, ab, bc);
                            }
                            else {
                                cout << "�����������󣬸ı�ʧ��" << endl;
                                strcpy_s(ab, "�����������󣬸ı�ʧ��\0");
                                getstr(servSock, ab, bc);

                            }
                            find = 1;
                        }
                    }
                    if (find == 0) {
                        cout << "���ﳵ��û�д���Ʒ���������ٽ��й���" << endl;
                        strcpy_s(ab, "���ﳵ��û�д���Ʒ���������ٽ��й���\0");
                        getstr(servSock, ab, bc);
                    }
                    this->operate(servSock);
                    break;
                }
                default:
                    system("cls");
                    cout << "��Ч��ѡ��" << endl;
                    this->operate(servSock);
                }
            }
            auto iter = car.begin();
            for (; iter != car.end();){
                if (iter->count == 0)  
                    iter = car.erase(iter);
                else 
                    iter++;
            }
            this->operate(servSock);
            break;
        }
        case 3://���ɶ���
        {
            int out = 0;
            orderrecord record1;
            record1.Goodsshu = 0;
            auto iter1 = car.begin();
            auto iter2 = ding.begin();
            auto riter = record.begin();
            if (car.empty() == 0) {
                for (int i = 0; iter1 != car.end() && out == 0; i++, iter1++){
                    if (this->amountofrecords(iter1->ID) + iter1->count > myTrade.Getstock(iter1->ID)){
                        strcpy_s(ab, "���ɶ���ʧ�ܣ���Ʒ��ţ�\0");
                        intab(ab, iter1->ID);
                        strcpy(ab, " ����������棬���޸Ķ�����\0");
                        bc[0] = '\0';
                        getstr(servSock, ab, bc);
                        cout << "���ɶ���ʧ�ܣ���Ʒ��ţ�" << iter1->ID << " ����������棬���޸Ķ�����\n" << endl;
                        out = 1;
                    }
                    else {
                        strcpy_s(record1.customer, myTrade.theuser);
                        strcpy_s(record1.owner[i], iter1->owner);
                        record1.Goodsamount[i] = iter1->count;
                        record1.Goodsmoney[i] = iter1->tolmoney;
                        record1.Goodsshu += 1;
                        record1.GoodsID[i] = iter1->ID;
                    }
                }
                if (out == 0){
                    int i = 1;
                    for (riter = record.begin(); riter != record.end(); riter++) i++;
                    record1.dingID = i;
                    record.push_back(record1);
                    record1.showrecord(servSock, myTrade);
                    cout << "���ɶ����ɹ�,�������Ϊ��" << record1.dingID << endl;
                    strcpy_s(ab, "���ɶ����ɹ�,�������Ϊ��\0"); intab(ab, record1.dingID);
                    getstr(servSock, ab, bc);
                }
            }
            else {
                cout << "���ڹ��ﳵ�������Ʒ" << endl;
                strcpy_s(ab, "���ڹ��ﳵ�������Ʒ\0"); //intab(ab, record1.dingID);
                getstr(servSock, ab, bc);
            }
            this->operate(servSock);
            break;
        }
        case 4://֧������
        {
            if (record.empty()){
                strcpy_s(ab, "�޶���\0");
                bc[0] = '\0';
                getstr(servSock, ab, bc);
                cout << "�޶���" << endl;
            }
            else{
                cout << "ѡ��Ҫ֧���Ķ�����" << myTrade.theuser << endl;
                for (auto iter = record.begin(); iter != record.end(); iter++)
                    if (strcmp(iter->customer, myTrade.theuser) == 0){
                        itoa(iter->dingID, ab, 10); bc[0] = '\0';
                        getstr(servSock, ab, bc);
                        cout << iter->dingID << endl;
                        iter->showrecord(servSock, myTrade);
                    }
                strcpy_s(ab, "ѡ��Ҫ֧���Ķ���:\0");
                getstr(servSock, ab, bc);
                int choice, out = 0;
                char bc[1000] = { '\0' };
                ab[0] = '\0';
                choice = getint(servSock, ab);
                int fail = 0;
                for (auto iter = record.begin(); iter != record.end() && out == 0; iter++)
                    if (strcmp(iter->customer, myTrade.theuser) == 0 && choice == iter->dingID){
                        out = 1;
                        for (auto it = custList.begin(); fail == 0 && it != custList.end(); it++){
                            if ((strcmp(it->username, iter->customer) == 0)){
                                if (it->money < iter->gettotal()) {
                                    fail = 1;
                                    strcpy_s(ab, "���㣬֧��ʧ��\0"); bc[0] = '\0';
                                    getstr(servSock, ab, bc);
                                    cout << "���㣬֧��ʧ��" << endl;
                                }
                                else{
                                    it->money -= iter->gettotal();
                                    for (int j = 0; j < iter->Goodsshu; j++){
                                        for (auto iter1 = merList.begin(); iter1 != merList.end(); iter1++){
                                            if (strcmp(iter->owner[j], iter1->username) == 0){
                                                iter1->money += iter->Goodsmoney[j];
                                                myTrade.Updatestock(iter->GoodsID[j], -iter->Goodsamount[j]);
                                                strcpy_s(ab, "֧���ɹ�\0"); bc[0] = '\0';
                                                getstr(servSock, ab, bc);
                                                cout << "֧���ɹ�" << endl;
                                            }
                                        }
                                    }
                                    fail = 2; iter->state = 1;
                                }
                            }
                        }
                    }
            }
            this->removerecord();
            this->operate(servSock);
            break;
        }
        default:
            system("cls");
            cout << "��Ч��ѡ��" << endl;
            strcpy_s(ab, "��Ч��ѡ��\0");
            getstr(servSock, ab, bc);
            this->operate(servSock);
        }
    }
    return 0;
}
int shoppingsystem::merchantsystem(trade& myTrade, SOCKET& servSock)//�̼�ϵͳ
{
    bool quitFlag2 = false;
    int select2;
    char ab[BUF_SIZE] = { "\0" };
    char bc[1000] = { "\0" };
    while (!quitFlag2){
        this->DrawMenu2(servSock);
        printf("����������ѡ�");
        select2 = getint(servSock, bc);
        switch (select2){
        case 2://�鿴
        {
            this->drawIndex(servSock);
            myTrade.GetIndex(servSock);
            strcpy_s(ab, "��ѡ����ϸ�鿴����Ʒ���:\0");
            getstr(servSock, ab, bc);
            cout << "��ѡ����ϸ�鿴����Ʒ��ţ�" << endl;
            int index;
            //cin >> index; getchar();
            bc[0] = '\0';
            index = getint(servSock, bc);
            system("cls");
            if (!myTrade.GetInformathion(index, servSock)) {
                cout << "��Ч����Ʒ���" << endl;
                strcpy_s(ab, "��Ч����Ʒ���\0");
                getstr(servSock, ab, bc);
                this->operate(servSock);
            }
            else 
                this->operate(servSock);
            break;
        }
        case 3://����
            myTrade.search(servSock);
            operate(servSock);  //system("cls");
            break;
        case 0://�˳�
            quitFlag2 = true;
            break;
        case 1://�˳���¼
            this->logout(myTrade, servSock); quitFlag2 = true;
            this->operate(servSock); //system("cls");
        case 4://�޸���Ϣ
        {
            this->information1(myTrade, servSock);
            this->operate(servSock); //system("cls");
            break; }
        case 5://������Ʒ
        {   
            char name[30];
            for (auto iter = merList.begin(); iter != merList.end(); iter++)
                if (iter->state && strcmp(myTrade.theuser, iter->username) == 0) 
                    strcpy_s(myTrade.theuser, iter->username);
            myTrade.manage(name, servSock);
            this->operate(servSock);
            break;
        }
        case 6://����
        {
            char name[30];
            strcpy_s(name, myTrade.theuser); cout << name << endl;
            char shang[30]; char introduces[30];
            float values;
            float costs;
            int lei;
            cout << "��������Ʒ����:\0" << endl;
            strcpy_s(ab, "��������Ʒ����:\0");
            getstr(servSock, ab, bc);
            getstr(servSock, bc, shang); cout << shang;
            cout << "����۸�:\0" << endl;
            strcpy_s(ab, "����۸�:\0");
            getstr(servSock, ab, bc);
            ab[0] = '!', ab[1] = '\0';
            getstr(servSock, ab, bc);
            costs = atof(bc); cout << costs << bc;
            cout << "�����۸�:\0" << endl;
            strcpy_s(ab, "�����۸�:\0");
            getstr(servSock, ab, bc);
            ab[0] = '!', ab[1] = '\0'; bc[0] = '\0';
            getstr(servSock, ab, bc);
            values = atof(bc); cout << values << bc;
            cout << "����������:\0" << endl;
            strcpy_s(ab, "����������:\0");
            getstr(servSock, ab, bc);
            getstr(servSock, bc, introduces); cout << introduces;
            cout << "���������:\0" << endl;
            strcpy_s(ab, "���������:\0");
            getstr(servSock, ab, bc);
            lei = getint(servSock, bc); cout << lei;
            myTrade.AddNew(name, shang, costs, values, lei, introduces);
            cout << "�������\0" << endl;
            strcpy_s(ab, "�������\0");
            getstr(servSock, ab, bc);
            this->operate(servSock);
            break;
        }
        default:
            system("cls");
            cout << "��Ч��ѡ��" << endl;
            strcpy_s(ab, "��Ч��ѡ��");
            getstr(servSock, ab, bc);
            this->operate(servSock);
        }
    }
    return select2;
}
void trade::manage(char name[], SOCKET& servSock)//������̼ҹ���
{
    char ab[BUF_SIZE];
    char bc[1000];
    bool quitFlag2 = false;
    int select2;
    while (!quitFlag2){
        strcpy_s(ab, "0.�����������\n 1.������Ʒ���\n 2.������Ʒ�۸�\n 3.Ʒ�����\n 4.�鿴��Ʒ\n����������ѡ��:\0");
        getstr(servSock, ab, bc);
        printf("����������ѡ�\n0.�����������\n 1.������Ʒ���\n 2.������Ʒ�۸�\n 3.Ʒ�����\n 4.�鿴��Ʒ\n");// cin >> select2; getchar();
        bc[0] = '\0';
        select2 = this->getint(servSock, bc);
        switch (select2)
        {
        case 4://�鿴��Ʒ
        {
            this->drawIndex(servSock);
            for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
                if (strcmp(this->theuser, iter->getowner()) == 0) {
                    newstr(ab, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
                    getstr(servSock, ab, bc);
                    printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n",
                        iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner());
                }
            }
            for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
                if (strcmp(this->theuser, iter->getowner()) == 0) {
                    newstr(ab, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
                    getstr(servSock, ab, bc);
                    printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n",
                        iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner());
                }
            }
            for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
                if (strcmp(this->theuser, iter->getowner()) == 0) {
                    newstr(ab, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
                    getstr(servSock, ab, bc);
                    printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n",
                        iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner());
                }
            }
            cout << "��ѡ����ϸ�鿴����Ʒ��ţ�" << endl; 
            strcpy_s(ab, "��ѡ����ϸ�鿴����Ʒ���:\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            int index; bc[0] = '\0';
            index = getint(servSock, bc);
            if (!this->GetInformathion(index, servSock)) {
                cout << "��Ч����Ʒ���" << endl;
                strcpy(ab, "��Ч����Ʒ���\0");
                getstr(servSock, ab, bc);
                this->operate(servSock);
            }
            else 
                this->operate(servSock);
            break;
        }
        case 3://Ʒ�����
        {
            cout << "ѡ����۵�Ʒ�ࣺ1.Book 2.Cloth 3.Food" << endl;
            strcpy_s(ab, "\t1.Book 2.Cloth 3.Foodѡ����۵�Ʒ��:\0"); bc[0] = '\0';
            getstr(servSock, ab, bc);
            int choice; //cin >> choice; getchar(); 
            bc[0] = '\0';
            choice = getint(servSock, bc);
            bc[0] = '\0';
            cout << "���������ۿۣ�" << endl;
            strcpy_s(ab, "���������ۿ�:\0");
            getstr(servSock, ab, bc);
            ab[0] = '\0'; bc[0] = '\0';
            getstr(servSock, ab, bc);
            float discount = atof(bc); 
            cout << discount << "!" << bc;
            if (choice == 1) for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
                float value = iter->sellValue;
                if (strcmp(this->theuser, iter->getowner()) == 0) 
                    iter->sellValue = discount * value, iter->price = discount * value;
            }
            if (choice == 2) for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++)
            {
                float value = iter->sellValue;
                if (strcmp(this->theuser, iter->getowner()) == 0) 
                    iter->sellValue = discount * value, iter->price = discount * value;
            }
            if (choice == 3) for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++)
            {
                float value = iter->sellValue;
                if (strcmp(this->theuser, iter->getowner()) == 0) 
                    iter->sellValue = discount * value, iter->price = discount * value;
            }
            cout << "�����ɹ�" << endl; strcpy_s(ab, "�����ɹ�\0"); bc[0] = '\0'; getstr(servSock, ab, bc);
            this->operate(servSock);
            break;
        }
        case 0://�˳�
            quitFlag2 = true;
            break;
        case 1://������Ʒ���
        {
            cout << "��������¿�����Ʒ��ţ�" << endl;
            strcpy_s(ab, "��������¿�����Ʒ���:\0");
            getstr(servSock, ab, bc);
            int _ID;
            _ID = getint(servSock, bc);
            // cin >> _ID; getchar();
            for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++)
            {
                if (_ID == iter->getID())
                {
                    cout << "������������Ʒ��������" << endl; strcpy_s(ab, "������������Ʒ������:\0");
                    getstr(servSock, ab, bc);
                    int amount;
                    amount = getint(servSock, bc);
                    //cin >> amount; getchar();
                    iter->UpdateStock(amount); cout << "�ı����Ϊ��" << iter->getStock() << endl;
                    strcpy_s(ab, "�ı����Ϊ��");
                    intab(ab, iter->getStock());
                    getstr(servSock, ab, bc);
                }
            }
            for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++)
            {
                if (_ID == iter->getID())
                {
                    cout << "������������Ʒ��������" << endl; strcpy_s(ab, "������������Ʒ������:\0");
                    getstr(servSock, ab, bc);
                    int amount;
                    amount = getint(servSock, bc);
                    iter->UpdateStock(amount); cout << "�ı����Ϊ��" << iter->getStock() << endl;
                    strcpy_s(ab, "�ı����Ϊ��");
                    intab(ab, iter->getStock());
                    getstr(servSock, ab, bc);
                }
            }
            for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++)
            {
                if (_ID == iter->getID())
                {
                    cout << "������������Ʒ��������" << endl; strcpy_s(ab, "������������Ʒ������:\0");
                    getstr(servSock, ab, bc);
                    int amount;
                    amount = getint(servSock, bc);
                    //cin >> amount; getchar();
                    iter->UpdateStock(amount); cout << "�ı����Ϊ��" << iter->getStock() << endl;
                    strcpy_s(ab, "�ı����Ϊ��");
                    intab(ab, iter->getStock());
                    getstr(servSock, ab, bc);
                }
            }
            this->operate(servSock);
            break;
        }

        case 2://������Ʒ�۸�
        {
            cout << "��������¼۸����Ʒ��ţ�" << endl; strcpy_s(ab, "��������¼۸����Ʒ���:\0");
            getstr(servSock, ab, bc);
            int ID_ = getint(servSock, bc); //cin >> ID_; getchar();
            for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++)
            {
                if (ID_ == iter->getID())
                {
                    cout << "��������Ʒ�ļ۸�" << endl;
                    strcpy_s(ab, "��������Ʒ�ļ۸�:\0");
                    getstr(servSock, ab, bc);
                    float amount;
                    ab[0] = '!', ab[1] = '\0';
                    getstr(servSock, ab, bc);
                    amount = atof(bc);
                    iter->sellValue = amount; cout << "�ı��۸�Ϊ��" << iter->getsellValue() << endl;
                    strcpy_s(ab, "�ı��۸�Ϊ��\0"); floatab(ab, iter->getsellValue());
                    getstr(servSock, ab, bc);
                }
            }
            for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++)
            {
                if (ID_ == iter->getID())
                {
                    cout << "��������Ʒ�ļ۸�" << endl;
                    strcpy_s(ab, "��������Ʒ�ļ۸�:\0");
                    getstr(servSock, ab, bc);
                    float amount;
                    ab[0] = '!', ab[1] = '\0';
                    getstr(servSock, ab, bc);
                    amount = atof(bc);
                    iter->sellValue = amount; cout << "�ı��۸�Ϊ��" << iter->getPrice() << endl;
                    strcpy_s(ab, "�ı��۸�Ϊ��\0"); floatab(ab, iter->getsellValue());
                    getstr(servSock, ab, bc);
                }
            }
            for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++)
            {
                if (ID_ == iter->getID())
                {
                    cout << "��������Ʒ�ļ۸�" << endl;
                    strcpy_s(ab, "��������Ʒ�ļ۸�:\0");
                    getstr(servSock, ab, bc);
                    float amount;
                    ab[0] = '!', ab[1] = '\0';
                    getstr(servSock, ab, bc);
                    amount = atof(bc);
                    iter->sellValue = amount; cout << "�ı��۸�Ϊ��" << iter->getPrice() << endl;
                    strcpy_s(ab, "�ı��۸�Ϊ��\0"); floatab(ab, iter->getsellValue());
                    getstr(servSock, ab, bc);
                }
            }
            this->operate(servSock);
            break;
        }
        default:
            system("cls");
            cout << "��Ч��ѡ��" << endl; strcpy_s(ab, "��Ч��ѡ��\0");
            this->operate(servSock);
        }
    }
}
void shoppingsystem::mermanage(trade& myTrade, SOCKET& servSock)
{
    char name[30];
    for (auto iter = merList.begin(); iter != merList.end(); iter++)
        if (iter->state)strcpy_s(name, iter->username);
    myTrade.manage(name, servSock);
}

int main() {                                                  //������������socket������ͻ��˽�������
    shoppingsystem mysystem;
    if (!mysystem.init()) {
        cout << "�޳�ʼ��" << endl;
        mysystem = shoppingsystem();
    }
    WSADATA wsaData;
    int initial_flag;
    initial_flag = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (initial_flag != 0) {
        cout << "SOCKET��ʼ��ʧ�ܣ�" << endl;
        return 0;
    }
    SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);        //�����׽���
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));                   //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;                            //ʹ��IPv4��ַ
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");        //�����IP��ַ
    sockAddr.sin_port = htons(1234);                          //�˿�
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));   //���׽���
    listen(servSock, 20);                                     //�������״̬
    mysystem.mainsystem(servSock);
    closesocket(servSock);                                    //�ر��׽���
    WSACleanup();                                             //��ֹ DLL ��ʹ��
    return 0;
}