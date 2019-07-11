#include "utils.h"
void LabelDisplayMat(QLabel *label, cv::Mat &mat, int height, int width)
{
    cv::Mat Rgb;
    QImage Img;
    if (mat.channels() == 3)//RGB Img
    {
        cv::cvtColor(mat, Rgb, CV_BGR2RGB);//颜色空间转换
        Img = QImage((Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
    }
    else//Gray Img
    {
        Img = QImage((mat.data), mat.cols, mat.rows, mat.cols*mat.channels(), QImage::Format_Indexed8);
    }
    QPixmap pixmap = QPixmap::fromImage(Img);
    label->setPixmap(pixmap.scaled(width,height,Qt::KeepAspectRatio));
    //label->resize(QSize(Img.width(),Img.height()));
}

void AreaDisplayMat(QScrollArea *area, cv::Mat &mat, int height, int width)
{
    QLabel *label = new QLabel();
    LabelDisplayMat(label,mat,height,width);
    area->setWidget(label);
    //LabelDisplayMat(label,mat);
    //area->show();
}

void ChangeScale(QScrollArea *area, int factor)
{
    QWidget* widget = area->widget();
    QList<QLabel*> labelList  = area->takeWidget()->findChildren<QLabel*>();
    QLabel* label = labelList.at(0);

    //const QPixmap *Img = new QPixmap();
    //Img = label->pixmap();
    //QWidget * widget = new QWidget();
    //label = area->takeWidget()->findChild<QLabel*>();
    //label->findChild<QImage*>();
    label->pixmap();
    int h = label->pixmap()->height()+factor;
    int w = label->pixmap()->width()+factor;
    label->pixmap()->scaled(w,h,Qt::KeepAspectRatio);
    //label->set
    //LabelDisplayMat(label,mat);
    area->setWidget(label);
    //LabelDisplayMat(label,mat);
    //area->show();
}
