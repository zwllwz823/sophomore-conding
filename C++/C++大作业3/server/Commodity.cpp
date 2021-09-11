#pragma once
#include "commodity.h"

double Book::discount = 1.0;
double Food::discount = 1.0;
double Cloth::discount = 1.0;

Commodity::Commodity(double pr, double pe, int q, const string& ct, const string& n, const string& own, const string& desc)
	:price(pr), percent(pe), quantity(q), comType(ct), name(n), owner(own), description(desc){}

Food::Food(double pr, double pe, int q, const string& ct, const string& n, const string& own, const string& desc)
	: Commodity(pr, pe, q, ct, n, own, desc){}

Cloth::Cloth(double pr, double pe, int q, const std::string& ct, const std::string& n, const std::string& own, const std::string& desc)
	: Commodity(pr, pe, q, ct, n, own, desc){}

Book::Book(double pr, double pe, int q, const std::string& ct, const std::string& n, const std::string& own, const std::string& desc)
	: Commodity(pr, pe, q, ct, n, own, desc) {}

bool Commodity::buy(int count) {
	if (count <= quantity) {
		quantity -= count;
		return true;
	}
	return false;
}

bool Commodity::isType(const string& t) {
	if (comType.compare(t) == 0)
		return true;
	return false;
}