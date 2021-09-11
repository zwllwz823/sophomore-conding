#pragma once

#include <string>
using namespace std;

class Goods {                                           //商品基类
public:
    float price;
    virtual float getPrice() = 0;                           //虚函数
};

class commodity :virtual public Goods {                  //继承Goods
public:
    int static kinds;                                       //商品种类
    int ID;                                                 //货品编号
    int stock;                                              //货存量
    float buyValue;                                         //进货的价格
    float sellValue;                                        //卖出价
    char name[30];                                          //商品名
    char owner[30];                                         //商品所属商家
    char introduce[100];                                     //介绍
public:
    int getID();                                            //返回ID
    int getStock();                                         //返回存储的数量
    float getBuyValue();                                    //返回进价
    float getsellValue();                                   //返回售价
    char* getName();                                        //返回商品名字
    char* getIntroduce() {                                  //返回介绍
        return this->introduce;
    }
    void UpdateStock(int n);                                //更新库存 
    char* getowner();                                       //返回商品所属的商家名
    commodity() {};                                         //构造函数
    commodity(char owner[], char name[], float buyValue, float sellValue, char introduce[]) {
        kinds++;
        this->ID = kinds;
        this->stock = 0;
        this->buyValue = buyValue;
        this->sellValue = sellValue; this->price = sellValue;
        strcpy_s(this->name, name); strcpy_s(this->owner, owner); strcpy_s(this->introduce, introduce);
    };
    commodity(int ID, char owner[], char name[], float buyValue, float sellValue, int stock, char introduce[])
    {
        // kinds++;
        this->ID = ID;
        this->stock = stock;
        this->buyValue = buyValue;
        this->sellValue = sellValue; this->price = sellValue;
        strcpy_s(this->name, name); 
        strcpy_s(this->owner, owner); 
        strcpy_s(this->introduce, introduce);
    }
    float getPrice()
    {
        return this->price;
    }
};
class Book :public commodity {                                 //书籍类，继承商品
private:
    int leibie = 1;
    // char owner[30];
public:
    Book(char owner[], char name[], float buyValue, float sellValue, char introduce[])
    {                                                          //构造函数
        kinds++;
        this->ID = kinds;
        this->stock = 0;
        this->buyValue = buyValue;
        this->sellValue = sellValue; this->price = sellValue;
        strcpy_s(this->name, name);
        strcpy_s(this->owner, owner); 
        strcpy_s(this->introduce, introduce);
    }
    Book(int ID, char owner[], char name[], float buyValue, float sellValue, int stock, char introduce[])
    {
        // kinds++;
        this->ID = ID;
        this->stock = stock;
        this->buyValue = buyValue;
        this->sellValue = sellValue; this->price = sellValue;
        strcpy_s(this->name, name); 
        strcpy_s(this->owner, owner);
        strcpy_s(this->introduce, introduce);
    }


};
class Cloth :public commodity {                                //衣服类
private:
    int leibie = 2;
    //char owner[30];
public:                                                        //构造函数
    Cloth(char owner[], char name[], float buyValue, float sellValue, char introduce[])
    {
        kinds++;
        this->ID = kinds;
        this->stock = 0;
        this->buyValue = buyValue;
        this->sellValue = sellValue; this->price = sellValue;
        strcpy_s(this->name, name); 
        strcpy_s(this->owner, owner); 
        strcpy_s(this->introduce, introduce);
    };
    Cloth(int ID, char owner[], char name[], float buyValue, float sellValue, int stock, char introduce[])
    {
        // kinds++;
        this->ID = ID;
        this->stock = stock;
        this->buyValue = buyValue;
        this->sellValue = sellValue; this->price = sellValue;
        strcpy_s(this->name, name); 
        strcpy_s(this->owner, owner); 
        strcpy_s(this->introduce, introduce);
    }

};
class Food :public commodity {                                 //食品类
private:
    int leibie = 3;
    // char owner[30];
public:                                                        //构造函数
    Food(char owner[], char name[], float buyValue, float sellValue, char introduce[])
    {
        kinds++;
        this->ID = kinds;
        this->stock = 0;
        this->buyValue = buyValue;
        this->sellValue = sellValue; this->price = sellValue;
        strcpy_s(this->name, name); 
        strcpy_s(this->owner, owner); 
        strcpy_s(this->introduce, introduce);
    };
    Food(int ID, char owner[], char name[], float buyValue, float sellValue, int stock, char introduce[])
    {
        // kinds++;
        this->ID = ID;
        this->stock = stock;
        this->buyValue = buyValue;
        this->sellValue = sellValue; this->price = sellValue;
        strcpy_s(this->name, name); 
        strcpy_s(this->owner, owner); 
        strcpy_s(this->introduce, introduce);
    }

};