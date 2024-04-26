#ifndef MYTIME_H
#define MYTIME_H

#include <QSqlQuery>
#include <QDateTime>
#include <QSqlRecord>
#include <QStringList>
#include <QDebug>

class Mytime
{
public:
    Mytime();//创建表 字段：姓名 name varchar 打卡时间  time datetime(数据类型 时间类型)
    void insertTime(QString name);//插入打卡记录
    QStringList selectTime(QDateTime start,QDateTime end);//查询记录
    ~Mytime();
};


#endif // MYTIME_H
