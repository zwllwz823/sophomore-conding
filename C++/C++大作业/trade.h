#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>
using namespace std;

#include "user.h"
#include "commodity.h"

const string adminName = "admin";
const string adminPassword = "123456";


class Trade{
private:
    vector<User *> userList;
    vector<Commodity *> commList;

    enum commType{//商品种类
        book,food,cloth
    };
    map<string,commType> commMap;

public:
    Trade();
    ~Trade();

    bool readUserFile(bool quiet = false, const string &fp = "userdata.txt");
    bool saveUserFile(bool quiet = false, const string &fp = "userdata.txt") const;
    bool readCommFile(bool quiet = false, const string &fp = "commdata.txt");
    bool saveCommFile(bool quiet = false, const string &fp = "commdata.txt") const;

    bool haveUser(const string &username) const;
    bool checkPassword(const string &username,const string &password) const;
    bool setPassword(const string &username,const string &password) const;
    bool addUser(const string& username, const string& password, int type);
    bool delUser(const string& username);
    bool getUserInfo (const string &username) const;
    double getBalance (const string &uname) const;
    bool setBalance (const string &uname,double b);
    bool addBalance (const string &username,double b);
    bool redBalance(const string& uname, double b);

    /*
    bool buy(const string &uname);
    bool clearCart(const string &uname);
    bool addCart(const string &uname,const string &name,int q);
    */
    bool haveComm(const string &name) const;
    void listComm() const;
    void listComm(const string &name, const string &comType = "*", const string &uname = "") const;
    bool addComm(const string &name, const string &uname, const string &comType, double price, const string &desc);
    bool delComm(const string &name, const string &uname);
    bool changeQuantity(const string &name, const string &uname, int q);
    int getQuantity(const string &name) const;
    bool setPrice(const string &name, const string &uname, double p);
    double getPrice(const string &name) const;
    bool setPercent(const string &name, const string &uname, double p);//特定人特定商品打折
    bool setPercent(double p, const string &type, const string &uname);//特定人对应种类打折
};