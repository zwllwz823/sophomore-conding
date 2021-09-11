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
    double balance;//余额

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
    virtual bool addcart(string goods, int num) = 0;
    virtual bool redcart(string goods, int num) = 0;

    string getpassword() const {return password;}
    void setpassword(const string &pw) {password=pw;}

    User(const string &n,const string &pw,const double b);
    virtual ~User(){};
};

class Consumer:public User
{
private:
public:
    vector<pair<string,int>> cart;//购物车
    virtual type getUserType() override{return type::consumer;}//重写
    vector<pair<string,int>> getcart() const {return cart;}
    bool addcart(string goods,int num);
    bool redcart(string goods, int num);
     
    Consumer(const string &m,const string &ps,const double b);
    virtual ~Consumer(){};

};

class Merchant:public User
{
private:
public:
    virtual type getUserType() override{return type::merchant;}//重写
    bool addcart(string goods, int num) { return true; };
    bool redcart(string goods, int num) { return true; };
    Merchant(const string &m,const string &ps,const double b);
    virtual ~Merchant(){};
};