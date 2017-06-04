#include "table.h"
Table::Table(int w, int h, QWidget *parent)
    : mywidth_(w), myheight_(h), QDialog(parent)
{

    data = new QTableWidget;
    last = new QPushButton;
    next = new QPushButton;


    data->setColumnCount(mywidth_);
    data->setRowCount(myheight_);
    for (int i = 0; i < mywidth_; ++i)
        data->setColumnWidth(i,50);
    for (int i = 0; i < myheight_; ++i)
        data->setRowHeight(i,30);

    mainLayout = new QVBoxLayout;
    dataLayout = new QHBoxLayout;
    pushLayout = new QHBoxLayout;

    last->setText(tr("上一步"));
    next->setText(tr("下一步"));

    last->setMaximumWidth(100);
    next->setMaximumWidth(100);

    dataLayout->addWidget(data);
    pushLayout->addStretch();
    pushLayout->addWidget(last);
    pushLayout->addWidget(next);
    pushLayout->addStretch();
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(pushLayout);
    setLayout(mainLayout);
}

Table::~Table()
{

}
