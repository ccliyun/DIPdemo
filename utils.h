#ifndef UTILS_H
#define UTILS_H

#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QButtonGroup>
#include <QStandardItemModel>
#include <QScrollArea>
#include <QItemDelegate>
#include <QSpinBox>
#include <QTableView>
#include <QWheelEvent>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void LabelDisplayMat(QLabel *label, cv::Mat &mat, int height, int width);
void AreaDisplayMat(QScrollArea *area, cv::Mat &mat, int height, int width);
void ChangeScale(QScrollArea *area, int factor);

#endif // UTILS_H
