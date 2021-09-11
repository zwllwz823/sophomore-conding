#pragma once

#include <string>
#include <vector>
#include <utility>
using namespace std;

class User
{
private:
    string name;
    string password;
    double balance;//���

public:
    enum type{
        consumer,merchant
    };
    virtual type getUserType()=0;

    double getBalance() const {return balance;}
    void setBalance(double b) {balance=b;}
    void addBalance(double b) { balance += b; }
    bool redBalance(double b) {
        if (balance >= b) {
            balance -= b;
            return true;
        }
        return false;
    }

    string getname() const {return name;}
    void setname(const string &n) {name=n;}

    string getpassword() const {return password;}
    void setpassword(const string &pw) {password=pw;}

    User(const string &n,const string &pw,const double b);
    virtual ~User(){};
};

class Consumer:public User
{
private:
public:
    vector<pair<string,int>> cart;//���ﳵ
    virtual type getUserType() override{return type::consumer;}//��д
    vector<pair<string,int>> getcart() const {return cart;}
    bool addcart(string goods,int num);

    Consumer(const string &m,const string &ps,const double b);
    virtual ~Consumer(){};

};

class Merchant:public User
{
private:
public:
    virtual type getUserType() override{return type::merchant;}//��д

    Merchant(const string &m,const string &ps,const double b);
    virtual ~Merchant(){};
};