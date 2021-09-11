#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

const int MAXBUF = 2048;

class Application
{
public:
    enum strValue
    {
        help,
        regis,
        login,
        logout,
        addcart,
        redcart,
        //delcart,
        settle,
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
	//~Application() {};
	vector <string> splitStr(const string& str, const char split = ' ');

	int exec(const string& ip, const string& port);

	bool islogged() const { return logged; }

	string getUname() const { return uname; }
	void setUname(const string& un) { uname = un; }
	string getPassword() const { return password; }
	void setPassword(const string& pwd) { password = pwd; }

private:
	string uname;
	string password;
	bool logged;
	//Trade* trade;
	int token;
};
