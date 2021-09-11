#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>
using namespace std;

#define BUF_SIZE 1000

class trade
{                                                 //�����࣬������Ʒ����
private:
    list<Book> dataList1;                                     //��Ʒ��������
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
    void  getstr(SOCKET servSock, char ab[], char recvstr[])    //�ӿͻ��˽����ַ���������ȥ��һ��ָʾ
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
    void AddNew(char orner[], char name[], float bayValue, float sellValue, int leibie, char introduce[]);//�������Ʒ
    void save() {
        this->save1();
        this->save2();
        this->save3();
    }
    char* Getname(int index);                                   //�õ���Ʒ������
    char* Getorner(int index);                                  //�õ���Ʒ�������̼�
    float Getmoney(int index);                                  //����Ʒ�ļ�Ǯ
    int Getleibie(int index);                                   //�õ���Ʒ�����
    int Getstock(int index);                                    //�õ���Ʒ�Ŀ��
    int Getit(int index) {                                      //�鿴�Ƿ��и���Ʒ

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
    void manage(char name[], SOCKET& servSock);                  //�̼ҹ���ϵͳ
    bool init()
    {
        bool i1, i2, i3;
        i1 = this->init1(); i2 = init2(); i3 = init3();
        if (i1 || i2 || i3) return true;
        else return false;
    };                                                          //�ӱ����ļ���ȡ��Ʒ��Ϣ
    void newstr(char str[], int index, char name[], float BuyValue, float SellValue, int Stock, char Orner[], char introduce[])
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
        strcat(ab, Orner);
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
                this->newstr(str, iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getOrner(), iter->getIntroduce());
                getstr(servSock, str, bc);
                printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n",
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

                printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n",
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
                printf("��Ʒ��ţ�%d\n ��Ʒ���ƣ�%s\n ����۸�%f\n ���ۼ۸�%f\n ʣ�ࣺ%d\n �̼ң�%s\n",
                    iter->getID(), iter->getName(), iter->getBuyValue(), iter->getsellValue(), iter->getStock(), iter->getOrner());
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