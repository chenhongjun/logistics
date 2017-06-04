#ifndef WIDGET_H
#define WIDGET_H
#include <QTableWidget>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include "table.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void one();
    void two();
    void three();
    double toD(QString text);
private:
    QHBoxLayout* nm;
    QHBoxLayout* dataLayout;
    QHBoxLayout* buttonsLayout;
    QVBoxLayout* mainLayout;

    QLabel* myitem;
    QLineEdit* n;
    QLabel* company;
    QLineEdit* m;

    //QTextEdit* endnum;
    QTableWidget* endnum;

    QPushButton* beginBo;
    QPushButton* resetBo;
    QPushButton* cleanBo;
private:
    Table** ppt;
    int now;
    int inputn;
    int inputm;
    double RI[9];
private:
    double* nn_ww;//1个*表数据
    double** n_mm_ww;//n个指标表数据

    double (*cir)[4];//指向1+n+1个(入max,CI,RI,CR)
    double* endw;//最终的w值,下标就是企业序号
public slots:
    void lastsolt();
};

#endif // WIDGET_H
