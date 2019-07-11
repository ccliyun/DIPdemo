#ifndef FILTER_H
#define FILTER_H

#include <QWidget>
#include <utils.h>


namespace Ui {
class filter;
}

class filter : public QWidget
{
    Q_OBJECT

public:
    explicit filter(QWidget *parent = nullptr);
    int openFile();
    void Roberts(cv::InputArray src, cv::OutputArray dst,float* a);
    void Prewitt(cv::InputArray src, cv::OutputArray dst,float* a);
    void Gaussian(cv::InputArray src, cv::OutputArray dst,int kernel_size, double sigma);
    void Median(cv::InputArray src, cv::OutputArray dst,int kernel_size);
    void showkernel(QStandardItemModel* table, cv::Mat array);
    ~filter();

private slots:
    void on_select_img_clicked();
    void onRadioClickEdge();
    void onRadioClickNoise();

    void on_spinBox_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

private:
    QStandardItemModel* model;
    QButtonGroup *btnEdge;
    QButtonGroup *btnNoise;
    Ui::filter *ui;
    cv::Mat ori;
    cv::Mat gray;
    int k=1;
    double sig=0.1;
    int img_h=0;
    int img_w=0;
};

#endif // FILTER_H
