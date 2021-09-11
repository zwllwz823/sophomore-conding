#pragma once
#pragma warning(disable : 4996)
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

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
{                                                 //交易类，处理商品交易
private:
    list<Book> dataList1;                                     //商品数据链表
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
    int getint(SOCKET servSock, char ab[])                    //从客户端接受整型，并发去下一步指示
    {
        SOCKADDR clntAddr;
        int notsend = 0;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //接收客户端请求

        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);      //接收客户端发来的数据
        if (strlen(ab) == 0 || strcmp(ab, "!\0") == 0) ab[0] = '!', ab[1] = '\0', notsend = 1, cout << "只接受";
        //puts(ab);
        cout << "from client:"; puts(buffer);
        int wrong = 0;
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (buffer[i] > '9' || buffer[i] < '0')
                wrong = 1;
            if (wrong == 1 && buffer[0] == '-' && i == 0) wrong = 0;//-号情况
        }
        int n = atoi(buffer);
        //puts(ab);

        char bc[100] = { "数据已接受\0" };
        if (notsend == 0)
            strcpy(buffer, ab);
        else
            strcpy(buffer, bc);
        //puts(buffer);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                      //将数据返回
        closesocket(clntSock);                                  //关闭套接字
        memset(buffer, 0, BUF_SIZE);                            //重置缓冲区
        return n;
    }
    void getstr(SOCKET servSock, char ab[], char recvstr[])    //从客户端接受字符串，并发去下一步指示
    {
        SOCKADDR clntAddr;
        int notsend = 0;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        if (strlen(ab) == 0 || strcmp(ab, "!\0") == 0)
            notsend = 1, ab[0] = '!', ab[1] = '\0', cout << "只进行接受\n" << endl;
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //接收客户端请求
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);       //接收客户端发来的数据
        cout << "from client:"; puts(buffer); strcpy(recvstr, buffer);
        //int n = atoi(buffer);
        char bc[100] = { "数据已接受\0" };
        if (notsend == 0)
            strLen = strlen(ab), send(clntSock, ab, strLen, 0);
        else
            strLen = strlen(bc), send(clntSock, buffer, strLen, 0);  //将数据返回

        closesocket(clntSock);                                   //关闭套接字
        memset(buffer, 0, BUF_SIZE);                             //重置缓冲区
        //return buffer;
    }
    void intab(char ab[], int a)                                //在字符串ab后添加整数；
    {
        char sen[10]; itoa(a, sen, 10);
        strcat(ab, sen);
    }
    void floatab(char ab[], float a)                            //在字符串ab后添加小数；
    {
        char sen[1000] = { '\0' }; gcvt(a, 8, sen);
        strcat(ab, sen);
    }
    void senddrawing(SOCKET servSock, char ab[])
    {
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //接收客户端请求
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);  //接收客户端发来的数据
        puts(buffer);
        puts(ab);
        strcpy_s(buffer, ab);// puts(buffer);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                      //将数据原样返回

        closesocket(clntSock);                                  //关闭套接字
        memset(buffer, 0, BUF_SIZE);                            //重置缓冲区
    }
    char theuser[30];                                           //登录的用户名
    void operate(SOCKET& servSock)
    {

        cout << "请按任意键继续" << endl;
        char ab[BUF_SIZE] = { "请按任意键继续" };
        senddrawing(servSock, ab);
        //getchar();
        system("cls");
    }
    void drawIndex(SOCKET& servSock)                            //画出目录
    {
        system("cls");
        printf("\t===================================================================\n");
        printf("\t                     *****商品目录*****                             \n");
        printf("\t===================================================================\n");
        char ab[BUF_SIZE] = {
            "\t===================================================================\n"
            "\t                     *****商品目录*****                             \n"
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
    bool GetInformathion(int index, SOCKET servSock);           //获取并输出商品信息
    void GetIndex(SOCKET servSock);                             //获取并输出商品目录
    bool init1();                                               //从本地文件读入信息
    bool init2();
    bool init3();
    void save1();
    void save2();
    void save3();                                               //将商品信息保存到本地文件中
    bool Buy(int ID, int count);                                //购买商品的操作与数据检查
    bool Sell(int n, int ID);                                   //售卖商品的操作与数据检查
    void AddNew(char owner[], char name[], float bayValue, float sellValue, int leibie, char introduce[]);//添加新商品
    void save() {
        this->save1();
        this->save2();
        this->save3();
    }
    char* Getname(int index);                                   //得到商品的名字
    char* Getowner(int index);                                  //得到商品所属的商家
    float Getmoney(int index);                                  //得商品的价钱
    int Getleibie(int index);                                   //得到商品的类别
    int Getstock(int index);                                    //得到商品的库存
    int Getit(int index) {                                      //查看是否含有该商品
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
    void manage(char name[], SOCKET& servSock);                  //商家管理系统
    bool init()
    {
        bool i1, i2, i3;
        i1 = this->init1(); i2 = init2(); i3 = init3();
        if (i1 || i2 || i3) return true;
        else return false;
    };                                                          //从本地文件获取商品信息
    void newstr(char str[], int index, char name[], float BuyValue, float SellValue, int Stock, char owner[], char introduce[])
    {                                                           //建立一个新的字符串
        char ab[BUF_SIZE] = { "\n商品编号：\0" }, bc[BUF_SIZE];
        intab(ab, index);
        strcat(ab, "\n 商品名称：\0");
        strcat(ab, name);
        strcat(ab, "\n 购入价格：\0");
        floatab(ab, BuyValue);
        strcat(ab, "\n 出售价格：\0");
        floatab(ab, SellValue);
        strcat(ab, "\n 剩余：\0");
        intab(ab, Stock);
        strcat(ab, "\n 商家：\0");
        strcat(ab, owner);
        strcat(ab, "\n 介绍：");
        strcat(ab, introduce);
        strcpy(str, ab);

    }
    void search(SOCKET servSock)                                //搜索功能
    {
        char ab[BUF_SIZE] = { "请输入要查找的商品名称:\0" };
        char bc[100] = { "!\0" };
        getstr(servSock, ab, bc);
        cout << "请输入要查找的商品名称:\0" << endl;
        char name[30];
        getstr(servSock, bc, name);
        int find = 0;                                           //遍历三个商品链表，搜寻商品并返回信息
        for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++)
        {
            if (strcmp(iter->getName(), name) == 0) {
                find += 1;
                char str[BUF_SIZE];
                this->newstr(str, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
                getstr(servSock, str, bc);
                printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n",
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

                printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n",
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
                printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n",
                    iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner());
            }
        }
        if (find == 0) {
            strcpy_s(ab, "平台没有该商品，抱歉\0");
            getstr(servSock, ab, bc);
            cout << "平台没有该商品，抱歉" << endl;
        }

        else {
            cout << "共找到" << find << "" << endl;
            strcpy_s(ab, "共找到\0");
            intab(ab, find);
            strcat(ab, "条相关商品\0");
            getstr(servSock, ab, bc);
        }
    }
};
class order
{                                                               //订单类
public:
    int type; int ID;
    char name[30];                                              //商品名
    char owner[30];                                             //商家
    int count;
    float tolmoney;                                             //购买的数量所需的钱
public:
    order(int _type, int ID, char _name[], char _owner[], int count, float tolmoney)
    {                                                           //构造函数
        this->type = _type; this->ID = ID;
        this->count = count;
        strcpy_s(this->name, _name);
        strcpy_s(this->owner, _owner);
        //  strcpy_s(this->cust, _cust);
        this->tolmoney = tolmoney;
    }

};
class orderrecord                                               //订单记录类
{
public:
    int state;                                                    //0未支付，1支付完成
    int dingID;                                                   //订单编号
    int Goodsshu;                                              //订单中商品种数
    int  Goodsamount[30] = { 0 };                              //每个商品的数量
    char customer[30];                                            //订单所属的消费者
    int GoodsID[30] = { 0 };                                   //订单中每种商品的ID
    float Goodsmoney[30] = { 0 };                              //购买每种商品的数目所需的钱
    char owner[30][30];                                           //订单中每种商品所属的商家
    orderrecord()
    {
        this->state = 0;
        this->dingID = 0;
        this->Goodsshu = 0;
    }
    float gettotal()                                           //返回总金钱
    {
        float money = 0;
        for (int i = 0; i < this->Goodsshu; i++)
            money += this->Goodsmoney[i];
        return money;
    }
    void showrecord(SOCKET servSock, trade myTrade)              //展示订单记录中的内容
    {
        cout << "订单编号：" << this->dingID << endl;
        char ab[BUF_SIZE];
        char bc[BUF_SIZE] = { "\0" };
        strcpy_s(ab, "订单编号：\0"); myTrade.intab(ab, this->dingID);
        strcat(ab, "\t订单中商品种数:\0"); myTrade.intab(ab, this->Goodsshu);
        cout << "\t订单中商品种数:\0" << this->Goodsshu << endl;
        for (int i = 0; i < this->Goodsshu; i++)
        {
            strcat(ab, "\n商品ID:\0");  myTrade.intab(ab, this->GoodsID[i]);
            strcat(ab, "\t数量：\0");  myTrade.intab(ab, this->Goodsamount[i]);
            strcat(ab, "\t商家：\0"); strcat(ab, this->owner[i]);
            cout << "商品ID:" << this->GoodsID[i] << "\n"
                << "数量：" << this->Goodsamount[i] << "\n"
                << "商家:" << this->owner[i] << endl;
        }
        myTrade.getstr(servSock, ab, bc);
    }
};
class shoppingsystem                                            //电商系统类
{
private:
    int static log;                                             //0未登录1顾客2商家                             
    int static lognumber;   //-1未登录，0,1,2.。。。。指示在相应链表中登录用户的位置
    list<customer> custList;
    list<merchant> merList;
    //list<order> orderlist;
    list<orderrecord> record;
public:
    void removerecord()                                         //删除已支付的订单记录
    {
        auto iter = record.begin();
        for (; iter != record.end();)
        {
            if (iter->state == 1)   iter = record.erase(iter);
            else iter++;
        }

    }
    int amountofrecords(int ID)                                 //返回已生成订单中特定的商品总数
    {
        int amount = 0;
        for (auto iter = record.begin(); iter != record.end(); iter++) {
            for (int i = 0; i < iter->Goodsshu && iter->GoodsID >= 0; i++)
                if (iter->GoodsID[i] == ID && iter->state == 0) 
                    amount += iter->Goodsamount[i];
        }
        return amount;
    }
    void senddrawing(SOCKET servSock, char ab[]) {                  //发送界面
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //接收客户端请求
        strcpy_s(buffer, ab); puts(buffer);
        int strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                       //将数据原样返回
        closesocket(clntSock);                                   //关闭套接字
        memset(buffer, 0, BUF_SIZE);                             //重置缓冲区
    }
    void intab(char ab[], int a)                                 //在字符串ab后添加整数；
    {
        char sen[10]; itoa(a, sen, 10);
        strcat(ab, sen);
    }
    void floatab(char ab[], float a)                              //在字符串ab后添加小数；
    {
        char sen[10]; gcvt(a, 8, sen);
        strcat(ab, sen);
    }
    int getint(SOCKET servSock, char ab[])                         //从客户端接受整型，并发去下一步指示
    {
        SOCKADDR clntAddr;
        int notsend = 0;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);//接收客户端请求

        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);               //接收客户端发来的数据
        if (strlen(ab) == 0 || strcmp(ab, "!\0") == 0) ab[0] = '!', ab[1] = '\0', notsend = 1, cout << "只接受";
        //puts(ab);
        cout << "from client:"; puts(buffer);
        int wrong = 0;
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (buffer[i] > '9' || buffer[i] < '0')
                wrong = 1;
            if (wrong == 1 && buffer[0] == '-' && i == 0) wrong = 0;//-号情况
        }
        int n = atoi(buffer);
        char bc[100] = { "数据已接受\0" };
        if (notsend == 0)
            strcpy(buffer, ab);
        else
            strcpy(buffer, bc);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                     //将数据返回
        closesocket(clntSock);                                 //关闭套接字
        memset(buffer, 0, BUF_SIZE);                           //重置缓冲区
        return n;
    }
    void  getstr(SOCKET servSock, char ab[], char recvstr[])   //从客户端接受字符串，并发去下一步指示
    {
        SOCKADDR clntAddr;
        int notsend = 0;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        if (strlen(ab) == 0 || strcmp(ab, "!\0") == 0) notsend = 1, ab[0] = '!', ab[1] = '\0', cout << "只进行接受\n" << endl;
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //接收客户端请求
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);        //接收客户端发来的数据
        cout << "from client:"; puts(buffer);
        char bc[100] = { "数据已接受\0" };
        strcpy(recvstr, buffer);
        if (notsend == 0)
            strLen = strlen(ab), send(clntSock, ab, strLen, 0);
        else
            strLen = strlen(bc), send(clntSock, buffer, strLen, 0);     //将数据返回
        closesocket(clntSock);                                     //关闭套接字
        memset(buffer, 0, BUF_SIZE);                               //重置缓冲区
    }
    void operate(SOCKET& servSock){
        cout << "请按任意键继续" << endl;
        char ab[BUF_SIZE] = { "请按任意键继续" };
        senddrawing(servSock, ab);
        //getchar();
        system("cls");
    }
    void drawIndex(SOCKET& servSock)
    {
        system("cls");
        printf("\t===================================================================\n");
        printf("\t                     *****商品目录*****                             \n");
        printf("\t===================================================================\n");
        char ab[BUF_SIZE] = {
            "\t===================================================================\n"
            "\t                     *****商品目录*****                             \n"
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
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //接收客户端请求
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);  //接收客户端发来的数据
        puts(buffer);
        char ab[BUF_SIZE] = {
            "\t =================================================================\n"
            "\t|                         欢迎使用                               |\n"
            "\t ================================================================= \n"
            "\t                  1.登录                                          \n"
            "\t                  2.查看商品                                      \n"
            "\t                  3.搜索商品                                      \n"
            "\t                  4.创建账号                                      \n"
            "\t                  退出（请按0）                                   \n"
            "\t =================================================================\n""请输入您的选项:\0"
        };
        puts(ab);
        strcpy_s(buffer, ab); puts(buffer);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);  //将数据原样返回
        send(clntSock, buffer, strLen, 0);  //将数据原样返回
        closesocket(clntSock);  //关闭套接字
        memset(buffer, 0, BUF_SIZE);  //重置缓冲区
    }
    void DrawMenu1(SOCKET& servSock)                            //消费者界面
    {
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //接收客户端请求
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);       //接收客户端发来的数据
        puts(buffer);
        char ab[BUF_SIZE] = {
            "\t =================================================================\n"
            "\t|                         欢迎使用                               |\n"
            "\t ================================================================= \n"
            "\t                     1.登出                                        \n"
            "\t                     2.查看商品                                    \n"
            "\t                     3.搜索商品                                    \n"
           "\t                      4.修改信息                                      \n"
            "\t                     5.购物车                                         \n"
            "\t                     退出（请按0）                                 \n"
            "\t =================================================================\n""请输入您的选项:\0"
        };
        puts(ab);
        strcpy_s(buffer, ab); puts(buffer);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                     //将数据原样返回
        send(clntSock, buffer, strLen, 0);                     //将数据原样返回
        closesocket(clntSock);                                 //关闭套接字
        memset(buffer, 0, BUF_SIZE);                           //重置缓冲区
    }
    void DrawMenu2(SOCKET& servSock)                           //商家界面
    {
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        char buffer[BUF_SIZE] = { 0 };
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);  //接收客户端请求
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);      //接收客户端发来的数据
        puts(buffer);
        char ab[BUF_SIZE] = {
           "\t =================================================================\n"
            "\t|                         欢迎使用                               |\n"
            "\t ================================================================= \n"
            "\t                     1.登出                                        \n"
            "\t                     2.查看商品                                    \n"
            "\t                     3.搜索商品                                    \n"
            "\t                     4.修改信息                                      \n"
            "\t                     5.管理商品                                      \n"
            "\t                     6.增加商品                                      \n"
            "\t                     退出（请按0）                                 \n"
            "\t =================================================================\n""请输入您的选项:\0"
        };
        puts(ab);
        strcpy_s(buffer, ab); puts(buffer);
        strLen = strlen(buffer);
        send(clntSock, buffer, strLen, 0);                     //将数据原样返回
        send(clntSock, buffer, strLen, 0);                     //将数据原样返回
        closesocket(clntSock);                                 //关闭套接字
        memset(buffer, 0, BUF_SIZE);                           //重置缓冲区
    }
    bool createaccount(SOCKET& sevSocket);                      //创建账号
    int login(trade& myTrade, SOCKET& sevSocket);               //登录
    void logout(trade& myTrade, SOCKET& sevSocket);             //登出
    bool init1();                                               //本地文件的初始化
    bool init2();
    void save1();                                               //本地文件的保存
    void save2();
    int ismer(char user[]);                                      //判断用户是否为商家
    int iscust(char user[]);                                     //判断用户是否为消费者
    int shoppingcar(trade& myTrade, SOCKET& sevSocket);
    void save()                                                  //存储用户信息到本地文件
    {
        save1();
        save2();
    }
    bool init()                                                  //从本地文件中初始化
    {
        int ok = 0, ko = 0;
        this->log = 0;
        this->lognumber = -1;
        if (this->init1()) { ok = 1; cout << "1chushi" << endl; }
        if (this->init2()) { ko = 1; cout << "2chushi" << endl; }
        if (ok || ko) return true;
        else return false;
    }
    int information(trade& myTrade, SOCKET& sevSocket);          //获取修改当前登录用户信息；
    int information1(trade& myTrade, SOCKET& sevSocket);
    int mainsystem(SOCKET& sevSocket);                           //主系统
    int custsystem(trade& myTrade, SOCKET& sevSocket);           //消费者系统
    int merchantsystem(trade& mytrade, SOCKET& sevSocket);       //商家系统
    void mermanage(trade& myTrade, SOCKET& sevSocket);           //商家管理操作
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
    char ab[BUF_SIZE] = { "请输入用户类别，2为商家，1为消费者:\0" }, bc[100] = { "!\0" };
    cout << "请输入用户类别，2为商家，1为消费者:\0" << endl;
    getstr(servSock, ab, bc);
    int type; //cin >> type;
    type = getint(servSock, bc);
    strcpy_s(ab, "开始创建用户，请输入用户名:\0");
    getstr(servSock, ab, bc);
    cout << "开始创建用户，请输入用户名:\0" << endl;
    char user[30] = { "\0" };
    bc[0] = '\0';
    getstr(servSock, bc, user);
    //cin >> user; getchar();
    auto iter1 = merList.begin(); auto iter2 = custList.begin();
    if (type == 2)                                              //判断身份
    {
        if (this->ismer(user) == 0 && this->iscust(user) == 0) {
            cout << "请输入密码：" << endl;
            strcpy_s(ab, "请输入密码:\0");
            getstr(servSock, ab, bc);
            char pass[30] = { "\0" };
            ab[0] = '\0';
            getstr(servSock, ab, pass);
            strcpy(ab, "请输入账户金额:\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            cout << "请输入账户金额：" << endl;
            float money; //cin >> money;
            //getchar(); 
            ab[0] = '!', ab[1] = '\0';
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            money = atof(bc);
            merList.push_back(merchant(user, pass, money, 0));

        }
        else {
            cout << "用户名重复，请重新创建" << endl;
            strcpy_s(ab, "用户名重复，请重新创建\0");
            getstr(servSock, ab, bc);
            return false;
        }
    }
    if (type == 1)
    {
        if (this->ismer(user) == 0 && this->iscust(user) == 0) {
            cout << "请输入密码：" << endl;
            strcpy(ab, "请输入密码:\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            char pass[30] = { "\0" }; ab[0] = '\0';
            getstr(servSock, ab, pass);
            //cin >> pass; getchar();
            strcpy_s(ab, "请输入账户金额:\0");
            getstr(servSock, ab, bc);
            cout << "请输入账户金额：" << endl;
            float money;// cin >> money;getchar();
            ab[0] = '!', ab[1] = '\0';
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            money = atof(bc);
            custList.push_back(customer(user, pass, money, 0));
        }
        else {
            strcpy_s(ab, "用户名重复，请重新创建\0");
            getstr(servSock, ab, bc);
            cout << "用户名重复，请重新创建" << endl;
            return false;
        }
    }
    return false;
}
int shoppingsystem::login(trade& myTrade, SOCKET& servSock){
    char pass[30]; int num = 0;
    int find = 0;//find==0,未找到;find==1，找到customer;find==2找到merchant；
    char ab[BUF_SIZE] = { "请输入用户名:\0" };
    char bc[100] = { "\0" };
    this->getstr(servSock, ab, bc);
    cout << "请输入用户名:\0" << endl;
    char user[30] = { "\0" }; //cin >> user;
    //getchar();
    bc[0] = '!\0';
    this->getstr(servSock, bc, user);
    if (this->log != 0) {
        cout << "请先退出当前用户" << endl;
        strcpy_s(ab, "请先退出当前用户\0");
        bc[0] = '!\0';
        getstr(servSock, ab, bc);
        return 0;
    }                                                 //重复登录
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
        cout << "未创建的用户，登陆失败" << endl;
        strcpy_s(ab, "未创建的用户，登陆失败\0");                   //登陆失败
        bc[0] = '\0';
        getstr(servSock, ab, bc);
        this->log = 0; this->lognumber = -1; return 0;
    };
    if (find == 1) {                                            //存在用户开始输入密码
        cout << "请输入密码：" << endl;
        strcpy_s(ab, "请输入密码:\0"); bc[0] = '\0';
        getstr(servSock, ab, bc);
        bc[0] = '\0'; pass[0] = '\0';
        getstr(servSock, bc, pass);
        //cin >> pass;
        //getchar();
        if (strcmp(iter1->password, pass) == 0)                 //密码正确登陆成功
        {
            cout << "登陆成功" << endl;
            strcpy_s(ab, "登陆成功\0");
            getstr(servSock, ab, bc);
            iter1->state = 1; this->log = 1;                    //标记登录
            strcpy_s(myTrade.theuser, user);
            return 1;
        }
        else {                                                  //密码错误登陆失败
            cout << "密码错误" << endl;
            strcpy_s(ab, "密码错误\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            this->lognumber = -1; this->log = 0;
            return 0;
        }
    }
    if (find == 2) {
        cout << "请输入密码：" << endl;
        // cin >> pass;
         //getchar();
        strcpy_s(ab, "请输入密码:\0");
        pass[0] = '\0';
        getstr(servSock, ab, pass);
        pass[0] = '\0'; ab[0] = '\0';
        getstr(servSock, ab, pass);
        if (strcmp(iter2->password, pass) == 0)
        {
            cout << "登陆成功" << endl;
            strcpy_s(ab, "登陆成功\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            iter2->state = 1; this->log = 2;//标记登录
            strcpy_s(myTrade.theuser, user);
            return 2;
        }
        else {
            cout << "密码错误" << endl;
            strcpy_s(ab, "密码错误\0"); bc[0] = '\0';
            getstr(servSock, ab, bc);
            this->lognumber = -1; this->log = 0;
            return 0;
        }
    }
    return 0;
}
void shoppingsystem::logout(trade& myTrade, SOCKET& servSock)
{                                                              //登出，修改系统状态
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
int shoppingsystem::information(trade& myTrade, SOCKET& servSock)//查看修改登录用户的信息；
{
    int over = 0;
    if (this->log == 0 && this->lognumber == -1){
        cout << "未登录" << endl;
        char ab[100] = { "未登录\0" };
        char bc[100] = { "!\0" };
        getstr(servSock, ab, bc);
        return 0;
    }
    else if (this->log == 1 && this->lognumber != -1){
        auto iter = custList.begin();
        for (int i = 0; i < this->lognumber && iter != custList.end(); i++)
            iter++;
        if (iter->state == 1){
            char ab[3000] = { "用户名：\0" }; char bc[BUF_SIZE] = { "!\0" };
            strcat(ab, iter->username);
            strcat(ab, "\n用户类型：消费者\0");
            strcat(ab, "\n密码：\0");
            strcat(ab, iter->password);
            strcat(ab, "\n账号余额：\0");
            this->floatab(ab, iter->money);
            puts(ab);
            this->getstr(servSock, ab, bc);

            cout << "用户名：" << iter->username << endl;
            cout << "用户类型：消费者" << endl;
            cout << "密码：" << iter->password << endl;
            cout << "账号余额：" << iter->money << endl;
        }
        for (int order = 0; order != 3;){
            char ab[BUF_SIZE] = { "查看请输入0，充值请输入1，修改密码请输入2，退出请输入3:\0" };
            char bc[100] = { "!\0" };
            getstr(servSock, ab, bc);
            cout << "查看请输入0，充值请输入1，修改密码请输入2，退出请输入3:" << endl;
            //cin >> order; getchar();
            order = getint(servSock, bc);
            if (order == 0) {
                strcat(ab, iter->username);
                strcat(ab, "\n用户类型：消费者\0");
                strcat(ab, "\n密码：\0");
                strcat(ab, iter->password);
                strcat(ab, "\n账号余额：\0");
                this->floatab(ab, iter->money);
                this->getstr(servSock, ab, bc);
                cout << "用户名：" << iter->username << endl;
                cout << "用户类型：消费者" << endl;
                cout << "密码：" << iter->password << endl;
                cout << "账号余额：" << iter->money << endl;
            }
            if (order == 1) {
                strcpy_s(ab, "请输入充值金额:\0");
                getstr(servSock, ab, bc);
                cout << "请输入充值金额：" << endl;
                float money; //cin >> money;
                ab[0] = '!'; ab[1] = '\0';
                this->getstr(servSock, ab, bc);
                money = atof(bc);
                iter->money += money; //getchar();
                cout << "充值成功" << endl;
                strcpy_s(ab, "充值成功\0");
                getstr(servSock, ab, bc);
            }
            if (order == 2) {
                char pass[30];
                strcpy_s(ab, "请输入新密码:\0");
                getstr(servSock, ab, bc);
                cout << "请输入新密码：" << endl;
                //cin >> pass;
                ab[0] = '!'; ab[1] = '\0';
                getstr(servSock, ab, pass);
                strcpy_s(iter->password, pass);
                cout << "已修改" << endl;
                strcpy_s(ab, "已修改");
                getstr(servSock, ab, bc);
                //getchar();
            }
            if (order == 3) over = 1;
        }
        return 1;
    }
    char ab[100] = { "未登录\0" };
    char bc[100] = { "!\0" };
    strcpy_s(ab, "退出修改\n\0");
    getstr(servSock, ab, bc);
    printf("退出修改\n");
    return over;
}
int shoppingsystem::information1(trade& myTrade, SOCKET& servSock)//查看修改登录用户的信息；
{
    int over = 0;

    if (this->log == 0 && this->lognumber == -1)
    {
        char ab[100] = { "未登录\0" };
        char bc[100] = { "!\0" };
        getstr(servSock, ab, bc);
        cout << "未登录" << endl; return 0;
    }
    else if (this->log == 2 && this->lognumber != -1){
        auto iter = merList.begin();
        auto it = iter;
        for (; iter != merList.end(); iter++)     
            if (strcmp(iter->username, myTrade.theuser) == 0) it = iter;
        char ab[3000] = { "用户名：" }; char bc[1000] = { "!\0" };
        strcat(ab, it->username);
        strcat(ab, "\n用户类型：商家\0");
        strcat(ab, "\n密码：\0");
        strcat(ab, it->password);
        strcat(ab, "\n账号余额：\0");
        this->floatab(ab, it->money);
        this->getstr(servSock, ab, bc);
        cout << "用户名：" << it->username << endl;
        cout << "用户类型：商家" << endl;
        cout << "密码：" << it->password << endl;
        cout << "账号余额：" << it->money << endl;
        for (int order = 0; order != 3;){
            char ab[BUF_SIZE] = { "查看请输入0，充值请输入1，修改密码请输入2，退出请输入3:\0" };
            char bc[1000] = { "!\0" };
            getstr(servSock, ab, bc);
            cout << "查看请输入0，充值请输入1，修改密码请输入2，退出请输入3:" << endl;
            order = getint(servSock, bc);
            if (order == 0) {
                strcpy_s(ab, "\n用户名："); char bc[3] = { "!\0" };
                strcat(ab, it->username);
                strcat(ab, "\n用户类型：商家\0");
                strcat(ab, "\n密码：\0");
                strcat(ab, it->password);
                strcat(ab, "\n账号余额：\0"); floatab(ab, it->money);
                bc[0] = '\0';
                getstr(servSock, ab, bc);
                cout << "用户名：" << it->username << endl;
                cout << "用户类型：商家" << endl;
                cout << "密码：" << it->password << endl;
                cout << "账号余额：" << it->money << endl;
            }
            if (order == 1) {
                cout << "请输入充值金额：" << endl;
                float money; //cin >> money;
                strcpy_s(ab, "请输入充值金额:\0");
                this->getstr(servSock, ab, bc);
                bc[0] = '\0';
                ab[0] = '!'; ab[1] = '\0';
                this->getstr(servSock, ab, bc);
                money = atof(bc);
                it->money += money;// getchar();
                cout << "充值成功" << endl;
                strcpy_s(ab, "充值成功\0");
                getstr(servSock, ab, bc);
            }
            if (order == 2) {
                char pass[30] = { "\0" };
                cout << "请输入新密码：" << endl;
                strcpy_s(ab, "请输入新密码:\0");
                getstr(servSock, ab, bc);
                //cin >> pass;
                ab[0] = '\0';
                getstr(servSock, ab, pass);
                strcpy_s(it->password, pass);
                cout << "已修改" << endl;
                strcpy_s(ab, "已修改");
                getstr(servSock, ab, bc);
                //getchar();
            }
            if (order == 3) over = 1;
        }
        return 2;
    }
    char ab[BUF_SIZE], bc[BUF_SIZE];
    strcpy_s(ab, "退出修改\n\0");
    getstr(servSock, ab, bc);
    printf("退出修改\n");
    return over;
}
void shoppingsystem::save2(){
    ofstream   file2;
    file2.open("merchant.txt");
    if (!file2.fail()){
        file2 << "=========================================================================\n";
        file2 << "                         *****商家信息*****                               \n";
        file2 << "=========================================================================\n";
        for (auto iter = merList.begin(); iter != merList.end(); iter++)
        {
            file2 << "USER:" << iter->username
                << "\tPASSWORD:" << iter->password
                << "\tMONEY:" << iter->money
                << "\n";
        }

    }
    else printf("记录文件创建失败");
}
void shoppingsystem::save1(){
    ofstream   file1;
    file1.open("customer.txt");
    if (!file1.fail())
    {
        file1 << "=========================================================================\n";
        file1 << "                      *****消费者信息*****                               \n";
        file1 << "=========================================================================\n";
        for (auto iter = custList.begin(); iter != custList.end(); iter++)
        {
            file1 << "USER:" << iter->username
                << "\tPASSWORD:" << iter->password
                << "\tMONEY:" << iter->money
                << "\n";
        }

    }
    else printf("记录文件创建失败\n");
}

bool shoppingsystem::init1(){
    ifstream file1("customer.txt");
    if (!file1.fail()){
        char titles[3][TLEN] = { "USER:" ,"PASSWORD:" ,"MONEY:" };
        char saves[3][ALEN] = {};
        int tIndex = 0;
        char buf[128];
        int kinds = 0;
        for (int i = 0; i < 3; i++)                             //忽略标题
            file1.getline(buf, 128);
        while (!file1.eof()){
            char temp[TLEN] = "";                               //读取文件内容的字符数组
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
                            return false;                       //超过参数长度，初始化失败
                    }
                    break;
                }
                if (i == TLEN - 1)
                    return false;                               //未匹配到参数名称，失败
            }
        }
        return true;
    }
    return false;                                               //文件不存在
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
        for (int i = 0; i < 3; i++)//忽略标题
            file2.getline(buf, 128);
        while (!file2.eof()){
            char temp[TLEN] = "";//读取文件内容的字符数组
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
                            return false;//超过参数长度，初始化失败                        
                    }
                    break;
                }
                if (i == TLEN - 1)               
                    return false;//未匹配到参数名称，失败                
            }
        }
        return true;
    }
    return false;//文件不存在
}
int commodity::kinds = 0;                                      //初始时商品种类为0
int shoppingsystem::log = 0;                                   //登录状态为0
int shoppingsystem::lognumber = -1;                            //登陆者无
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
void trade::AddNew(char owner[], char name[], float buyValue, float sellValue, int leibie, char introduce[])//添加新的商品对象
{                                                              //商家添加商品
    switch (leibie) {
    case 1:dataList1.push_back(Book(owner, name, buyValue, sellValue, introduce)); break;
    case 2:dataList2.push_back(Cloth(owner, name, buyValue, sellValue, introduce)); break;
    case 3:dataList3.push_back(Food(owner, name, buyValue, sellValue, introduce)); break;
    default: break;
    }
}
void trade::GetIndex(SOCKET recvSock)                           //得到商品目录
{
    for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
        char ab[100] = { "\t 商品编号：" }; char bc[100] = { "\n 商品名称： " };
        this->intab(ab, iter->getID());
        strcat(ab, bc);
        strcat(ab, iter->getName());
        getstr(recvSock, ab, bc);
        printf("\t 商品编号： %i   商品名称：%s\n", iter->getID(), iter->getName());
    }
    for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
        char ab[100] = { "\t 商品编号：" }; char bc[100] = { "\n 商品名称： " };
        this->intab(ab, iter->getID());
        strcat(ab, bc);
        strcat(ab, iter->getName());
        getstr(recvSock, ab, bc);
        printf("\t 商品编号： %i   商品名称：%s\n", iter->getID(), iter->getName());
    }
    for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
        char ab[100] = { "\t 商品编号：" }; char bc[100] = { "\n 商品名称： " };
        this->intab(ab, iter->getID());
        strcat(ab, bc);
        strcat(ab, iter->getName());
        getstr(recvSock, ab, bc);
        printf("\t 商品编号： %i   商品名称：%s\n", iter->getID(), iter->getName());
    }
}
bool trade::GetInformathion(int index, SOCKET servSock) {         //获得商品的介绍
    for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
        if (iter->getID() == index){
            char ab[BUF_SIZE] = { "商品编号：\0" }, bc[BUF_SIZE];
            intab(ab, index);
            strcat(ab, "\n 商品名称：\0");
            strcat(ab, iter->getName());
            strcat(ab, "\n 购入价格：\0");
            floatab(ab, iter->getBuyValue());
            strcat(ab, "\n 出售价格：\0");
            floatab(ab, iter->getsellValue());
            strcat(ab, "\n 剩余：\0");
            intab(ab, iter->getStock());
            strcat(ab, "\n 商家：\0");
            strcat(ab, iter->getowner());
            strcat(ab, "\n 介绍：\0");
            strcat(ab, iter->getIntroduce());
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n 介绍：%s\n",
                index, iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
            return true;
        }
    }
    for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
        if (iter->getID() == index){
            char ab[BUF_SIZE] = { "商品编号：\0" }, bc[BUF_SIZE];
            intab(ab, index);
            strcat(ab, "\n 商品名称：\0");
            strcat(ab, iter->getName());
            strcat(ab, "\n 购入价格：\0");
            floatab(ab, iter->getBuyValue());
            strcat(ab, "\n 出售价格：\0");
            floatab(ab, iter->getsellValue());
            strcat(ab, "\n 剩余：\0");
            intab(ab, iter->getStock());
            strcat(ab, "\n 商家：\0");
            strcat(ab, iter->getowner());
            strcat(ab, "\n 介绍：\0");
            strcat(ab, iter->getIntroduce());
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n 介绍：%s\n",
                index, iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
            return true;
        }
    }
    for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
        if (iter->getID() == index){
            char ab[BUF_SIZE] = { "商品编号：\0" }, bc[BUF_SIZE];
            intab(ab, index);
            strcat(ab, "\n 商品名称：\0");
            strcat(ab, iter->getName());
            strcat(ab, "\n 购入价格：\0");
            floatab(ab, iter->getBuyValue());
            strcat(ab, "\n 出售价格：\0");
            floatab(ab, iter->getsellValue());
            strcat(ab, "\n 剩余：\0");
            intab(ab, iter->getStock());
            strcat(ab, "\n 商家：\0");
            strcat(ab, iter->getowner());
            strcat(ab, "\n 介绍：\0");
            strcat(ab, iter->getIntroduce());
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n 介绍：%s\n",
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
        file1 << "                         *****商品信息*****                               \n";
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
    else printf("记录文件创建失败\n");
}
void trade::save2(){
    ofstream   file1;
    file1.open("stock2.txt");
    if (!file1.fail()){
        file1 << "=========================================================================\n";
        file1 << "                         *****商品信息*****                               \n";
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
    else printf("记录文件创建失败\n");
}
void trade::save3(){
    ofstream   file1;
    file1.open("stock3.txt");
    if (!file1.fail()){
        file1 << "=========================================================================\n";
        file1 << "                         *****商品信息*****                               \n";
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
    else printf("记录文件创建失败\n");
}
bool trade::init1(){
    ifstream file3("stock1.txt");
    if (!file3.fail()){
        char titles[CAP][TLEN] = { "ID:","owner:" ,"NAME:","COST:","VALUE:","STOCK:","INTRODUCE:" };
        char saves[CAP][ALEN] = {};
        int tIndex = 0;
        char buf[128];
        int kinds = 0;
        for (int i = 0; i < 3; i++)//忽略标题
            file3.getline(buf, 128);
        while (!file3.eof()){
            char temp[TLEN] = "";//读取文件内容的字符数组
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
                            return false;//超过参数长度，初始化失败
                    }
                    break;
                }
                if (i == TLEN - 1)
                    return false;//未匹配到参数名称，失败
            }
        }
        file3.close();
        commodity::kinds = kinds;
        return true;
    }
    return false;//文件不存在
}
bool trade::init2(){
    ifstream file4("stock2.txt");
    if (!file4.fail()){
        char titles[CAP][TLEN] = { "ID:","owner:" ,"NAME:","COST:","VALUE:","STOCK:" ,"INTRODUCE:" };
        char saves[CAP][ALEN] = {};
        int tIndex = 0;
        char buf[128];
        int kinds = 0;
        for (int i = 0; i < 3; i++)//忽略标题
            file4.getline(buf, 128);
        while (!file4.eof()){
            char temp[TLEN] = "";//读取文件内容的字符数组
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
                            return false;//超过参数长度，初始化失败
                    }
                    break;
                }
                if (i == TLEN - 1)
                    return false;//未匹配到参数名称，失败
            }
        }
        file4.close();
        commodity::kinds += kinds;
        return true;
    }
    return false;//文件不存在
}
bool trade::init3(){
    ifstream file5("stock3.txt");
    if (!file5.fail()){
        char titles[CAP][TLEN] = { "ID:","owner:" ,"NAME:","COST:","VALUE:","STOCK:" ,"INTRODUCE:" };
        char saves[CAP][ALEN] = {};
        int tIndex = 0;
        char buf[128];
        int kinds = 0;
        for (int i = 0; i < 3; i++)//忽略标题
            file5.getline(buf, 128);
        while (!file5.eof()){
            char temp[TLEN] = "";//读取文件内容的字符数组
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
                            return false;//超过参数长度，初始化失败
                    }
                    break;
                }
                if (i == TLEN - 1)
                    return false;//未匹配到参数名称，失败
            }
        }
        file5.close();
        commodity::kinds += kinds;
        return true;
    }
    return false;//文件不存在
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
        printf("请输入您的选项:\0");
        select0 = getint(servSock, bc);
        switch (select0){
        case 2://查看
            this->drawIndex(servSock);
            myTrade.GetIndex(servSock);
            cout << "请选择详细查看的商品序号：" << endl;
            int index; //char bc[100] = { "请选择详细查看的商品序号:\0" };
            strcpy_s(bc, "请选择详细查看的商品序号:\0");
            getstr(servSock, bc, ab);
            index = getint(servSock, ab);
            if (!myTrade.GetInformathion(index, servSock)) {
                char ab[100] = { "无效的商品序号\0" };
                cout << "无效的商品序号" << endl; this->getstr(servSock, ab, bc);
                this->operate(servSock); break;
            }
            this->operate(servSock);
            break;
        case 3://查找
            myTrade.search(servSock);
            this->operate(servSock);
            break;
        case 0://退出
            quitFlag = true;
            break;
        case 1://登录
            this->login(myTrade, servSock);
            if (this->log == 0) break;
            else   if (this->log == 1)  this->custsystem(myTrade, servSock);
            else   if (this->log == 2) this->merchantsystem(myTrade, servSock);
            if (this->log != 0)
                quitFlag = true;
            this->operate(servSock);
            break;
        case 4://创建账号
            this->createaccount(servSock);
            this->operate(servSock);
            break;
        default:
        {
            system("cls");
            char ab[100] = { "无效的选项\0" };
            // char bc[3] = { "!\0" };
            getstr(servSock, ab, bc);
            cout << "无效的选项" << endl;
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
        printf("请输入您的选项：");
        bc[0] = '\0';
        select1 = getint(servSock, bc);
        //cin >> select1; getchar();
        switch (select1){
        case 2://查看
            drawIndex(servSock);
            myTrade.GetIndex(servSock);
            strcpy_s(ab, "请选择详细查看的商品序号:\0");
            getstr(servSock, ab, bc);
            cout << "请选择详细查看的商品序号：" << endl;
            int index;
            ab[1] = '\0', ab[0] = '!';
            index = getint(servSock, bc);
            if (!myTrade.GetInformathion(index, servSock)) {
                cout << "无效的商品序号" << endl;
                strcpy_s(ab, "无效的商品序号\0");
                getstr(servSock, ab, bc);
            }
            this->operate(servSock);
            break;
        case 3://查找
            myTrade.search(servSock);
            this->operate(servSock);
            break;
        case 0://退出
            quitFlag1 = true;
            break;
        case 1://退出登录
            this->logout(myTrade, servSock); quitFlag1 = true;
            this->operate(servSock);
            break;
        case 4://修改信息
            this->information(myTrade, servSock);
            this->operate(servSock);
            break;
        case 5://购物车
            this->shoppingcar(myTrade, servSock);
            this->operate(servSock);
            break;
        default:
            system("cls");
            cout << "无效的选项" << endl;
            strcpy_s(ab, "无效的选项\0");
            getstr(servSock, ab, bc);
            this->operate(servSock);
        }
    }
    return select1;
}
int shoppingsystem::shoppingcar(trade& myTrade, SOCKET& servSock)//购物车操作
{
    list<order> car;
    list<order> ding;
    bool quit = false;
    while (!quit){
        char ab[BUF_SIZE] = { "0.退出\n"
            "1.添加商品到购物车\n"
             "2.管理购物车\n"
             "3.生成订单\n"
             "4.支付订单\n"
             "请输入所需的操作:\0" };
        char bc[1000] = { "\0" };
        cout << "0.退出\n"
            << "1.添加商品到购物车\n"
            << "2.管理购物车\n"
            << "3.生成订单\n"
            << "4.支付订单\n"
            << "请输入所需的操作：\n"
            << endl;
        getstr(servSock, ab, bc);
        int select; bc[0] = '\0';
        select = getint(servSock, bc);
        switch (select)
        {
        case 0://退出
            quit = true;
            break;
        case 1://添加商品到购物车中
        {
            myTrade.GetIndex(servSock);
            cout << "请输入加入购物车的商品ID：" << endl;
            strcpy_s(ab, "请输入加入购物车的商品ID:\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            int ID, amount;
            ID = getint(servSock, bc);
            int times = 0;
            //        cin >> ID; getchar();
            while (myTrade.Getit(ID) == 0 && times < 5) {
                cout << "请输入正确的编号：" << endl;
                strcpy_s(ab, "请输入正确的编号:\0"); getstr(servSock, ab, bc);
                //cin >> ID; getchar();
                bc[0] = '\0';
                ID = getint(servSock, bc);
                times++;
            }
            if (times >= 5) break;
            cout << "请输入拟购买的商品数量：" << endl;
            strcpy_s(ab, "请输入拟购买的商品数量:\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            bc[0] = '\0';
            amount = getint(servSock, bc);
            //cin >> amount;
            //getchar();
            car.push_back(order(myTrade.Getleibie(ID), ID, myTrade.Getname(ID), myTrade.Getowner(ID), amount, amount * myTrade.Getmoney(ID)));
            cout << "加入购物车成功！" << endl; strcpy(ab, "加入购物车成功\0"); getstr(servSock, ab, bc);
            this->operate(servSock); break;
        }
        case 2://管理购物车
        {
            for (auto iter = car.begin(); iter != car.end(); iter++)
            {
                if (iter->count) {
                    strcpy(ab, "ID:\0");
                    intab(ab, iter->ID);
                    strcat(ab, ", name:\0 ");
                    strcat(ab, iter->name);
                    strcat(ab, ", 类别：\0");
                    intab(ab, iter->type);
                    strcat(ab, ", amount:\0 ");
                    intab(ab, iter->count);
                    strcat(ab, ", 价格：\0");
                    floatab(ab, iter->tolmoney);
                    getstr(servSock, ab, bc);
                    printf("ID:%d, name:%s, 类别：%d, amount:%d, 价格:%f\n", iter->ID, iter->name, iter->type, iter->count, iter->tolmoney);
                }
            }
            int action;
            bool quit1 = false;
            while (!quit1)
            {
                cout << "0.退出\n"
                    << "1.从购物车中删除"
                    << "2.增减数量"
                    << "请输入所需的操作：\n" << endl;
                strcpy_s(ab, "\t0.退出\n""\t1.从购物车中删除\n""\t2.增减数量\n""\t请输入所需的操作:\0");
                bc[0] = '\0';
                getstr(servSock, ab, bc);
                bc[0] = '\0';
                action = getint(servSock, bc);
                switch (action)
                {
                case 0:
                    quit1 = true;
                    break;
                case 1://从购物车中删除
                {
                    for (auto iter = car.begin(); iter != car.end(); iter++){
                        if (iter->count) {
                            strcpy(ab, "ID:\0");
                            intab(ab, iter->ID);
                            strcat(ab, ", name: ");
                            strcat(ab, iter->name);
                            strcat(ab, ", 类别：");
                            intab(ab, iter->type);
                            strcat(ab, ", amount: ");
                            intab(ab, iter->count);
                            strcat(ab, ", 价格：");
                            floatab(ab, iter->tolmoney);
                            getstr(servSock, ab, bc);
                            printf("ID:%d, name:%s, 类别：%d, amount:%d, 价格:%f\n", iter->ID, iter->name, iter->type, iter->count, iter->tolmoney);
                        }
                    }
                    cout << "请选择删除的商品编号:" << endl;
                    strcpy_s(ab, "请选择删除的商品编号:\0"); getstr(servSock, ab, bc);
                    int a;    bc[0] = '\0';
                    a = getint(servSock, bc);
                    //cin >> a; getchar();
                    for (auto iter = car.begin(); iter != car.end(); iter++)
                    {
                        if (iter->ID == a)
                            iter->count = 0, iter->tolmoney = 0;
                    }
                    cout << "已删除" << endl; strcpy_s(ab, "已删除\0"); getstr(servSock, ab, bc);
                    this->operate(servSock); break;
                }
                case 2://增减商品数量
                {
                    for (auto iter = car.begin(); iter != car.end(); iter++){
                        if (iter->count) {
                            strcpy(ab, "ID:\0");
                            intab(ab, iter->ID);
                            strcat(ab, ", name: ");
                            strcat(ab, iter->name);
                            strcat(ab, ", 类别：");
                            intab(ab, iter->type);
                            strcat(ab, ", amount: ");
                            intab(ab, iter->count);
                            strcat(ab, ", 价格：");
                            floatab(ab, iter->tolmoney);
                            getstr(servSock, ab, bc);
                            printf("ID:%d, name:%s, 类别：%d, amount:%d, 价格:%f\n", iter->ID, iter->name, iter->type, iter->count, iter->tolmoney);
                        }            
                    }
                    cout << "请选择增减数量的商品编号:" << endl;
                    strcpy_s(ab, "请选择增减数量的商品编号:\0"); getstr(servSock, ab, bc);
                    int b, find; find = 0;
                    b = getint(servSock, bc);
                    for (auto iter = car.begin(); iter != car.end(); iter++){
                        if (iter->ID == b){
                            int c; 
                            cout << "请输入增减的数量(.../-..." << endl;
                            strcpy_s(ab, "请输入增减的数量(.../-...):\0"); 
                            getstr(servSock, ab, bc);
                            c = getint(servSock, bc);
                            if (iter->count + c >= 0) {
                                iter->count += c;
                                iter->tolmoney += c * (myTrade.GetGoodsPrice(iter->name));
                                cout << "已改变" << endl; strcpy_s(ab, "已改变\0");
                                getstr(servSock, ab, bc);
                            }
                            else {
                                cout << "输入数量有误，改变失败" << endl;
                                strcpy_s(ab, "输入数量有误，改变失败\0");
                                getstr(servSock, ab, bc);

                            }
                            find = 1;
                        }
                    }
                    if (find == 0) {
                        cout << "购物车中没有此商品，请加入后再进行管理" << endl;
                        strcpy_s(ab, "购物车中没有此商品，请加入后再进行管理\0");
                        getstr(servSock, ab, bc);
                    }
                    this->operate(servSock);
                    break;
                }
                default:
                    system("cls");
                    cout << "无效的选项" << endl;
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
        case 3://生成订单
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
                        strcpy_s(ab, "生成订单失败，商品编号：\0");
                        intab(ab, iter1->ID);
                        strcpy(ab, " 数量超出库存，请修改订单。\0");
                        bc[0] = '\0';
                        getstr(servSock, ab, bc);
                        cout << "生成订单失败，商品编号：" << iter1->ID << " 数量超出库存，请修改订单。\n" << endl;
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
                    cout << "生成订单成功,订单编号为：" << record1.dingID << endl;
                    strcpy_s(ab, "生成订单成功,订单编号为：\0"); intab(ab, record1.dingID);
                    getstr(servSock, ab, bc);
                }
            }
            else {
                cout << "请在购物车中添加商品" << endl;
                strcpy_s(ab, "请在购物车中添加商品\0"); //intab(ab, record1.dingID);
                getstr(servSock, ab, bc);
            }
            this->operate(servSock);
            break;
        }
        case 4://支付订单
        {
            if (record.empty()){
                strcpy_s(ab, "无订单\0");
                bc[0] = '\0';
                getstr(servSock, ab, bc);
                cout << "无订单" << endl;
            }
            else{
                cout << "选择要支付的订单：" << myTrade.theuser << endl;
                for (auto iter = record.begin(); iter != record.end(); iter++)
                    if (strcmp(iter->customer, myTrade.theuser) == 0){
                        itoa(iter->dingID, ab, 10); bc[0] = '\0';
                        getstr(servSock, ab, bc);
                        cout << iter->dingID << endl;
                        iter->showrecord(servSock, myTrade);
                    }
                strcpy_s(ab, "选择要支付的订单:\0");
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
                                    strcpy_s(ab, "余额不足，支付失败\0"); bc[0] = '\0';
                                    getstr(servSock, ab, bc);
                                    cout << "余额不足，支付失败" << endl;
                                }
                                else{
                                    it->money -= iter->gettotal();
                                    for (int j = 0; j < iter->Goodsshu; j++){
                                        for (auto iter1 = merList.begin(); iter1 != merList.end(); iter1++){
                                            if (strcmp(iter->owner[j], iter1->username) == 0){
                                                iter1->money += iter->Goodsmoney[j];
                                                myTrade.Updatestock(iter->GoodsID[j], -iter->Goodsamount[j]);
                                                strcpy_s(ab, "支付成功\0"); bc[0] = '\0';
                                                getstr(servSock, ab, bc);
                                                cout << "支付成功" << endl;
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
            cout << "无效的选项" << endl;
            strcpy_s(ab, "无效的选项\0");
            getstr(servSock, ab, bc);
            this->operate(servSock);
        }
    }
    return 0;
}
int shoppingsystem::merchantsystem(trade& myTrade, SOCKET& servSock)//商家系统
{
    bool quitFlag2 = false;
    int select2;
    char ab[BUF_SIZE] = { "\0" };
    char bc[1000] = { "\0" };
    while (!quitFlag2){
        this->DrawMenu2(servSock);
        printf("请输入您的选项：");
        select2 = getint(servSock, bc);
        switch (select2){
        case 2://查看
        {
            this->drawIndex(servSock);
            myTrade.GetIndex(servSock);
            strcpy_s(ab, "请选择详细查看的商品序号:\0");
            getstr(servSock, ab, bc);
            cout << "请选择详细查看的商品序号：" << endl;
            int index;
            //cin >> index; getchar();
            bc[0] = '\0';
            index = getint(servSock, bc);
            system("cls");
            if (!myTrade.GetInformathion(index, servSock)) {
                cout << "无效的商品序号" << endl;
                strcpy_s(ab, "无效的商品序号\0");
                getstr(servSock, ab, bc);
                this->operate(servSock);
            }
            else 
                this->operate(servSock);
            break;
        }
        case 3://查找
            myTrade.search(servSock);
            operate(servSock);  //system("cls");
            break;
        case 0://退出
            quitFlag2 = true;
            break;
        case 1://退出登录
            this->logout(myTrade, servSock); quitFlag2 = true;
            this->operate(servSock); //system("cls");
        case 4://修改信息
        {
            this->information1(myTrade, servSock);
            this->operate(servSock); //system("cls");
            break; }
        case 5://管理商品
        {   
            char name[30];
            for (auto iter = merList.begin(); iter != merList.end(); iter++)
                if (iter->state && strcmp(myTrade.theuser, iter->username) == 0) 
                    strcpy_s(myTrade.theuser, iter->username);
            myTrade.manage(name, servSock);
            this->operate(servSock);
            break;
        }
        case 6://添新
        {
            char name[30];
            strcpy_s(name, myTrade.theuser); cout << name << endl;
            char shang[30]; char introduces[30];
            float values;
            float costs;
            int lei;
            cout << "请输入新品名称:\0" << endl;
            strcpy_s(ab, "请输入新品名称:\0");
            getstr(servSock, ab, bc);
            getstr(servSock, bc, shang); cout << shang;
            cout << "购入价格:\0" << endl;
            strcpy_s(ab, "购入价格:\0");
            getstr(servSock, ab, bc);
            ab[0] = '!', ab[1] = '\0';
            getstr(servSock, ab, bc);
            costs = atof(bc); cout << costs << bc;
            cout << "卖出价格:\0" << endl;
            strcpy_s(ab, "卖出价格:\0");
            getstr(servSock, ab, bc);
            ab[0] = '!', ab[1] = '\0'; bc[0] = '\0';
            getstr(servSock, ab, bc);
            values = atof(bc); cout << values << bc;
            cout << "请输入描述:\0" << endl;
            strcpy_s(ab, "请输入描述:\0");
            getstr(servSock, ab, bc);
            getstr(servSock, bc, introduces); cout << introduces;
            cout << "请输入类别:\0" << endl;
            strcpy_s(ab, "请输入类别:\0");
            getstr(servSock, ab, bc);
            lei = getint(servSock, bc); cout << lei;
            myTrade.AddNew(name, shang, costs, values, lei, introduces);
            cout << "完成添新\0" << endl;
            strcpy_s(ab, "完成添新\0");
            getstr(servSock, ab, bc);
            this->operate(servSock);
            break;
        }
        default:
            system("cls");
            cout << "无效的选项" << endl;
            strcpy_s(ab, "无效的选项");
            getstr(servSock, ab, bc);
            this->operate(servSock);
        }
    }
    return select2;
}
void trade::manage(char name[], SOCKET& servSock)//具体的商家管理
{
    char ab[BUF_SIZE];
    char bc[1000];
    bool quitFlag2 = false;
    int select2;
    while (!quitFlag2){
        strcpy_s(ab, "0.结束管理操作\n 1.更新商品库存\n 2.管理商品价格\n 3.品类打折\n 4.查看商品\n请输入您的选项:\0");
        getstr(servSock, ab, bc);
        printf("请输入您的选项：\n0.结束管理操作\n 1.更新商品库存\n 2.管理商品价格\n 3.品类打折\n 4.查看商品\n");// cin >> select2; getchar();
        bc[0] = '\0';
        select2 = this->getint(servSock, bc);
        switch (select2)
        {
        case 4://查看商品
        {
            this->drawIndex(servSock);
            for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++){
                if (strcmp(this->theuser, iter->getowner()) == 0) {
                    newstr(ab, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
                    getstr(servSock, ab, bc);
                    printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n",
                        iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner());
                }
            }
            for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++){
                if (strcmp(this->theuser, iter->getowner()) == 0) {
                    newstr(ab, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
                    getstr(servSock, ab, bc);
                    printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n",
                        iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner());
                }
            }
            for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++){
                if (strcmp(this->theuser, iter->getowner()) == 0) {
                    newstr(ab, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner(), iter->getIntroduce());
                    getstr(servSock, ab, bc);
                    printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n",
                        iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getowner());
                }
            }
            cout << "请选择详细查看的商品序号：" << endl; 
            strcpy_s(ab, "请选择详细查看的商品序号:\0");
            bc[0] = '\0';
            getstr(servSock, ab, bc);
            int index; bc[0] = '\0';
            index = getint(servSock, bc);
            if (!this->GetInformathion(index, servSock)) {
                cout << "无效的商品序号" << endl;
                strcpy(ab, "无效的商品序号\0");
                getstr(servSock, ab, bc);
                this->operate(servSock);
            }
            else 
                this->operate(servSock);
            break;
        }
        case 3://品类打折
        {
            cout << "选择打折的品类：1.Book 2.Cloth 3.Food" << endl;
            strcpy_s(ab, "\t1.Book 2.Cloth 3.Food选择打折的品类:\0"); bc[0] = '\0';
            getstr(servSock, ab, bc);
            int choice; //cin >> choice; getchar(); 
            bc[0] = '\0';
            choice = getint(servSock, bc);
            bc[0] = '\0';
            cout << "输入所打折扣：" << endl;
            strcpy_s(ab, "输入所打折扣:\0");
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
            cout << "操作成功" << endl; strcpy_s(ab, "操作成功\0"); bc[0] = '\0'; getstr(servSock, ab, bc);
            this->operate(servSock);
            break;
        }
        case 0://退出
            quitFlag2 = true;
            break;
        case 1://更新商品库存
        {
            cout << "输入需更新库存的商品编号：" << endl;
            strcpy_s(ab, "输入需更新库存的商品编号:\0");
            getstr(servSock, ab, bc);
            int _ID;
            _ID = getint(servSock, bc);
            // cin >> _ID; getchar();
            for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++)
            {
                if (_ID == iter->getID())
                {
                    cout << "请输入增减商品的数量：" << endl; strcpy_s(ab, "请输入增减商品的数量:\0");
                    getstr(servSock, ab, bc);
                    int amount;
                    amount = getint(servSock, bc);
                    //cin >> amount; getchar();
                    iter->UpdateStock(amount); cout << "改变后存货为：" << iter->getStock() << endl;
                    strcpy_s(ab, "改变后存货为：");
                    intab(ab, iter->getStock());
                    getstr(servSock, ab, bc);
                }
            }
            for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++)
            {
                if (_ID == iter->getID())
                {
                    cout << "请输入增减商品的数量：" << endl; strcpy_s(ab, "请输入增减商品的数量:\0");
                    getstr(servSock, ab, bc);
                    int amount;
                    amount = getint(servSock, bc);
                    iter->UpdateStock(amount); cout << "改变后存货为：" << iter->getStock() << endl;
                    strcpy_s(ab, "改变后存货为：");
                    intab(ab, iter->getStock());
                    getstr(servSock, ab, bc);
                }
            }
            for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++)
            {
                if (_ID == iter->getID())
                {
                    cout << "请输入增减商品的数量：" << endl; strcpy_s(ab, "请输入增减商品的数量:\0");
                    getstr(servSock, ab, bc);
                    int amount;
                    amount = getint(servSock, bc);
                    //cin >> amount; getchar();
                    iter->UpdateStock(amount); cout << "改变后存货为：" << iter->getStock() << endl;
                    strcpy_s(ab, "改变后存货为：");
                    intab(ab, iter->getStock());
                    getstr(servSock, ab, bc);
                }
            }
            this->operate(servSock);
            break;
        }

        case 2://更新商品价格
        {
            cout << "输入需更新价格的商品编号：" << endl; strcpy_s(ab, "输入需更新价格的商品编号:\0");
            getstr(servSock, ab, bc);
            int ID_ = getint(servSock, bc); //cin >> ID_; getchar();
            for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++)
            {
                if (ID_ == iter->getID())
                {
                    cout << "请输入商品的价格：" << endl;
                    strcpy_s(ab, "请输入商品的价格:\0");
                    getstr(servSock, ab, bc);
                    float amount;
                    ab[0] = '!', ab[1] = '\0';
                    getstr(servSock, ab, bc);
                    amount = atof(bc);
                    iter->sellValue = amount; cout << "改变后价格为：" << iter->getsellValue() << endl;
                    strcpy_s(ab, "改变后价格为：\0"); floatab(ab, iter->getsellValue());
                    getstr(servSock, ab, bc);
                }
            }
            for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++)
            {
                if (ID_ == iter->getID())
                {
                    cout << "请输入商品的价格：" << endl;
                    strcpy_s(ab, "请输入商品的价格:\0");
                    getstr(servSock, ab, bc);
                    float amount;
                    ab[0] = '!', ab[1] = '\0';
                    getstr(servSock, ab, bc);
                    amount = atof(bc);
                    iter->sellValue = amount; cout << "改变后价格为：" << iter->getPrice() << endl;
                    strcpy_s(ab, "改变后价格为：\0"); floatab(ab, iter->getsellValue());
                    getstr(servSock, ab, bc);
                }
            }
            for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++)
            {
                if (ID_ == iter->getID())
                {
                    cout << "请输入商品的价格：" << endl;
                    strcpy_s(ab, "请输入商品的价格:\0");
                    getstr(servSock, ab, bc);
                    float amount;
                    ab[0] = '!', ab[1] = '\0';
                    getstr(servSock, ab, bc);
                    amount = atof(bc);
                    iter->sellValue = amount; cout << "改变后价格为：" << iter->getPrice() << endl;
                    strcpy_s(ab, "改变后价格为：\0"); floatab(ab, iter->getsellValue());
                    getstr(servSock, ab, bc);
                }
            }
            this->operate(servSock);
            break;
        }
        default:
            system("cls");
            cout << "无效的选项" << endl; strcpy_s(ab, "无效的选项\0");
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

int main() {                                                  //主程序负责利用socket进行与客户端进行连接
    shoppingsystem mysystem;
    if (!mysystem.init()) {
        cout << "无初始化" << endl;
        mysystem = shoppingsystem();
    }
    WSADATA wsaData;
    int initial_flag;
    initial_flag = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (initial_flag != 0) {
        cout << "SOCKET初始化失败！" << endl;
        return 0;
    }
    SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);        //创建套接字
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));                   //每个字节都用0填充
    sockAddr.sin_family = PF_INET;                            //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");        //具体的IP地址
    sockAddr.sin_port = htons(1234);                          //端口
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));   //绑定套接字
    listen(servSock, 20);                                     //进入监听状态
    mysystem.mainsystem(servSock);
    closesocket(servSock);                                    //关闭套接字
    WSACleanup();                                             //终止 DLL 的使用
    return 0;
}