#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtWidgets>
#include "confirmbox.h"
#include "zbar.h"  
#include <QCloseEvent>		
#pragma once
#pragma execution_character_set("utf-8")


using namespace std;
using namespace cv;
using namespace zbar;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QFile styleSheet("./Resources/main.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
        return;
    }
    qApp->setStyleSheet(styleSheet.readAll());
    styleSheet.close();


    ui->setupUi(this);
    connect(ui->camera, SIGNAL(clicked()), this, SLOT(DoCamera()));
    connect(ui->playpause, SIGNAL(clicked()), this, SLOT(PlayPause()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(ClearTable()));
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setStretchLastSection(false);
}


MainWindow::~MainWindow()
{

	delete ui;
	
}

//关闭程序
void MainWindow::closeEvent(QCloseEvent *event)

{
	cap.release();
	cv::destroyAllWindows();
	exit(0);
}

//将Mat格式转化为QImage格式，并输出在label控件上
void MainWindow::ImgDisplay(cv::Mat frame)
{

	if (!frame.empty())
	{

		img = QImage((const unsigned char*)(frame.data), frame.cols, frame.rows,
			QImage::Format_RGB888).rgbSwapped();
		img = img.scaled(
			ui->label->width(),
			ui->label->height(),
			Qt::IgnoreAspectRatio,
			Qt::FastTransformation
			);//调整图片大小，使图片和显示的label控件一样大
		ui->label->setPixmap(QPixmap::fromImage(img));
	}
}

//将时间和读取的二维码信息输出到TreeWidget控件上
void MainWindow::CodeShow(String code)
{
	QString Qcode = QString::fromStdString(code);


	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("hh:mm:ss");

	QStringList tt;
	tt << current_date << Qcode;
	QTreeWidgetItem *child = new QTreeWidgetItem(ui->treeWidget, tt);

}

//检测及识别二维码
void MainWindow::CodeRead(void)
{
	cap >> frame;//读取帧信息
	
	//扫描器初始化
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	Mat imageGray;
	cvtColor(frame, imageGray, CV_RGB2GRAY);//转化为灰度图
	//对图像进行平滑处理,模糊，去毛刺
	blur(imageGray, imageGray, Size(3, 3));
	//使灰度图象直方图均衡化
	equalizeHist(imageGray, imageGray);
	//指定112阀值进行二值化
	threshold(imageGray, imageGray, 180, 255, THRESH_BINARY);

	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;
	Image image(width, height, "Y800", raw, width * height);
	int n = scanner.scan(image);

	if (image.symbol_begin() == image.symbol_end())//若图中无二维码
	{
		ImgDisplay(frame);
		waitKey(5);
		return;
	}
	for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); symbol++)
	{

		//二维码检测
		vector<Point> vp;		
		int n = symbol->get_location_size();
		for (int i = 0; i<n; i++)
		{
			vp.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
		}
		RotatedRect r = minAreaRect(vp);
		Point2f pts[4];
		r.points(pts);
		Point textPoint(pts[1]);
		string codeNum = symbol->get_data();
		for (int i = 0; i<4; i++)
		{
			line(frame, pts[i], pts[(i + 1) % 4], Scalar(255, 0, 0), 3);

		}

		//二维码识别
		code = symbol->get_data();
		if (code != codecmp || result)
		{
			result = 0;
			CodeShow(code);
		}

		codecmp = code;
	}
	ImgDisplay(frame);
	image.set_data(NULL, 0);


	waitKey(5);

}

//摄像头开启及关闭
void MainWindow::DoCamera()
{
	if (ui->camera->text() == "打开摄像头")
	{

		cap.open(0);
		cvNamedWindow("window",0);
		ui->camera->setText("关闭摄像头");


		while (1)
		{

			
			CodeRead();

		}
		
	}
	else
	{

		ui->camera->setText("打开摄像头");
		//cv::destroyAllWindows();
		cap.release();
		ui->label->setText("请先打开摄像头！");
	}

}

//视频播放和暂停
void MainWindow::PlayPause()
{
	if (ui->playpause->text() == "暂停")
	{

		ui->playpause->setText("播放");
		waitKey(-1);
	}
	else
	{

		ui->playpause->setText("暂停");
		while (1)
		{

			CodeRead();
		}
	}
}


 int MainWindow::ShowConfirmBox()
{
    ConfirmBox *msg = new ConfirmBox;
    return msg->exec();
}

 //清空列表
void MainWindow::ClearTable()
{

	result=ShowConfirmBox();
    if(result==1)
    ui->treeWidget->clear();
}
