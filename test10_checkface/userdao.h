#ifndef USERDAO_H
#define USERDAO_H
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
//人员管理
class UserDao
{
public:
    UserDao();//创建人员表
    int insertUser(QString name);//插入人员表
    QString selectName(int id);//获取人员名字
    ~UserDao();
};

#endif // USERDAO_H
