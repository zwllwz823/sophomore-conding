#pragma once

#include <string>
using namespace std;

class Goods {                                           //��Ʒ����
public:
    float price;
    virtual float getPrice() = 0;                           //�麯��
};

class commodity :virtual public Goods {                  //�̳�Goods
public:
    int static kinds;                                       //��Ʒ����
    int ID;                                                 //��Ʒ���
    int stock;                                              //������
    float buyValue;                                         //�����ļ۸�
    float sellValue;                                        //������
    char name[30];                                          //��Ʒ��
    char owner[30];                                         //��Ʒ�����̼�
    char introduce[100];                                     //����
public:
    int getID();                                            //����ID
    int getStock();                                         //���ش洢������
    float getBuyValue();                                    //���ؽ���
    float getsellValue();                                   //�����ۼ�
    char* getName();                                        //������Ʒ����
    char* getIntroduce() {                                  //���ؽ���
        return this->introduce;
    }
    void UpdateStock(int n);                                //���¿�� 
    char* getowner();                                       //������Ʒ�������̼���
    commodity() {};                                         //���캯��
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
class Book :public commodity {                                 //�鼮�࣬�̳���Ʒ
private:
    int leibie = 1;
    // char owner[30];
public:
    Book(char owner[], char name[], float buyValue, float sellValue, char introduce[])
    {                                                          //���캯��
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
class Cloth :public commodity {                                //�·���
private:
    int leibie = 2;
    //char owner[30];
public:                                                        //���캯��
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
class Food :public commodity {                                 //ʳƷ��
private:
    int leibie = 3;
    // char owner[30];
public:                                                        //���캯��
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