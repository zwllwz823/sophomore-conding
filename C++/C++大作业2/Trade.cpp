#pragma once

#include "trade.h"
#include <iostream>
#include <fstream>
using namespace std;

Trade::~Trade(){
	for (auto uit : userList)
		delete uit;
}

Trade::Trade() {};

bool Trade::addCart(const string& uname, const string& name, int q) {//根据用户名在购物车中加入对应数量的商品
	if (haveComm(name) && haveUser(uname)) {
		for (auto it1 : userList) {
			if (uname.compare(it1->getname()) == 0 && it1->getUserType() == User::type::consumer) {
				auto it2 = static_cast<Consumer *>(it1);
				it2->addcart(name, q);
				return true;
			}
		}
	}
	return false;
}

bool Trade::redCart(const string& uname, const string& name, int q) {
	if (haveComm(name) && haveUser(uname)) {
		for (auto it1 : userList) {
			if (uname.compare(it1->getname()) == 0 && it1->getUserType() == User::type::consumer) {
				auto it = static_cast<Consumer*>(it1);
				it->redcart(name, q);
				return true;
			}
		}
	}
	return false;
}

bool Trade::clearCart(const string& uname) {
	for (auto it1 : userList) {
		if (uname.compare(it1->getname()) == 0 && it1->getUserType() == User::type::consumer) {
			Consumer* it = dynamic_cast<Consumer*>(it1);
			it->cart.erase(it->cart.begin(), it->cart.end());
			return true;
		}
	}
	return false;
}

bool Trade::buy(const string& uname) {
	for (auto it1 : userList) {
		if (uname.compare(it1->getname()) == 0 && it1->getUserType() == User::type::consumer) {
			double sum = 0.0;
			Consumer* uit = dynamic_cast<Consumer*>(it1);
			auto cart = uit->getcart();
			/*
			for (auto it2 : cart) {
				cout << it2.first << ":" << it2.second << endl;
			}
			*/
			dynamic_cast<Merchant*>(it1);
			for (auto it2 : cart) {
				double price = getPrice(it2.first);
				if (it2.second > getQuantity(it2.first)) {
					cout << it2.first << " " << "not have enough quantity to offer!" << endl;
					return false;
				}
				changeQuantity(it2.first, getOwner(it2.first), getQuantity(it2.first) - it2.second);
				sum += price * it2.second;
			}
			if (sum > it1->getBalance()) {
				for (auto it2 : cart) 
					changeQuantity(it2.first, getOwner(it2.first), getQuantity(it2.first) + it2.second);
				cout << it1->getname() << " " << "not have enough money!" << endl;
				return false;
			}
			cout << "Buy : " << endl;
			for (auto it2 : cart) {
				addBalance(getOwner(it2.first), it2.second * getPrice(it2.first));
				cout << it2.first << ":" << it2.second << endl;
			}
			it1->setBalance(it1->getBalance() - sum);
			clearCart(it1->getname());
			return true;
		}
	}
	return false;
}

string Trade::getOwner(const std::string& name) {
	for (auto it : commList) {
		if (name.compare(it->getName()) == 0)
			return it->getOwner();
	}
	return "";
}

bool Trade::readUserFile(bool quiet, const string& fp) {
	cout << "Start Reading user data file : " << fp << endl;
	ifstream f(fp, ios::in);
	if (!f.good()) {
		cout << "File not exist!" << endl;
		return quiet;
	}
	string temp_name, temp_password;
	int temp_type, temp_balance;
	f >> temp_name;
	while (!f.eof()) {
		f >> temp_password >> temp_type >> temp_balance;
		User* u = nullptr;
		switch (temp_type) {
		case 0:
			u = new Consumer(temp_name, temp_password, temp_balance);
			break;
		case 1:
			u = new Merchant(temp_name, temp_password, temp_balance);
			break;
		}
		if (u != nullptr)
			userList.push_back(u);
		f >> temp_name;
	}
	f.close();
	//cout << userList.size();
	cout << "Read user file done." << endl;
	return true;
}

bool Trade::readCommFile(bool quiet, const string& fp) {
	cout << "Start Reading user commodity file : " << fp << endl;
	ifstream f(fp, ios::in);
	if (!f.good()) {
		cout << "File not exist!" << endl;
		return quiet;
	}
	string temp_name, temp_commType, temp_owner, temp_description;
	double temp_price, temp_percent;
	int temp_quantity;
	while (!f.eof()) {
		f >>temp_name >> temp_commType >> temp_owner >> temp_description >> temp_price >> temp_percent >> temp_quantity;
		commMap["book"] = commType::book;
		commMap["food"] = commType::food;
		commMap["cloth"] = commType::cloth;
		Commodity* u = nullptr;
		auto cur_type = commMap.find(temp_commType);
		if (cur_type != commMap.end()) {
			//cout << commMap[temp_commType] << endl;
			switch (commMap[temp_commType]) 
			{
			case 0:
				u = new Book(temp_price, temp_percent, temp_quantity,temp_commType, temp_name, temp_owner, temp_description);
				break;
			case 1:
				u = new Food(temp_price, temp_percent, temp_quantity, temp_commType, temp_name, temp_owner, temp_description);
				break;
			case 2:
				u = new Cloth(temp_price, temp_percent, temp_quantity, temp_commType, temp_name, temp_owner, temp_description);
				break;
			}
		}
		//cout << commList.size();
		if (u != nullptr)
			commList.push_back(u);
		//cout << commList.size();
	}
	//cout << commList.size();
	f.close();
	cout << "Read commodity file done." << endl;
	return true;
}

bool Trade::saveUserFile(bool quiet, const string& fp) const {
	cout << "Start Saving user data file : " << fp << endl;
	ofstream f(fp, ios::out);
	for (auto it : userList) {
		if (it->getname().compare(adminName) == 0)
			continue;
		User::type cur_type = it->getUserType();
		int type;
		switch (cur_type) {
		case User::type::consumer:
			type = 0;
			break;
		case User::type::merchant:
			type = 1;
			break;
		}
		f << it->getname()
			<< " " << it->getpassword()
			<< " " << type
			<< " " << it->getBalance() << endl;
	}
	f.close();
	cout << "Save user data file done." << endl;
	return true;
}

bool Trade::saveCommFile(bool quiet, const string& fp) const {
	cout << "Start Saving commodity data file : " << fp << endl;
	ofstream f(fp, std::ios::out);
	for (auto it : commList) {
		f << it->getName()
			<< " " << it->getComType()
			<< " " << it->getOwner()
			<< " " << it->getDesc()
			<< " " << it->getoriginprice()
			<< " " << it->getRealDiscount()
			<< " " << it->getQuantity() << endl;
	}
	f.close();
	cout << "Save commodity file done." << endl;
	return true;
}

bool Trade::haveUser(const string& username) const {
	for (auto it : userList) {
		if (username.compare(it->getname()) == 0)
			return true;
	}
	return false;
}

bool Trade::checkPassword(const string& username, const string& password) const {
	for (auto it : userList) {
		if (username.compare(it->getname()) == 0) {
			if (password.compare(it->getpassword()) == 0)
				return true;
			else return false;
		}
	}
	return false;
}

bool Trade::setPassword(const string& username, const string& password) const {
	for (auto it : userList) {
		if (username.compare(it->getname()) == 0) {
			it->setpassword(password);
			return true;
		}
	}
	return false;
}

bool Trade::addUser(const string& username, const string& password, int type) {
	if (haveUser(username))
		return false;
	User *u = nullptr;
	switch (type)
	{
	case 0:
		u = new Consumer(username, password, 0);
		break;
	case 1:
		u = new Merchant(username, password, 0);
		break;
	}
	if (u != nullptr){
		userList.push_back(u);
		return true;
	}
	return false;
}

bool Trade::delUser(const string& username){
	for (auto it = userList.begin(); it != userList.end(); it++) {
		if (username.compare((*it)->getname()) == 0) {
			User* temp = *it;
			userList.erase(it);
			delete temp;
			return true;
		}
	}
	return false;
}

bool Trade::getUserInfo(const string& username) const {
	bool flag = 0;
	if (username == "*" || haveUser(username)) {
		cout << "User Info:" << endl;
		for (auto it : userList) {
			if (username == "*" || username.compare(it->getname()) == 0) {
				cout << "Name :" << it->getname() << endl
					<< "Type :" << it->getUserType() << endl
					<< "Balance :" << it->getBalance() << endl;
				flag = 1;
			}
		}
	}
	if (!flag)
		cout << "Can't find the user!" << endl;
	return flag;
}

double Trade::getBalance(const string& username) const {
	for (auto it : userList) {
		if (username.compare(it->getname())==0)
			return it->getBalance();
	}
	return -1;
}

bool Trade::setBalance(const string& uname, double b) {
	for (auto it : userList) {
		if (uname.compare(it->getname()) == 0) {
			it->setBalance(b);
			return true;
		}
	}
	return false;
}

bool Trade::addBalance(const string& uname, double b) {
	for (auto it : userList) {
		if (uname.compare(it->getname()) == 0) {
			it->addBalance(b);
			return true;
		}
	}
	return false;
}

bool Trade::redBalance(const string& uname, double b) {
	for (auto it : userList) {
		if (uname.compare(it->getname())==0) {
			return it->redBalance(b);
		}
	}
	return false;
}

bool Trade::haveComm(const string& name) const {
	for (auto it : commList) {
		if (name.compare(it->getName()) == 0)
			return true;
	}
	return false;
}

void Trade::listComm() const {
	for (auto it : commList) {
		cout << "Name :" << it->getName() << endl
			<< "Description :" << it->getDesc() << endl
			<< "Owner :" << it->getOwner() << endl
			<< "Price :" << it->getPrice() << endl
			<< "Type :" << it->getComType() << endl
			<< "Quantity :" << it->getQuantity() << endl;
	}
}

void Trade::listComm(const string& name, const string& comType, const string& uname) const {
	for (auto it : commList) {
		if ((name == "*" || it->getName().find(name) != string::npos) && (comType == "*" || comType.compare(it->getComType()) == 0) && (uname == "*" || uname.compare(it->getOwner()) == 0)) {
			cout << "Name :" << it->getName() << endl
				<< "Description :" << it->getDesc() << endl
				<< "Owner :" << it->getOwner() << endl
				<< "Price :" << it->getPrice() << endl
				<< "Type :" << it->getComType() << endl
				<< "Quantity :" << it->getQuantity() << endl;
		}
	}
}

bool Trade::addComm(const string& name, const string& uname, const string& comType, double price, const string& desc) {
	for (auto it : commList) {
		if (name.compare(it->getName()) == 0)
			return false;
	}
	commMap["book"] = commType::book;
	commMap["food"] = commType::food;
	commMap["cloth"] = commType::cloth;
	Commodity* u = nullptr;
	auto cur_type = commMap.find(comType);
	if (cur_type != commMap.end()) {
		switch (commMap[comType])
		{
		case book:
			u = new Book(price, 1, 0, comType, name, uname, desc);
			break;
		case food:
			u = new Food(price, 1, 0, comType, name, uname, desc);
			break;
		case cloth:
			u = new Cloth(price, 1, 0, comType, name, uname, desc);
			break;
		}
	}
	if (u != nullptr) {
		commList.push_back(u);
		return true;
	}
	return false;
}

bool Trade::delComm(const string& name, const string& uname) {
	for (auto it=commList.begin();it!=commList.end();it++){
		if (name.compare((*it)->getName()) == 0 && uname.compare((*it)->getOwner()) == 0) {
			Commodity* temp = *it;
			commList.erase(it);
			delete temp;
			return true;
		}
	}
	return false;
}

bool Trade::changeQuantity(const string& name, const string& uname, int q) {
	for (auto it : commList) {
		if (name.compare(it->getName()) == 0 && (uname.compare(it->getOwner()) == 0 || uname == adminName)) {
			it->setQuantity(q);
			return true;
		}
	}
	return false;
}

int Trade::getQuantity(const string& name) const {
	int total = -1;
	for (auto it : commList) {
		if (name.compare(it->getName()) == 0)
			return it->getQuantity();
	}
	return total;
}

bool Trade::setPrice(const string& name, const string& uname, double p) {
	for (auto it : commList) {
		if (name.compare(it->getName()) == 0 && uname.compare(it->getOwner()) == 0) {
			it->setOriginPrice(p);
			return true;
		}
	}
	return false;
}

double Trade::getPrice(const string& name) const {
	for (auto it : commList) {
		if (name.compare(it->getName()) == 0) {
			return it->getPrice();
		}
	}
	return -1;
}

bool Trade::setPercent(const string& name, const string& uname, double p) {//某商家的特定商品打折
	for (auto it : commList) {
		if (name.compare(it->getName()) == 0 && (uname.compare(it->getOwner()) == 0 || uname == adminName)) {
			it->setPercent(p);
			return true;
		}
	}
	return false;
}

bool Trade::setPercent(double p, const string& type, const string& uname) {//同一品种下所有商品打折
	for (auto it : commList) {
		if (type.compare(it->getComType()) == 0 && (uname.compare(it->getOwner()) == 0 || uname == adminName)) {
			//it->setPercent(p);
			it->setDiscount(p);
			/*auto type = commMap.find(it->getComType());
			if (type != commMap.end()) {
				switch (commMap[it->getComType()])
				{
				case book:
					Book::discount = p;
				}
			}
			*/
			return true;
		}
	}
	return false;
}