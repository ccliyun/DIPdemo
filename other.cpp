#include "other.h"
#include "ui_other.h"
#include <QTimer>

other::other(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::other)
{
    ui->setupUi(this);
    element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    btnType = new QButtonGroup(this);
    btnType->addButton(ui->standardButton, 0);
    btnType->addButton(ui->externalButton, 1);
    btnType->addButton(ui->internalButton, 2);
    //btnType->setId(ui->standardButton,0);
    ui->standardButton->setChecked(true);

    distanceType = new QButtonGroup(this);
    distanceType->addButton(ui->chessButton, 0);
    distanceType->addButton(ui->crossButton, 1);
    distanceType->addButton(ui->euclideanButton, 2);
    //btnType->setId(ui->standardButton,0);
    ui->chessButton->setChecked(true);

    connect(ui->chessButton, SIGNAL(clicked()), this, SLOT(onRadioClickDistance()));
    connect(ui->crossButton, SIGNAL(clicked()), this, SLOT(onRadioClickDistance()));
    connect(ui->euclideanButton, SIGNAL(clicked()), this, SLOT(onRadioClickDistance()));
}

other::~other()
{
    delete ui;
}

int other::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                ".",
                                                tr("Image Files(*.png *.jpg *.tif *.bmp)"));
    if(path.isEmpty())
            return 1;
    std::string path_tmp = std::string(path.toLocal8Bit());
    gray = cv::imread(path_tmp, cv::IMREAD_GRAYSCALE);
    img_h = gray.rows;
    img_w = gray.cols;
    //cv::threshold(gray,gray,100,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
    return 0;
}

void other::on_selecttButton_clicked()
{
    if(openFile()==0)
    {
        AreaDisplayMat(ui->oriArea, gray, img_h, img_w);
    }
}

void other::on_distanceButton_clicked()
{
    //cv::Mat result;
    //cv::distanceTransform(gray,result,cv::DIST_L2,CV_DIST_MASK_3);
    //cv::normalize(result,result,0,255,cv::NORM_MINMAX,CV_8UC1);
    cv::Mat img;
    cv::Mat skel(gray.size(), CV_8UC1, cv::Scalar(0));
    cv::Mat temp;
    cv::Mat eroded;

    //cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
    cv::threshold(gray,img,100,1,cv::THRESH_BINARY|cv::THRESH_OTSU);

    bool done;
    do
    {
      cv::erode(img, eroded, element);
      //cv::dilate(eroded, temp, element); // temp = open(img)
      //cv::morphologyEx(img,temp,cv::MORPH_OPEN,element);
      //cv::subtract(img, temp, temp);
      cv::add(skel, eroded, skel);

      eroded.copyTo(img);

      done = (cv::countNonZero(img) == 0);
    } while (!done);
    skel.copyTo(distance);
    cv::normalize(skel,skel,0,255,cv::NORM_MINMAX,CV_8UC1);
    AreaDisplayMat(ui->grayArea, skel, img_h, img_w);
}

void other::on_skeletButton_clicked()
{
    cv::Mat img;
    cv::Mat skel(gray.size(), CV_8UC1, cv::Scalar(0));
    cv::Mat temp;
    cv::Mat eroded;

    //cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
    cv::threshold(gray,img,100,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
    AreaDisplayMat(ui->oriArea, img, img_h, img_w);
    // gray.copyTo(img);
    bool done;
    do
    {
      cv::erode(img, eroded, element);
      cv::dilate(eroded, temp, element); // temp = open(img)
      //cv::morphologyEx(img,temp,cv::MORPH_OPEN,element);
      cv::subtract(img, temp, temp);
      cv::bitwise_or(skel, temp, skel);
      eroded.copyTo(img);

      done = (cv::countNonZero(img) == 0);
    } while (!done);
    skel.copyTo(skeleton);
    AreaDisplayMat(ui->grayArea, skel, img_h, img_w);
}


void other::on_restButton_clicked()
{
    cv::Mat img;
    cv::Mat res(gray.size(), CV_8UC1, cv::Scalar(0));
    cv::Mat temp;
    cv::Mat dilated;
    //cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
    double minVal;
    double maxVal;
    cv::Point minLoc;
    cv::Point maxLoc;
    //distance.copyTo(img);
    distance.copyTo(img, skeleton);
    minMaxLoc(img, &minVal, &maxVal, &minLoc, &maxLoc);
    for(int i =int(maxVal);i>0;i--)
    {
        temp = img==i;
        cv::bitwise_or(res,temp,res);
        cv::dilate(res,res,element);
    }
    AreaDisplayMat(ui->grayArea, res, img_h, img_w);
}

void other::on_edgeButton_clicked()
{
    cv::Mat eroded;
    cv::Mat dilated;
    cv::erode(gray, eroded, element);
    cv::dilate(gray,dilated,element);
    switch(btnType->checkedId())
    {
    case 0:
        cv::subtract(dilated, eroded, dilated);
        break;
    case 1:
        cv::subtract(dilated, gray, dilated);
        break;
    case 2:
        cv::subtract(gray, eroded, dilated);
        break;
    default:
        break;
    }

    AreaDisplayMat(ui->grayArea, dilated, img_h, img_w);
}

void other::on_gradientButton_clicked()
{
    cv::Mat eroded;
    cv::Mat dilated;
    cv::erode(gray, eroded, element);
    cv::dilate(gray,dilated,element);
    switch(btnType->checkedId())
    {
    case 0:
        cv::subtract(dilated, eroded, dilated);
        break;
    case 1:
        cv::subtract(dilated, gray, dilated);
        break;
    case 2:
        cv::subtract(gray, eroded, dilated);
        break;
    default:
        break;
    }
    dilated = dilated/2;
    AreaDisplayMat(ui->grayArea, dilated, img_h, img_w);
}

void other::onRadioClickDistance()
{
    switch(distanceType->checkedId())
    {
    case 0:
        element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        break;
    case 1:
        element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5));
        break;
    case 2:
        element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        break;
    default:
        break;
    }
}

void other::wheelEvent(QWheelEvent *e)
{
    int numDegress = e->delta();
    if ( QApplication::keyboardModifiers () == Qt::ControlModifier)
     {
     //pressed
        if(numDegress>0)
        {
            if(img_h<1500&&img_w<1500)
            {
                img_h+=5;
                img_w+=5;

            }
        }
        else {
            if(img_h<700&&img_w<700)
            {
                img_h+=5;
                img_w+=5;

            }
        }
    }

}

