#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>
using namespace std;

#define BUF_SIZE 1000

class trade
{                                                 //交易类，处理商品交易
private:
    list<Book> dataList1;                                     //商品数据链表
    list<Cloth> dataList2;
    list<Food> dataList3;

public:
    float getshangPrice(char shangname[]) {
        for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++) {
            if (strcmp(shangname, iter->name) == 0)
                return iter->getsellValue();
        }
        for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++) {
            if (strcmp(shangname, iter->name) == 0)
                return iter->getsellValue();
        }
        for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++) {
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
    void  getstr(SOCKET servSock, char ab[], char recvstr[])    //从客户端接受字符串，并发去下一步指示
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
    void AddNew(char orner[], char name[], float bayValue, float sellValue, int leibie, char introduce[]);//添加新商品
    void save() {
        this->save1();
        this->save2();
        this->save3();
    }
    char* Getname(int index);                                   //得到商品的名字
    char* Getorner(int index);                                  //得到商品所属的商家
    float Getmoney(int index);                                  //得商品的价钱
    int Getleibie(int index);                                   //得到商品的类别
    int Getstock(int index);                                    //得到商品的库存
    int Getit(int index) {                                      //查看是否含有该商品

        for (auto iter = dataList1.begin(); iter != dataList1.end(); iter++)
        {
            if (iter->getID() == index)
            {

                return 1;

            }
        }
        for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++)
        {
            if (iter->getID() == index)
            {
                return 2;
            }
        }
        for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++)
        {
            if (iter->getID() == index)
            {
                return 3;
            }
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
    void newstr(char str[], int index, char name[], float BuyValue, float SellValue, int Stock, char Orner[], char introduce[])
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
        strcat(ab, Orner);
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
                this->newstr(str, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getOrner(), iter->getIntroduce());
                getstr(servSock, str, bc);
                printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n",
                    iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getOrner());
            }

        }
        for (auto iter = dataList2.begin(); iter != dataList2.end(); iter++)
        {
            if (strcmp(iter->getName(), name) == 0)
            {
                find += 1;
                char str[BUF_SIZE];
                this->newstr(str, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getOrner(), iter->getIntroduce());
                getstr(servSock, str, bc);

                printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n",
                    iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getOrner());
            }
        }
        for (auto iter = dataList3.begin(); iter != dataList3.end(); iter++)
        {
            if (strcmp(iter->getName(), name) == 0)
            {
                find += 1;
                char str[BUF_SIZE];
                this->newstr(str, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getOrner(), iter->getIntroduce());
                getstr(servSock, str, bc);
                printf("商品编号：%d\n 商品名称：%s\n 购入价格：%f\n 出售价格：%f\n 剩余：%d\n 商家：%s\n",
                    iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getOrner());
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