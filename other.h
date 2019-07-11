#ifndef OTHER_H
#define OTHER_H

#include <QWidget>
#include "utils.h"
#include <opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>


namespace Ui {
class other;
}

class other : public QWidget
{
    Q_OBJECT

public:
    explicit other(QWidget *parent = nullptr);
    int openFile();
    ~other()Q_DECL_OVERRIDE;

private slots:
    void on_selecttButton_clicked();

    void on_distanceButton_clicked();

    void on_skeletButton_clicked();

    void on_restButton_clicked();

    void on_edgeButton_clicked();

    void on_gradientButton_clicked();
    void onRadioClickDistance();

    void wheelEvent(QWheelEvent *e)Q_DECL_OVERRIDE;

private:
    Ui::other *ui;
    QButtonGroup *btnType;
    QButtonGroup *distanceType;
    cv::Mat gray;
    cv::Mat distance;
    cv::Mat skeleton;
    cv::Mat element;
    int img_h=0;
    int img_w=0;
};

#endif // OTHER_H
