#pragma once

#include <string>
#include <vector>
#include <utility>
using namespace std;

class User {                                                 //用户基类
public:
    int usertype;                                              //用户类型
    char username[30];                                         //用户名 
    char password[30];                                         //密码
    float money;                                               //所持资产
    int state;                                                 //状态：0未登录，1登录
public:
    User() {};
    virtual int getUsertype() = 0;                              //虚函数
};
class customer :virtual public User                           //消费者子类
{
public:                                                         //构造函数
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
class merchant :virtual public User                           //商家子类
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