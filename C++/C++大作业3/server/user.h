#pragma once

#include <string>
#include <vector>
#include <utility>
using namespace std;

class User {                                                 //�û�����
public:
    int usertype;                                              //�û�����
    char username[30];                                         //�û��� 
    char password[30];                                         //����
    float money;                                               //�����ʲ�
    int state;                                                 //״̬��0δ��¼��1��¼
public:
    User() {};
    virtual int getUsertype() = 0;                              //�麯��
};
class customer :virtual public User                           //����������
{
public:                                                         //���캯��
    customer(char username[30], char password[30], float money, int state)
    {
        this->usertype = 1;
        this->money = money;
        this->state = state;
        strcpy_s(this->username, username);
        strcpy_s(this->password, password);
    }
    int getUsertype()
    {
        return this->usertype;
    }
};
class merchant :virtual public User                           //�̼�����
{
public:
    merchant(char username[30], char password[30], float money, int state)
    {
        this->usertype = 2;
        this->money = money;
        this->state = state;
        strcpy_s(this->username, username);
        strcpy_s(this->password, password);
    }
    int getUsertype()
    {
        return this->usertype;
    }
};