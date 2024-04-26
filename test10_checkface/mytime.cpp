#include "mytime.h"

Mytime::Mytime()
{
    QSqlQuery query;
    bool ok = query.exec("create table if not exists mytime ( "
                         "name varchar(20) , time datetime );");
    if(ok)
        qDebug()<<  "创建时间表成功";
    else
        qDebug()<<  "创建时间表失败";
}

void Mytime::insertTime(QString name)
{
    QSqlQuery query;
    //数据库函数：获取当前本地的时间 datetime('now','localtime' )
    //INSERT into mytime values ('xm',datetime('now','localtime' ))
    query.prepare("insert into mytime ( name,time ) values ( ? , datetime('now','localtime' ) );");
    query.bindValue(0,name);
    bool ok = query.exec();
    if(ok)
    {
        qDebug()<<  "插入打卡记录表成功";
    }
    else
        qDebug()<<  "插入打卡记录表失败";
}

QStringList Mytime::selectTime(QDateTime start, QDateTime end)
{
    //创建执行sql的类对象
    QSqlQuery query;
    //准备语句
    query.prepare("select * from mytime where time between ? and ? ;");
    //绑定数据
    query.bindValue(0,QString("%1 00:00:00").arg(start.toString("yyyy-MM-dd")));
    query.bindValue(1,QString("%1 23:59:59").arg(end.toString("yyyy-MM-dd")));
    //定义存放数据的容器
    QStringList list;
    //执行语句
    bool re = query.exec();
    if(re)
    {
        qDebug()<<"查询时间表成功";
        //遍历结果集
        while (query.next()) {
            //获取名字
            QString name  = query.record().value(0).toString();
            //获取打卡时间
            QDateTime ti = query.record().value(1).toDateTime();
            //拼接字符串
            QString str = "姓名：" + name + " 打卡时间：" + ti.toString("yyyy-MM-dd hh:mm:ss");
            //插入数据
            list.append(str);
        }
    }
    else
    {
        qDebug()<<"查询时间表失败";
    }
    return list;
}

Mytime::~Mytime()
{

}

