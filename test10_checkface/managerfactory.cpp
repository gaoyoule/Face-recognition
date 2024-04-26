#include "managerfactory.h"

ManagerFactory* ManagerFactory::ins = NULL;

ManagerFactory::ManagerFactory()
{
    //加载数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库主机名称
    db.setHostName("test");
    //设置数据库文件名称
    db.setDatabaseName("test.db");
    //打开数据库
    db.open();
    //初始化指针
    user = new UserDao;
    time = new Mytime;
}

ManagerFactory::ManagerFactory(const ManagerFactory &)
{

}

ManagerFactory *ManagerFactory::getIns()
{
    if(ins == NULL)
    {
        ins = new ManagerFactory;
    }
    return ins;
}

ManagerFactory::~ManagerFactory()
{
    //关闭数据库
    db.close();
    //释放空间
    delete user;
    delete time;
}

