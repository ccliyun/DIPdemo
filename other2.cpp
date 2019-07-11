#include "other2.h"
#include "ui_other2.h"

other2::other2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::other2)
{
    ui->setupUi(this);
}

other2::~other2()
{
    delete ui;
}

int other2::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                ".",
                                                tr("Image Files(*.png *.jpg *.tif *.bmp)"));
    if(path.isEmpty())
            return 1;
    std::string path_tmp = std::string(path.toLocal8Bit());
    gray = cv::imread(path_tmp, cv::IMREAD_GRAYSCALE);
    //cv::threshold(gray,gray,100,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
    img_h = gray.rows;
    img_w = gray.cols;
    return 0;
}

bool other2::matIsEqual(const cv::Mat mat1, const cv::Mat mat2) {
    if (mat1.empty() && mat2.empty()) {
        return true;
    }
    if (mat1.cols != mat2.cols || mat1.rows != mat2.rows || mat1.dims != mat2.dims||
        mat1.channels()!=mat2.channels()) {
        return false;
    }
    if (mat1.size() != mat2.size() || mat1.channels() != mat2.channels() || mat1.type() != mat2.type()) {
        return false;
    }
    unsigned long long nrOfElements1 = mat1.total()*mat1.elemSize();
    if (nrOfElements1 != mat2.total()*mat2.elemSize()) return false;
    bool lvRet = memcmp(mat1.data, mat2.data, nrOfElements1) == 0;
    return lvRet;
}

cv::Mat other2::reconstruction(const cv::Mat ImReference, const cv::Mat ImMarker)
{
    cv::Mat ImRec;
    cv::Mat ImResult;
    cv::Mat ImDilated;
    cv::Mat K = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
    bool done;
    //cv::morphologyEx(ImReference,ImMarker,cv::MORPH_OPEN,ele,cv::Point(-1,-1));
    //AreaDisplayMat(ui->markArea, ImMarker);
    ImMarker.copyTo(ImRec);
    do
    {
        ImRec.copyTo(ImResult);
        cv::dilate(ImResult,ImDilated,K);
        cv::min(ImDilated,ImReference,ImRec);
        done = matIsEqual(ImRec,ImResult);
    }while(!done);
    return ImResult;
}

void other2::on_selecttButton_clicked()
{
    if(openFile()==0)
    {
        AreaDisplayMat(ui->oriArea, gray, img_h, img_w);
    }
}

void other2::on_ginButton_clicked()
{
    cv::Mat T;
    cv::Mat M;
    cv::Mat V;
    cv::Mat ele = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11));
    cv::Mat K = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
    bool done;
    cv::threshold(gray,V,100,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
    cv::erode(V, M, ele);
    //cv::morphologyEx(V,M,cv::MORPH_ERODE,ele,cv::Point(-1,-1));
    AreaDisplayMat(ui->markArea, M, img_h, img_w);
    int i = 0;
    do
    {
        i++;
        M.copyTo(T);
        cv::dilate(M,M,K);
        cv::bitwise_and(M,V,M);
        done = matIsEqual(M,T);
    }while (!done);

    AreaDisplayMat(ui->grayArea, M, img_h, img_w);
}

void other2::on_grayButton_clicked()
{
    cv::Mat ImReference;
    cv::Mat ImMarker;
    cv::Mat ImResult;
    cv::Mat ele = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11));
    gray.copyTo(ImReference);
    cv::morphologyEx(ImReference,ImMarker,cv::MORPH_OPEN,ele,cv::Point(-1,-1));
    AreaDisplayMat(ui->markArea, ImMarker, img_h, img_w);
    ImResult = reconstruction(ImReference,ImMarker);
    AreaDisplayMat(ui->grayArea, ImResult, img_h, img_w);
}

void other2::on_cbrButton_clicked()
{
    cv::Mat ImReference;
    cv::Mat ImMarker;
    cv::Mat ImResult;
    cv::Mat ele = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11));
    gray.copyTo(ImReference);
    cv::morphologyEx(ImReference,ImMarker,cv::MORPH_CLOSE,ele,cv::Point(-1,-1));
    AreaDisplayMat(ui->markArea, ImMarker, img_h, img_w);
    ImResult = reconstruction(ImReference,ImMarker);
    AreaDisplayMat(ui->grayArea, ImResult, img_h, img_w);
}
