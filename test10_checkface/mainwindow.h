#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <QMessageBox>
#include <QDebug>
#include <QTimerEvent>
#include <QTimer>
#include <QDateTime>
#include <vector>
#include <QFile>
#include "managerfactory.h"
#include "historydialog.h"
#include <QAxObject>
using namespace cv::face;
using namespace cv;
using namespace std;
#define FACETOOL "E:/opencv/path/opencv3.4-install/install/etc/haarcascades/haarcascade_frontalface_alt2.xml"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initVideoCapture();//摄像头初始化
    void initCascadeClassifier();//级联分类器初始化
    void initFaceRecognizer();//人脸识别器指针初始化
    void timerEvent(QTimerEvent *);//重写定时器事件
    void blackSrc();//绘制有效区域
    void findFace();//用级联分类器进行人脸识别
    void checkFace();//用人脸识别器进行预测人脸
    void showImage();//人脸显示
    bool initSpeech();//语音播报初始化
    bool speak(QString txt);//文本语音朗读函数
    ~MainWindow();
private slots:
    void slot_study();//自定义定时器的槽函数

    void on_pushButton_insert_clicked();

    void on_pushButton_history_clicked();

    void dealevent(QString name, int, void *);//朗读结束

private:
    Ui::MainWindow *ui;
    //创建摄像头类对象
    VideoCapture vc;
    //创建级联分类器对象
    CascadeClassifier classifier;
    //创建人脸识别器指针
    Ptr<FaceRecognizer> recognizer;
    //定义读取摄像头的图像类对象
    Mat src;
    //定义有效矩形类对象
    Rect ret;
    //定义有效区域内截图
    Mat mid;
    //定义人脸矩形容器
    vector<Rect> faces;
    //定义自定义定时器
    QTimer studyTimer;
    //定义保存人脸图像的容器
    vector<Mat> studyFaces;
    //定义标志学习状态的变量
    bool isStudy;
    //语言播报的相关变量
    bool binit;
    bool bReading;
    //语言类
    QAxObject voice;
};

#endif // MAINWINDOW_H
