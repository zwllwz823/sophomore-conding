#pragma once
#include "application.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

Application::Application(const string& un, const string& pwd) :uname(un), password(pwd), logged(false) {
	trade = new Trade;
	trade->readCommFile();
	trade->readUserFile();
	logged = false;
}

vector <string> Application::splitStr(const string& str, const char split) {
    vector<string> ret;
    istringstream iss(str);
    string buf;
    while (getline(iss, buf, split))
        ret.push_back(buf);
    return ret;
}

int Application::exec() {
    cout << "欢迎来到电商交易系统2\n"
        << "开发者：赵文龙\n"
        << "以下为详细功能：\n"
        << "help : help information\n"
        << "regis <name> <password> <0/1>:register a user\n"
        << "login <name> <password>:login as a user.\n"
        << "logout :logout from service.\n"
        << "addcart <commodity name> <number> : add a commodity to your cart.\n"
        << "redcart <commodity name> <number> : reduce a commodity from your cart.\n"
        << "settle : settle.\n"
        << "recharge <number> : recharge money.\n"
        << "withdraw <number> : withdraw money.\n"
        << "ls <commodity name> <commodity type> : list commodity\n"
        << "lsall : list all commodity.\n"
        << "lsu <username> : list a user info\n"
        << "addcomm <commodity name> <commodity type> <price> <description>: add a commdity.\n"
        << "chquantity <commodity name> <number> : change quantity\n"
        << "chpr <commodity name> <number> : change price\n"
        << "chpercent <commodity name> <number> : change goods discount\n"
        << "chtpercent <type> <number> : change type discount\n"
        << "quit : quit\n";

    map<string, Application::strValue> strMap;
    strMap["help"] = strValue::help;
    strMap["regis"] = strValue::regis;
    strMap["login"] = strValue::login;
    strMap["logout"] = strValue::logout;
    strMap["addcart"] = strValue::addcart;
    strMap["redcart"] = strValue::redcart;
    //strMap["delcart"] = strValue::delcart;
    strMap["settle"] = strValue::settle;
    strMap["recharge"] = strValue::recharge;
    strMap["withdraw"] = strValue::withdraw;
    strMap["ls"] = strValue::ls;
    strMap["lsall"] = strValue::lsall;
    strMap["lsu"] = strValue::lsu;
    strMap["addcomm"] = strValue::addcomm;
    strMap["chquantity"] = strValue::chquantity;
    strMap["chpr"] = strValue::chpr;
    strMap["chpercent"] = strValue::chpercent;
    strMap["chtpercent"] = strValue::chtpercent;
    strMap["quit"] = strValue::quit;
    strMap["setpw"] = strValue::setpw;
    
    string getstring;
    while (getline(std::cin, getstring)) {
        auto argv = splitStr(getstring,' ');
        if (argv.size() > 0) {
            auto it = strMap.find(argv[0]);
            if (it != strMap.end()) {
                switch (strMap[argv[0]])
                {
                case addcart:
                {
                    if (argv.size() < 3) {
                        cout << "Invalid input!Please input again!" << endl;
                        break;
                    }
                    else {
                        istringstream iss(argv[2]);
                        int num;
                        if (!(iss >> num)) {
                            std::cout << "Invalid input format!\n";
                            break;
                        }
                        else {
                            if (trade->haveComm(argv[1])) {
                                if (trade->addCart(uname, argv[1], num))
                                    cout << "Add to the cart successfully!" << endl;
                            }
                            else cout << argv[1] << " " << "not found!" << endl;
                        }
                        break;
                    }
                }

                case redcart:
                {
                    if (argv.size() < 3) {
                        cout << "Invalid input!Please input again!" << endl;
                        break;
                    }
                    else {
                        istringstream iss(argv[2]);
                        int num;
                        if (!(iss >> num)) {
                            std::cout << "Invalid input format!\n";
                            break;
                        }
                        else {
                            if (trade->haveComm(argv[1])) {
                                if (trade->redCart(uname, argv[1], num))
                                    cout << "Reduce from the cart successfully!" << endl;
                            }
                            else cout << argv[1] << " " << "not found!" << endl;
                        }
                        break;
                    }
                }

                case settle:
                {
                    if (trade->buy(uname))
                        cout << "Successfully purchased." << endl;
                    else
                        cout << "Failed to pay." << endl;
                }

                case help: 
                {
                    cout << "help : help information\n"
                        << "regis <name> <password> <0/1>:register a user\n"
                        << "login <name> <password>:login as a user.\n"
                        << "logout :logout from service.\n"
                        << "addcart <commodity name> <number> : add a commodity to your cart.\n"
                        << "redcart <commodity name> <number> : reduce commodity from your cart.\n"
                        << "settle : settle.\n"
                        << "recharge <number> : recharge money.\n"
                        << "withdraw <number> : withdraw money.\n"
                        << "ls <commodity name> <commodity type> : list commodity\n"
                        << "lsall : list all commodity.\n"
                        << "lsu <username> : list a user info\n"
                        << "addcomm <commodity name> <commodity type> <price> <description>: add a commodity\n"
                        << "chquantity <commodity name> <number> : change quantity\n"
                        << "chpr <commodity name> <number> : change price\n"
                        << "chpercent <commodity name> <number> : change discount\n"
                        << "chtpercent <type> <number>\n"
                        << "quit : quit\n";
                    break;
                }
                
                case regis:
                {
                    if (argv.size() < 4) {
                        std::cout << "Invalid Input! Please input again!\n";
                        break;
                    }
                    istringstream iss(argv[3]);
                    int t;
                    if (!(iss >> t)) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (iss >> getstring) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (trade->addUser(argv[1], argv[2], t)) 
                        std::cout << "User [" << argv[1] << "] has been added!\n"
                            << "Please login again!\n";
                    else
                        std::cout << "Failed to add new user!\n";
                }
                break;

                case login:
                {
                    if (argv.size() < 3) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (trade->checkPassword(argv[1], argv[2])) {
                        uname = argv[1];
                        password = argv[2];
                        logged = true;
                        std::cout << "Logged successfully!\n";
                    }
                    else
                        std::cout << "Wrong password! Fail to login.\n";
                    break;
                }
                case logout:
                {
                    logged = false;
                    cout << "Log out successfully!" << endl;
                    break;
                }
                case ls:
                {
                    if (argv.size() < 3) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    trade->listComm(argv[1], argv[2],"*");
                    break;
                }
                   
                case lsall:
                {
                    trade->listComm();
                    break;
                }
                   
                case addcomm:
                {
                    if (!islogged()) {
                        std::cout << "Not logged!\n";
                        break;
                    }
                    if (argv.size() < 4) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    istringstream iss(argv[3]); double p;
                    if (!(iss >> p)) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (iss >> getstring) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (trade->addComm(argv[1], uname, argv[2], p, argv[4]))
                        std::cout << "Add the commodity successfully.\n";
                    else
                        std::cout << "Failed to add the commodity.\n";
                }
                break;

                case chquantity:
                {
                    if (!islogged()) {
                        std::cout << "Not logged!\n";
                        break;
                    }
                    if (argv.size() < 3) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    istringstream iss(argv[2]); int q;
                    if (!(iss >> q)) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (iss >> getstring) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (trade->changeQuantity(argv[1], uname, q))
                        std::cout << "Change the quantity successfully.\n";
                    else
                        std::cout << "Failed to Change the quantity.\n";
                }
                break;

                case chpr:
                {
                    if (!islogged()) {
                        std::cout << "Not logged!\n";
                        break;
                    }
                    if (argv.size() < 3) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    istringstream iss(argv[2]); double p;
                    if (!(iss >> p)) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (iss >> getstring) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (trade->setPrice(argv[1], uname, p))
                        std::cout << "Set the price successfully.\n";
                    else
                        std::cout << "Failed to set the price.\n";
                }
                break;

                case chpercent:
                {
                    if (!islogged()) {
                        std::cout << "Not logged!\n";
                        break;
                    }
                    if (argv.size() < 3) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    istringstream iss(argv[2]); double p;
                    if (!(iss >> p)) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (iss >> getstring) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (trade->setPercent(argv[1],uname,p))
                        std::cout << "Set the discount successfully.\n";
                    else
                        std::cout << "Failed to set the discount.\n";
                }
                break;

                case chtpercent:
                {
                    if (!islogged()) {
                        std::cout << "Not logged!\n";
                        break;
                    }
                    if (argv.size() < 3) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    istringstream iss(argv[2]); double p;
                    if (!(iss >> p)) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (iss >> getstring) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (trade->setPercent(p,argv[1],uname))
                        std::cout << "Set the type's discount successfully.\n";
                    else
                        std::cout << "Failed to set the type's discount.\n";
                }
                break;

                case lsu:
                {
                    if (argv.size() < 2) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (trade->getUserInfo(argv[1]))
                        std::cout << "Get the information successfully.\n";
                    else
                        std::cout << "Failed to get the information.\n";
                    break;
                }
                    

                case quit: 
                {
                    trade->saveCommFile();
                    trade->saveUserFile();
                    std::cout << "Quit.\n";
                    return 0;
                    break;
                }
                   

                case setpw: 
                {
                    if (argv.size() < 2) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (!logged) {
                        cout << "Not having logged yet!" << endl;
                        break;
                    }
                    if (trade->setPassword(uname, argv[1]))
                        std::cout << "Set the password successfully.\n";
                    else
                        std::cout << "Failed to set the password.\n";
                    break;
                }
                   

                case recharge:
                {
                    if (argv.size() < 2) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    std::istringstream iss(argv[1]);
                    double b;
                    if (!(iss >> b)) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (iss >> getstring) {
                        std::cout << "Invalid input format!\n";
                        break;
                    }
                    if (trade->addBalance(uname, b))
                        cout << "Recharge RMB " << b << " successfully.\n";
                    else
                        cout << "Failed to recharge.\n";
                    break;
                }
                  
                case withdraw: {
                    if (argv.size() < 2) {
                        cout << "Invalid input format!\n";
                        break;
                    }
                    std::istringstream iss(argv[1]);
                    double b;
                    if (!(iss >> b)) {
                        cout << "Invalid input format!\n";
                        break;
                    }
                    if (iss >> getstring) {
                        cout << "Invalid input format!\n";
                        break;
                    }
                    if (trade->redBalance(uname, b))
                        cout << "Withdraw RMB " << b << " successfully.\n";
                    else
                        cout << "Failed to withdraw.\n";
                    break;
                }
                   
                default:
                    cout << "Illegal request: " << argv[0] << endl;
                    break;
                }
            }
            else {
                cout << "Illegal request: " << argv[0] << endl;
                break;
            }
        }
        //trade->saveCommFile();
        //trade->saveUserFile();
        if (islogged())
            std::cout << "Current user:" << uname << endl;
    }
    return 0;
}