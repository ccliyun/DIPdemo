#ifndef THRESHOLD_H
#define THRESHOLD_H

#include <QWidget>
#include <QFileDialog>
#include <QScrollArea>
#include <QPainter>
#include "string.h"
#include "utils.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class threshold;
}

class threshold : public QWidget
{
    Q_OBJECT

public:
    explicit threshold(QWidget *parent = nullptr);
    ~threshold();
    int openFile();
    void drawHist();
    float caculateCurrentEntropy(cv::Mat hist, int threshold);
    int maxEntropySegMentation(cv::Mat inputImage, cv::OutputArray result);

private slots:
    void on_select_img_clicked();
    void dispHist(int threshold);
    void on_pushButton_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_entropy_clicked();

private:
    Ui::threshold *ui;
    cv::Mat ori;
    cv::Mat gray;
    QPixmap histmap;
    int thd;
    int img_h=0;
    int img_w=0;
};

#endif // THRESHOLD_H
