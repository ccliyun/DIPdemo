#include "threshold.h"
#include "ui_threshold.h"

using namespace cv;

threshold::threshold(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::threshold)
{
    ui->setupUi(this);
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->spinBox, SLOT(setValue(int)));
}

threshold::~threshold()
{
    delete ui;
}

int threshold::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                ".",
                                                tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));
    if(path.isEmpty())
            return 1;
    std::string path_tmp = std::string(path.toLocal8Bit());
    ori = cv::imread(path_tmp, cv::IMREAD_COLOR);
    cv::cvtColor(ori,gray,CV_BGR2GRAY);
    img_h = gray.rows;
    img_w = gray.cols;
    return 0;
}

void threshold::drawHist()
{
    Mat hist;
    Mat histtmp(Size(256, 300),  CV_8UC3, Scalar(255,255,255));
           // Mat::zeros(Size(256, 300), CV_8UC3);

    Mat Rgb;
    int histSize = 256;
    float range[] = { 0, 255 } ;
    const float* histRange = { range };
    calcHist(&gray,1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange);
    double g_dHistMaxValue;
    minMaxLoc(hist, nullptr, &g_dHistMaxValue, nullptr, nullptr);
    hist.convertTo(hist, CV_64F);
    for (int i = 0; i < 256; i++)
        {
            //std::cout<<hist.at<float>(i)<<std::endl;
            double tmp = hist.at<double>(i);
            int value = cvRound(tmp * 300 * 0.9 / g_dHistMaxValue);
            line(histtmp, Point(i, histtmp.rows - 1), Point(i, histtmp.rows - 1 - value), Scalar(255, 0, 0));
        }
    cv::cvtColor(histtmp, Rgb, CV_BGR2RGB);//颜色空间转换
    histmap = QPixmap::fromImage(QImage((Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888));
}

void threshold::dispHist(int threshold)
{
    thd = threshold;
    QPixmap px = histmap.copy();
    QPainter p(&px);
    p.setPen (Qt::red);
    p.drawLine (threshold, 0, threshold, px.height()-1);
    p.end ();
    ui->histLabel->setPixmap(px);
}

float threshold::caculateCurrentEntropy(cv::Mat hist, int threshold)
{
    float BackgroundSum = 0, targetSum = 0;
    //const float* pDataHist = hist.ptr<float>(0);
    for (int i = 0; i < 256; i++)
    {
        // background
        if( i < threshold )
        {
            BackgroundSum += hist.at<float>(i);
        }else // foreground
        {
            targetSum += hist.at<float>(i);
        }
    }
    //std::cout << BackgroundSum << " "<< targetSum << std::endl;
    float BackgroundEntropy = 0, targetEntropy = 0;
    for (int i = 0; i < 256; i++)
    {
        // cal background entropy
        if( i < threshold )
        {
            if( hist.at<float>(i) == 0 )
                continue;
            float ratio1 = hist.at<float>(i) / BackgroundSum;
            // cal energy entropy
            BackgroundEntropy += - ratio1 * logf(ratio1);
        }else // cal foreground entropy
        {
            if( hist.at<float>(i) == 0 )
                continue;
            float ratio2 =hist.at<float>(i) / targetSum;
            targetEntropy += -ratio2 * logf(ratio2);
        }
    }
    return ( targetEntropy + BackgroundEntropy );
}

int threshold::maxEntropySegMentation(cv::Mat inputImage, cv::OutputArray result)
{
    // init
    const int channels[1]={0};
    const int histSize[1]={256};
    float pranges[2]={0, 256};
    const float* ranges[1]={pranges};
    cv::MatND hist;
    // cal hist
    cv::calcHist(&inputImage,1,channels,
        cv::Mat(),hist,1,histSize,ranges);
    float maxentropy = 0;
    int    max_index  = 0;
    //cv::Mat result;
    // look through all hist
    for(int i = 0; i < 256; i++)
    {
        float cur_entropy =
            caculateCurrentEntropy(hist,i);
        // cal max
        if(cur_entropy > maxentropy)
        {
            maxentropy = cur_entropy;
            max_index = i;
        }
    }
    //  threshold
    cv::threshold(inputImage, result, max_index, 255,
           CV_THRESH_BINARY);
    return max_index;
}

void threshold::on_select_img_clicked()
{
    if(openFile()==0)
    {
        AreaDisplayMat(ui->oriArea, ori, img_h, img_w);
        AreaDisplayMat(ui->grayArea, gray, img_h, img_w);
        drawHist();
        dispHist(0);
    }
}

void threshold::on_pushButton_clicked()
{
    Mat result;
    int ret = int(cv::threshold(gray,result,0,255,THRESH_BINARY|THRESH_OTSU));
    dispHist(ret);
    AreaDisplayMat(ui->binaryArea,result, img_h, img_w);
    ui->spinBox->setValue(ret);
    ui->horizontalSlider->setValue(ret);

}

void threshold::on_spinBox_valueChanged(int arg1)
{
    Mat result;
    dispHist(arg1);
    cv::threshold(gray,result,thd,255,THRESH_BINARY);
    AreaDisplayMat(ui->binaryArea,result, img_h, img_w);
}

void threshold::on_entropy_clicked()
{
    Mat result;
    int ret = maxEntropySegMentation(gray, result);
    dispHist(ret);
    AreaDisplayMat(ui->binaryArea,result, img_h, img_w);
    ui->spinBox->setValue(ret);
    ui->horizontalSlider->setValue(ret);
}
