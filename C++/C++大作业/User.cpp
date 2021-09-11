#pragma once

#include "user.h"
#include <iostream>

User::User(const string& n, const string& pw, const double b):name(n),password(pw),balance(b){}
Consumer::Consumer(const string& n, const string& pw, const double b) : User(n, pw, b){}
Merchant::Merchant(const string& n, const string& pw, const double b) : User(n, pw, b){}

bool Consumer::addcart(string goods, int num) {
	bool flag = 0;
	vector<pair<string, int>>::iterator iter;
	for (iter = cart.begin(); iter != cart.end(); iter++) {
		if (goods.compare(iter->first) == 0) {
			(iter->second) += num;
			flag = 1;
		}
	}
	if (!flag) {
		cart.push_back(pair<string, int>(goods, num));
		flag = 1;
	}
	return flag;
}