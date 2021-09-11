#pragma once

#include "user.h"
#include <iostream>

User::User(const string& n, const string& pw, const double b):name(n),password(pw),balance(b){}
Consumer::Consumer(const string& n, const string& pw, const double b) : User(n, pw, b){}
Merchant::Merchant(const string& n, const string& pw, const double b) : User(n, pw, b){}

bool Consumer::addcart(string goods, int num) {
	bool flag = false;
	for (auto it=cart.begin();it!=cart.end();it++) {
		if (goods.compare((*it).first) == 0) {
			int cnt = (*it).second;
			//it.second += num;
			cart.erase(it);
			cart.push_back(pair<string,int>(goods,cnt+num));
			return true;
		}
	}
	if (!flag) {
		cart.push_back(pair<string, int>(goods, num));
		flag = true;
	}
	return flag;
}

bool Consumer::redcart(string goods, int num) {
	bool flag = 0;
	for (auto it = cart.begin(); it != cart.end(); it++) {
		if (goods.compare((*it).first) == 0) {
			int cnt = (*it).second;
			//it.second += num;
			cart.erase(it);
			cart.push_back(pair<string, int>(goods, cnt - num));
			return true;
		}
	}
	return flag;
}