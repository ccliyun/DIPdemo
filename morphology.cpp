#include "morphology.h"
#include "ui_morphology.h"

morphology::morphology(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::morphology)
{
    ui->setupUi(this);
    model = new QStandardItemModel();
    model->setColumnCount(3);
    model->setRowCount(3);
    for(int i=0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
        {
            model->setItem(i, j, new QStandardItem("1"));
        }
    }
    ui->tableView->setModel(model);
    //spinBoxDelegate = new NumDelegate();
    //ui->tableView->setItemDelegate(spinBoxDelegate);
    ui->tableView->setItemDelegate(new NumDelegate(this));
    getfromTable(model,kernel);
}

morphology::~morphology()
{
    delete ui;
}

int morphology::openFile()
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
    return 0;
}

void morphology::getfromTable(QStandardItemModel* table, cv::Mat& k)
{
    int col = table->columnCount();
    int row = table->rowCount();
    unsigned int **arr=new unsigned int*[col];
    for (int i = 0;i<row;i++) {
        arr[i]= new unsigned int[col];
    }
    for(int i=0;i<row;i++)
        for(int j=0;j<col;j++)
        {
            QString tmp = table->item(i,j)->text();
            arr[i][j]=tmp.toUInt();
        }
    k = cv::Mat(row,col,CV_8U,arr);
}
void morphology::on_selectButton_clicked()
{
    if(openFile()==0)
    {
        AreaDisplayMat(ui->oriArea, gray, img_h, img_w);
    }
}

void morphology::on_kernelBox_valueChanged(int arg1)
{
    model->setColumnCount(arg1);
    model->setRowCount(arg1);
    for(int i=0;i<arg1;i++)
    {
        for(int j = 0;j<arg1;j++)
        {
            model->setItem(i, j, new QStandardItem("1"));
        }
    }
    //getfromTable(model,kernel,arg1);
}

void morphology::on_dilateButton_clicked()
{
    cv::Mat result;
    getfromTable(model,kernel);
    cv::dilate(gray,result,kernel,cv::Point(-1,-1));
    AreaDisplayMat(ui->erosionArea, result, img_h, img_w);
}

void morphology::on_erosionButton_clicked()
{
    cv::Mat result;
    getfromTable(model,kernel);
    cv::erode(gray,result,kernel,cv::Point(-1,-1));
    AreaDisplayMat(ui->erosionArea, result, img_h, img_w);
}

void morphology::on_openButton_clicked()
{
    cv::Mat result;
    getfromTable(model,kernel);
    cv::morphologyEx(gray,result,cv::MORPH_OPEN,kernel,cv::Point(-1,-1));
    //cv::imshow("open",result);
    AreaDisplayMat(ui->openArea, result, img_h, img_w);
}

void morphology::on_colseButton_clicked()
{
    cv::Mat result;
    getfromTable(model,kernel);
    cv::morphologyEx(gray,result,cv::MORPH_CLOSE,kernel,cv::Point(-1,-1));
    AreaDisplayMat(ui->openArea, result, img_h, img_w);
}
