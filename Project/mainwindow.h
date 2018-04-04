#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include "core/core.hpp"      
#include "highgui/highgui.hpp"      
#include "imgproc/imgproc.hpp"      
#include "iostream"    
#include <QDesktopServices>
#include <Windows.h>
#include <tchar.h>  
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

using namespace std;
using namespace cv;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
	cv::VideoCapture cap;
	cv::Mat frame;
	String code;
	String codecmp;
	QImage img;
	int result;


protected:					//Ìí¼Ó

	void closeEvent(QCloseEvent *event);
    static int ShowConfirmBox();
	void ImgDisplay(cv::Mat frame);
	void CodeShow(String code);
	void CodeRead(void);

private slots:

     void DoCamera();
     void PlayPause();
     void ClearTable();

};

#endif // MAINWINDOW_H
