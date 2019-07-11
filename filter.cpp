#include "filter.h"
#include "ui_filter.h"
using namespace cv;
filter::filter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::filter)
{
    ui->setupUi(this);

    btnEdge = new QButtonGroup(this);
    btnEdge->addButton(ui->robortsButton, 0);
    btnEdge->addButton(ui->robortsButton_2, 1);
    btnEdge->addButton(ui->prewittButton, 2);
    btnEdge->addButton(ui->prewittButton_2, 3);
    btnEdge->addButton(ui->sobelButton, 4);
    btnEdge->addButton(ui->sobelButton2, 5);

    btnNoise = new QButtonGroup(this);
    btnNoise->addButton(ui->gaussianButton, 0);
    btnNoise->addButton(ui->medianButton, 1);

    model = new QStandardItemModel();
    model->setColumnCount(5);
    model->setRowCount(5);
    ui->tableView->setModel(model);
    connect(ui->robortsButton, SIGNAL(clicked()), this, SLOT(onRadioClickEdge()));
    connect(ui->robortsButton_2, SIGNAL(clicked()), this, SLOT(onRadioClickEdge()));
    connect(ui->prewittButton, SIGNAL(clicked()), this, SLOT(onRadioClickEdge()));
    connect(ui->prewittButton_2, SIGNAL(clicked()), this, SLOT(onRadioClickEdge()));
    connect(ui->sobelButton, SIGNAL(clicked()), this, SLOT(onRadioClickEdge()));
    connect(ui->sobelButton2, SIGNAL(clicked()), this, SLOT(onRadioClickEdge()));

    connect(ui->gaussianButton, SIGNAL(clicked()), this, SLOT(onRadioClickNoise()));
    connect(ui->medianButton, SIGNAL(clicked()), this, SLOT(onRadioClickNoise()));

}

filter::~filter()
{
    delete ui;
}

int filter::openFile()
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

void filter::Roberts(InputArray src, OutputArray dst, float* a)
{
    Mat kernel = Mat(2,2,CV_32F,a);
    filter2D(src,dst,-1,kernel);
    showkernel(model,kernel);
}

void filter::Prewitt(InputArray src, OutputArray dst,float* a)
{
    Mat kernel = Mat(3,3,CV_32F,a);
    filter2D(src,dst,-1,kernel);
    showkernel(model,kernel);
}

void filter::Gaussian(InputArray src, OutputArray dst,int kernel_size, double sigma)
{
    Mat kernel_x = getGaussianKernel(kernel_size, sigma, CV_32F);
    Mat kernel_y = getGaussianKernel(kernel_size, sigma, CV_32F);
    Mat kernel = kernel_x * kernel_y.t();
    GaussianBlur(src, dst, Size(kernel_size,kernel_size),sigma);
    if(kernel_size<=5)
    {
        showkernel(model,kernel);
    }
}

void filter::Median(InputArray src, OutputArray dst,int kernel_size)
{
    medianBlur(src,dst,kernel_size);
}

void filter::showkernel(QStandardItemModel* table, cv::Mat array)
{
    model->clear();
    int row = array.rows;
    int col = array.cols;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            // table->setItem(i,j,new QStandardItem( QString("%0").arg(static_cast<double>(array.at<float>(i,j)))));
            table->setItem(i,j,new QStandardItem(QString::number(static_cast<double>(array.at<float>(i,j)),'f',2)));
        }
    }

}
void filter::onRadioClickEdge()
{
    Mat result;
    float a1[]={1,0,0,-1};
    float a2[]={0,1,-1,0};
    float b1[]={1,1,1,0,0,0,-1,-1,-1};
    float b2[]={-1,0,1,-1,0,1,-1,0,1};
    float s1[]={-1,0,1,-2,0,2,-1,0,1};
    float s2[]={-1,-2,-1,0,0,0,1,2,1};
    Mat k;
    switch (btnEdge->checkedId())
    {
    case 0:
        Roberts(gray,result,a1);
        break;
    case 1:
        Roberts(gray,result,a2);
        break;
    case 2:
        Prewitt(gray,result,b2);
        break;
    case 3:
        Prewitt(gray,result,b1);
        break;
    case 4:
        k = Mat(3,3,CV_32F,s1);
        showkernel(model,k);
        Sobel(gray,result,-1,1,0);
        break;
    case 5:
        k = Mat(3,3,CV_32F,s2);
        showkernel(model,k);
        Sobel(gray,result,-1,0,1);
        break;
    default:
        return;
    }
    AreaDisplayMat(ui->edgeArea,result, img_h, img_w);
}

void filter::onRadioClickNoise()
{
    Mat result;
    switch (btnNoise->checkedId())
    {
    case 0:
        Gaussian(gray,result,k,sig);
        break;
    case 1:
        Median(gray,result,k);
        break;
    }
    AreaDisplayMat(ui->filterArea,result, img_h, img_w);
}
void filter::on_select_img_clicked()
{
    if(openFile()==0)
    {
        AreaDisplayMat(ui->oriArea, ori, img_h, img_w);
        AreaDisplayMat(ui->grayArea, gray, img_h, img_w);
    }
}

void filter::on_spinBox_valueChanged(int arg1)
{
    k = arg1;
    Mat result;
    switch (btnNoise->checkedId())
    {
    case 0:
        Gaussian(gray,result,k,sig);
        break;
    case 1:
        Median(gray,result,k);
        break;
    }
    AreaDisplayMat(ui->filterArea,result, img_h, img_w);
}

void filter::on_doubleSpinBox_valueChanged(double arg1)
{
    sig = arg1;
    Mat result;
    if(btnNoise->checkedId()==0)
    {
        Gaussian(gray,result,k,sig);
        AreaDisplayMat(ui->filterArea,result, img_h, img_w);
    }
}
