#include "historydialog.h"
#include "ui_historydialog.h"

HistoryDialog::HistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoryDialog)
{
    ui->setupUi(this);
    //设置当天日期
    ui->dateEdit_start->setDateTime(QDateTime::currentDateTime());
    ui->dateEdit_end->setDateTime(QDateTime::currentDateTime());
    //获取日期
    QDateTime start = ui->dateEdit_start->dateTime();
    QDateTime end = ui->dateEdit_end->dateTime();
    //根据数据库查询
    QStringList l = ManagerFactory::getIns()->time->selectTime(start,end);
    //遍历容器
    for(QStringList::iterator it=l.begin();it!=l.end();it++)
    {
        //显示ui上
        ui->textEdit->append(*it);
    }
}

HistoryDialog::~HistoryDialog()
{
    delete ui;
}
