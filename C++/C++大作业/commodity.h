#pragma once

#include <string>
using namespace std;

class Commodity{
private:
    double price;
    double percent;//可对单一商品进行打折
    int quantity;
    string comType;
    string name;
    string owner;
    string description;

public:
    double getoriginprice() const {return price;}
    void setOriginPrice(double pr) { price = pr; }

    int getQuantity() const { return quantity; }
    void setQuantity(int q) { quantity = q; }

    double getPercent() const { return percent; }
    void setPercent(double p) { percent = p; }
    //virtual double getRealDiscount() const { return getPercent(); }
    
    virtual double getPrice() = 0;
    virtual void setDiscount(double d) = 0;
    virtual double getDiscount() = 0;
    virtual double getRealDiscount() = 0;

    string getComType() const { return comType; }
    void setComType(const string &t) { comType = t; }

    string getName() const { return name; }
    void setName(const string &n) { name = n; }

    string getOwner() const { return owner; }
    string getDesc() const { return description; }

    Commodity(double pr, double pe, int q, const string &ct, const string &n, const string &own,const string &desc);

    bool buy(int count);
    bool isType(const string &t);
};

class Food : public Commodity
{
protected:
    static double discount;

public:
    double getPrice()  { return getoriginprice() * getPercent() * discount; }
    void setDiscount(double d) { discount = d; }
    double getDiscount() { return discount; }
    double getRealDiscount() { return discount * getPercent(); }

    Food(double pr, double pe, int q, const string &ct, const string &n, const string &own, const string &desc);
};

class Book : public Commodity
{
protected:
    static double discount;//种类打折

public:
    double getPrice()  { return getoriginprice() * getPercent() * discount; }
    void setDiscount(double d) { discount = d; }
    double getDiscount() { return discount; }
    double getRealDiscount()  { return discount * getPercent(); }

    Book(double pr, double pe, int q, const string &ct, const string &n, const string &own, const string &desc);
};

class Cloth : public Commodity
{
protected:
    static double discount;

public:
    double getPrice()  { return getoriginprice() * getPercent() * discount; }
    void setDiscount(double d) { discount = d; }
    double getDiscount() { return discount; }
    double getRealDiscount()  { return discount * getPercent(); }

    Cloth(double pr, double pe, int q, const string &ct, const string &n, const string &own, const string &desc);
};