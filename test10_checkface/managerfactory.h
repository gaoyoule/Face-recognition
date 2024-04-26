#ifndef MANAGERFACTORY_H
#define MANAGERFACTORY_H
#include <QDebug>
#include <QSqlDatabase>
#include "userdao.h"
#include "mytime.h"

//用单例模式模拟工厂模式
//1.数据库模块 2.人员表管理 3.时间表管理
class ManagerFactory
{
    //私有的构造和拷贝构造
    ManagerFactory();
    ManagerFactory(const ManagerFactory&);
    //私有静态类指针
    static ManagerFactory *ins;
    //创建数据库对象
    QSqlDatabase db;
public:
    //公有的静态成员函数
    static ManagerFactory* getIns();
    //人员表指针
    UserDao *user;
    //时间表指针
    Mytime *time;
    ~ManagerFactory();
};

#endif // MANAGERFACTORY_H
