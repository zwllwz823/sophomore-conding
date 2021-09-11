#pragma once

#include <string>
#include "trade.h"
using namespace std;

class Application
{
public:
    enum strValue
    {
        help,
        regis,
        login,
        logout,
        recharge,
        ls,
        lsall,
        lsu,
        addcomm,
        chquantity,
        chpr,
        chpercent,
        chtpercent,
        quit,
        setpw,
        withdraw
    };

	Application(const string &un,const string &pwd);
	~Application() {};
	vector <string> splitStr(const string& str, const char split = ' ');
	int exec();

	bool islogged() const { return logged; }
	string getUname() const { return uname; }
	void setUname(const string& un) { uname = un; }
	string getPassword() const { return password; }
	void setPassword(const string& pwd) { password = pwd; }

private:
	string uname;
	string password;
	bool logged;
	Trade* trade;
	//int token;
};
