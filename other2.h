#ifndef OTHER2_H
#define OTHER2_H

#include <QWidget>
#include "utils.h"

namespace Ui {
class other2;
}

class other2 : public QWidget
{
    Q_OBJECT

public:
    explicit other2(QWidget *parent = nullptr);
    ~other2();
    int openFile();
    bool matIsEqual(const cv::Mat mat1, const cv::Mat mat2);

private slots:
    void on_selecttButton_clicked();

    void on_ginButton_clicked();

    void on_grayButton_clicked();

    void on_cbrButton_clicked();

    cv::Mat reconstruction(const cv::Mat mat1, const cv::Mat mat2);

private:
    Ui::other2 *ui;
    cv::Mat gray;
    int img_h=0;
    int img_w=0;
};

#endif // OTHER2_H
