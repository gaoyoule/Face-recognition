#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),isStudy(false)
{
    ui->setupUi(this);
    //初始化
    initVideoCapture();
    initCascadeClassifier();
    initFaceRecognizer();
    binit = false;
    bReading = false;
    if(initSpeech())
    {
        qDebug()<<"speek ok";
    }
    else
    {
        qDebug()<<"speek error";
    }
    //开启定时器事件(每50毫秒读取一帧图像)
    startTimer(50);
    //当自定义定时器超时时会发送timeout信号
    connect(&studyTimer,SIGNAL(timeout()),this,SLOT(slot_study()));
}

void MainWindow::initVideoCapture()
{
    //打开摄像头
    if(vc.open(0))
    {
        qDebug()<<"call initVideoCapture open ok";
    }
    else
    {
        qDebug()<<"call initVideoCapture open error";
        return;
    }
}

void MainWindow::initCascadeClassifier()
{
    //通过配置文件haarcascade_frontalface_alt2.xml创建级联分类器对象
    classifier = CascadeClassifier(FACETOOL);
    //判断是否创建成功
    if(classifier.empty())
    {
        qDebug()<<"call initCascadeClassifier error";
        return;
    }
    else
    {
        qDebug()<<"call initCascadeClassifier ok";
    }
}

void MainWindow::initFaceRecognizer()
{
    //创建文件类对象
    QFile file("face.xml");
    //判断模型文件是否存在
    if(file.exists())
    {
        //加载模型文件
        recognizer = FaceRecognizer::load<LBPHFaceRecognizer>("face.xml");
        qDebug()<<"LBPHFaceRecognizer load ok";
    }
    else
    {
        //创建空的人脸识别器
        recognizer = LBPHFaceRecognizer::create();
        qDebug()<<"LBPHFaceRecognizer create ok";
    }
}

void MainWindow::timerEvent(QTimerEvent *)
{
    //读取摄像头图像
    vc>>src;
    //判断是否读取成功
    if(src.empty())
        return;
    //翻转
    flip(src,src,1);
    //调整大小
    cv::resize(src,src,Size(600,600));
    //绘制有效区域
    blackSrc();
    //人脸识别
    findFace();
    //预测人脸
    checkFace();
    //人脸显示
    showImage();
}

void MainWindow::blackSrc()
{
    //获取矩形类对象
    ret = Rect(src.cols/2-150,src.rows/2-150,300,300);
    //画白色矩形
    rectangle(src,ret,Scalar(255,255,255),2);
    //遍历行
    for(int y=0;y<src.rows;y++)
    {
        //遍历列
        for(int x=0;x<src.cols;x++)
        {
            //判断是否是有效区域内
            if(!ret.contains(Point(x,y)))
            {
                //蓝色
                src.at<Vec3b>(y,x)[0] =
                        saturate_cast<uchar>(src.at<Vec3b>(y,x)[0]-50);
                //绿色
                src.at<Vec3b>(y,x)[1] =
                        saturate_cast<uchar>(src.at<Vec3b>(y,x)[1]-50);
                //红色
                src.at<Vec3b>(y,x)[2] =
                        saturate_cast<uchar>(src.at<Vec3b>(y,x)[2]-50);
            }
        }
    }
}

void MainWindow::findFace()
{
    //截图
    mid = src(ret);
    //接收灰度图像的类对象
    Mat gray;
    //色彩空间转换
    cvtColor(mid,gray,CV_BGR2GRAY);
    //检测人脸
    classifier.detectMultiScale(gray,faces);
    //判断是否有人脸
    if(faces.size()>0)
    {
        //给人脸标上红框
        rectangle(mid,faces[0],Scalar(0,0,255),2);
    }
}

void MainWindow::checkFace()
{
    //判断是否有人脸    是否是学习状态  人脸识别器为空
    if(faces.size()<=0||isStudy||recognizer->empty())
    {
        return;
    }
    //截取人脸
    Mat fa = src(faces[0]);
    //色彩空间转换
    cvtColor(fa,fa,CV_BGR2GRAY);
    //统一尺寸
    cv::resize(fa,fa,Size(100,100));
    //准备数据
    int labels = -1;
    double confidence = 0;
    //预测人脸
    recognizer->predict(fa,labels,confidence);
    qDebug()<<labels<<" "<<confidence;
    //判断人脸是谁的
    if(labels != -1 && confidence <= 150)
    {
        //获取名字
        QString name = ManagerFactory::getIns()->user->selectName(labels);
        //拼接
        QString str = name + "已签到";
        //语音播报
        speak(str);
        //消息提示框
        QMessageBox::warning(this,str,
                    QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        //把打卡记录插入到记录表里
        ManagerFactory::getIns()->time->insertTime(name);
    }
}

void MainWindow::showImage()
{
    //接收rgb的图像类对象
    Mat rgb;
    //色彩空间转换
    cvtColor(src,rgb,CV_BGR2RGB);
    //Mat->QImage
    QImage img(rgb.data,rgb.cols,rgb.rows,
               rgb.cols*rgb.channels(),QImage::Format_RGB888);
    //显示在ui上
    ui->label_video->setPixmap(QPixmap::fromImage(img));
}

bool MainWindow::initSpeech()
{
    if(binit)
        return true;
    //设置此属性会初始化COM对象。先前设置的任何COM对象都将关闭。
    //设置此属性的最有效方法是使用已注册组件的UUID
    binit = this->voice.setControl("96749377-3391-11D2-9EE3-00C04F797396");//设置COM的名称用来初始化COM的模型,返回是否加载成功
    if(binit)
    {
        connect(&this->voice,SIGNAL(signal(QString, int, void*)), this, SLOT(dealevent(QString, int, void*)));
    }
    return binit;
}

bool MainWindow::speak(QString txt)
{
    if(!binit)
        return false;
    qDebug()<<"开始朗读";
    //dynamicCall 功能：调用  这是调用COM对象的方法函数
    int result = this->voice.dynamicCall("Speak(QString, SpeechVoiceSpeakFlags)", txt ,1).toInt();//执行第一个参数的方法名，以第二个参数返回 是否执行成功
    bReading = true;
    return result;//返回是否执行阅读函数
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_study()
{
    //判断是否有人脸       人脸识别器是否为空
    if(faces.size()<=0||recognizer->empty())
        return;
    if(studyFaces.size()==10)
    {
        //停止机器学习
        studyTimer.stop();
        //获取名字
        QString name = ui->lineEdit->text();
        //插入信息
        int id = ManagerFactory::getIns()->user->insertUser(name);
        //标签数组
        vector<int> labels;
        //往容器中插入标签 插入10个一样的标签
        labels.insert(labels.begin(),10,id);
        //学习并更新模型文件
        recognizer->update(studyFaces,labels);
        recognizer->save("face.xml");
        //消息提示框
        QMessageBox::information(this,"通知","恭喜你，录入成功！");
        //清空
        ui->lineEdit->clear();
        studyFaces.clear();
        isStudy = false;
    }
    else
    {
        //截取人脸矩形
        Mat fa = mid(faces[0]);
        //色彩空间转换
        cvtColor(fa,fa,CV_BGR2GRAY);
        //统一尺寸
        cv::resize(fa,fa,Size(100,100));
        //存放到容器
        studyFaces.push_back(fa);
        qDebug()<<studyFaces.size();
    }
}

void MainWindow::on_pushButton_insert_clicked()
{
    //开启自定义定时器
    studyTimer.start(1000);
    isStudy = true;
}

void MainWindow::on_pushButton_history_clicked()
{
    //页面切换
    HistoryDialog *h = new HistoryDialog(this);
    h->show();
}

void MainWindow::dealevent(QString name, int, void *)
{
    if(name == "EndStream(int,QVariant)")
    {
        bReading = false;
        qDebug()<<"朗读完毕";
    }
}
