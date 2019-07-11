#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

#include <QWidget>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QStandardItemModel>
#include <utils.h>

namespace Ui {
class morphology;
}

class morphology : public QWidget
{
    Q_OBJECT

public:
    explicit morphology(QWidget *parent = nullptr);
    ~morphology();
    int openFile();
    void getfromTable(QStandardItemModel* table,  cv::Mat& k);

private slots:
    void on_selectButton_clicked();

    void on_kernelBox_valueChanged(int arg1);

    void on_dilateButton_clicked();

    void on_erosionButton_clicked();

    void on_openButton_clicked();

    void on_colseButton_clicked();

private:
    Ui::morphology *ui;
    cv::Mat gray;
    QStandardItemModel* model;
    cv::Mat kernel;
    int img_h=0;
    int img_w=0;
};
class  NumDelegate :  public  QItemDelegate
{
    Q_OBJECT
public :
    NumDelegate(QObject *parent = 0): QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent,  const QStyleOptionViewItem &option,
         const  QModelIndex &index)  const
    {
        QSpinBox *editor =  new  QSpinBox(parent);
        editor->setMinimum(-10);
        editor->setMaximum(20);
        editor->setValue(0);
         return  editor;
    }
     void  setEditorData(QWidget *editor,  const  QModelIndex &index)  const
    {
         int  value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox =  static_cast <QSpinBox*>(editor);
        spinBox->setValue(value);
    }
     void  setModelData(QWidget *editor, QAbstractItemModel *model,
         const  QModelIndex &index)  const
    {
        QSpinBox *spinBox =  static_cast <QSpinBox*>(editor);
        spinBox->interpretText();
         int  value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
     void  updateEditorGeometry(QWidget *editor,
         const  QStyleOptionViewItem &option,  const  QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }
};

#endif // MORPHOLOGY_H
