#include "userdao.h"

UserDao::UserDao()
{
    //创建操作数据库类对象
    QSqlQuery query;
    //执行语句
    bool ok = query.exec("create table if not exists user ( "
                         "id integer primary key autoincrement, "
                         "name varchar(20));");
    //判断是否执行
    if(ok)
    {
        qDebug()<<"创建人员表";
    }
    else
    {
        qDebug()<<"创建人员表失败";
    }
}

int UserDao::insertUser(QString name)
{
    //创建操作数据库类对象
    QSqlQuery query;
    //准备语句
    query.prepare("insert into user(name) values(?);");
    //绑定数据
    query.bindValue(0,name);
    //执行语句
    bool ok = query.exec();
    int id = 0;
    //判断是否执行
    if(ok)
    {
        //查询最新插入的id
        query.exec("select last_insert_rowid();");
        //判断是否有数据
        if(query.next())
        {
            //获取id
            id = query.record().value(0).toInt();
        }
    }
    else
    {
        qDebug()<<"插入人员表失败";
    }
    return id;
}

QString UserDao::selectName(int id)
{
    //创建操作数据库类对象
    QSqlQuery query;
    //准备语句
    query.prepare("select name from user where id = ?;");
    //绑定数据
    query.bindValue(0,id);
    //执行语句
    bool ok = query.exec();
    //判断是否执行
    if(ok)
    {
        qDebug()<<  "查询人员表成功";
    }
    else
    {
        qDebug()<<  "查询人员表失败";
    }
    QString name;
    if(query.next())
    {
        name = query.record().value(0).toString();
    }

    return name;
}

UserDao::~UserDao()
{

}

