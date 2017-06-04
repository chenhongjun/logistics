#include "widget.h"
#include <stdio.h>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    RI[0] = 0,RI[1] = 0,RI[2] = 0.58,RI[3] = 0.90,RI[4] = 1.12,RI[5] = 1.24,RI[6] = 1.32,RI[7] = 1.41,RI[8] = 1.45;
    nm = new QHBoxLayout;
    dataLayout = new QHBoxLayout;
    buttonsLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;

    n = new QLineEdit;
    myitem = new QLabel;
    m = new QLineEdit;
    company = new QLabel;

    endnum = new QTableWidget;


    beginBo = new QPushButton;
    resetBo = new QPushButton;
    cleanBo = new QPushButton;

    myitem->setText(tr("指标数:"));
    company->setText(tr("企业数:"));

    n->setFixedSize(50,20);
    n->setAlignment(Qt::AlignRight);
    n->setValidator(new QIntValidator(1, 50, this));
    n->setText("1");

    m->setFixedSize(50,20);
    m->setAlignment(Qt::AlignRight);
    m->setValidator(new QIntValidator(1, 50, this));
    m->setText("1");

    beginBo->setText(tr("输入数据"));
    resetBo->setText(tr("修改数据"));
    cleanBo->setText(tr("清空所有"));

    nm->addWidget(myitem);
    nm->addWidget(n);
    nm->addWidget(company);
    nm->addWidget(m);
    nm->addStretch();

    dataLayout->addWidget(endnum);

    buttonsLayout->addWidget(beginBo);
    buttonsLayout->addWidget(resetBo);
    buttonsLayout->addWidget(cleanBo);

    mainLayout->addLayout(nm);
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    connect(beginBo, SIGNAL(clicked()), this, SLOT(one()));
    connect(resetBo, SIGNAL(clicked()), this, SLOT(two()));
    connect(cleanBo, SIGNAL(clicked()), this, SLOT(three()));

    ppt = nullptr;
    nn_ww = nullptr;//1个*表数据
    n_mm_ww = nullptr;//n个指标表数据
    cir = nullptr;//指向1+n+1个(入max,CI,RI,CR)
    endw = nullptr;//最终的w值,下标就是企业序号
    inputn = 0;
    inputm = 0;
}

void Widget::one()//输入数据
{
    if(!(inputn == n->text().toInt() && inputm == m->text().toInt()))
    {
        int lastn = inputn;
        int lastm = inputm;
        inputn = n->text().toInt();
        inputm = m->text().toInt();
        int ntable = inputn+1;
        now = 0;

        if (ppt)
        {
            for (int i = 0; i < lastn+1; ++i)
                delete ppt[i];
            delete[] ppt;
            ppt = nullptr;
        }

        ppt = new Table* [ntable];
        ppt[0] = new Table(inputn, inputn);
        ppt[0]->setMinimumSize(inputn*50+50, inputn*30+30);
        ppt[0]->setMaximumSize(inputn*50+100, inputn*30+100);
        connect(ppt[0]->next, SIGNAL(clicked()), ppt[0], SLOT(close()));

        for (int i = 1; i < ntable; ++i)
        {
            ppt[i] = new Table(inputm, inputm);
            ppt[i]->setMinimumSize(inputm*50+50, inputm*30+30);
            ppt[i]->setMaximumSize(inputm*50+100, inputm*30+100);
            connect(ppt[i]->last, SIGNAL(clicked()), this, SLOT(lastsolt()));
            connect(ppt[i]->next, SIGNAL(clicked()), ppt[i], SLOT(close()));
        }


        if (nn_ww)
        {
            delete[] nn_ww;
        }
        if (n_mm_ww)
        {
            for (int i = 0; i < lastn; ++i)
                delete[] n_mm_ww[i];
            delete[] n_mm_ww;
        }
        if (cir)
        {
            delete[] cir;
        }
        if (endw)
        {
            delete[] endw;
        }
        nn_ww = new double[inputn*(inputn+2)];
        n_mm_ww = new double* [inputn];
        for (int i = 0; i < inputn; ++i)
        {
            n_mm_ww[i] = new double[inputm*(inputm+2)];
        }
        cir = new double[1+inputn+1][4];
        endw = new double[inputm];
    }


    for (now = 0; now < inputn+1; ++now)
    {
        ppt[now]->exec();
    }
    {
        for (int i = 0; i < inputn; ++i)
            for (int j = 0; j < inputn; ++j)
            {

                nn_ww[i*(inputn+2)+j] = toD(ppt[0]->data->item(i,j) ? ppt[0]->data->item(i,j)->text() : "");
                //qDebug() << nn_ww[i*(inputn+2)+j];
            }
        double *buf = new double[inputn];
        for (int i = 0; i < inputn; ++i)
        {
            for (int j = 0; j < inputn; ++j)
            {
                if (i == 0)
                    buf[j] = 0;
                buf[j] += nn_ww[i*(inputn+2)+j];
            }
        }
        for (int i = 0; i < inputn; ++i)
            nn_ww[i*(inputn+2) + (inputn)] = buf[i];

        for (int i = 0; i < inputn; ++i)
        {
            double sh = 0;
            for (int j = 0; j < inputn; ++j)
            {
                sh += nn_ww[i*(inputn+2)+j]/nn_ww[j*(inputn+2) + (inputn)];
            }
            nn_ww[i*(inputn+2)+ inputn+1] = sh/inputn;
            //qDebug() << sh/inputn;
        }
        delete[] buf;

        //获取值计算
        for (now = 1; now < inputn+1; ++now)
        {
            for (int i = 0; i < inputm; ++i)
                for (int j = 0; j < inputm; ++j)
                {
                    n_mm_ww[now-1][i*(inputm+2)+j] = toD(ppt[now]->data->item(i,j) ? ppt[now]->data->item(i,j)->text() : "");
                    //qDebug() << n_mm_ww[now-1][i*(inputm+2)+j];
                }
            double *buf = new double[inputm];

            for (int i = 0; i < inputm; ++i)
            {
                for (int j = 0; j < inputm; ++j)
                {
                    if (i == 0)
                        buf[j] = 0;
                    buf[j] += n_mm_ww[now-1][i*(inputm+2)+j];
                }
            }
            for (int i = 0; i < inputm; ++i)
                n_mm_ww[now-1][i*(inputm+2) + (inputm)] = buf[i];

            for (int i = 0; i < inputm; ++i)
            {
                double sh = 0;
                for (int j = 0; j < inputm; ++j)
                {
                    sh += n_mm_ww[now-1][i*(inputm+2)+j]/n_mm_ww[now-1][j*(inputm+2) + (inputm)];
                }
                n_mm_ww[now-1][i*(inputm+2)+ inputm+1] = sh/inputm;
                //qDebug() << sh/inputm;
            }
            delete[] buf;
        }
    }
    //计算
    cir[0][0] = 0;
    for (int i = 0; i < inputn; ++i)
    {
        double he = 0;
        for (int j = 0; j < inputn; ++j)
            he += nn_ww[i*(inputn+2) + j] * nn_ww[j*(inputn+2) + (inputn+1)];
        cir[0][0] += he/nn_ww[i*(inputn+2) + (inputn+1)];
    }
    cir[0][0] /= inputn;
    cir[0][1] = (cir[0][0]-inputn)/(inputn-1);
    cir[0][2] = RI[inputn-1];
    cir[0][3] = cir[0][1]/cir[0][2];
    //qDebug() << cir[0][0];

    for (int w = 1; w < inputn+1; ++w)
    {
        cir[w][0] = 0;
        for (int i = 0; i < inputm; ++i)
        {
            double he = 0;
            for (int j = 0; j < inputm; ++j)
                he += n_mm_ww[w-1][i*(inputm+2) + j] * n_mm_ww[w-1][j*(inputm+2) + (inputm+1)];
            cir[w][0] += he/n_mm_ww[w-1][i*(inputm+2) + (inputm+1)];
        }
        cir[w][0] /= inputm;
        cir[w][1] = (cir[w][0]-inputm)/(inputm-1);
        cir[w][2] = RI[inputm-1];
        cir[w][3] = cir[w][1]/cir[w][2];

        //qDebug() << cir[w][0];
    }

    cir[inputn+1][0] = 0;
    cir[inputn+1][1] = 0;
    for (int j = 0; j < inputn; ++j)
        cir[inputn+1][1] += nn_ww[j*(inputn+2) + (inputn+1)] * cir[j+1][1];
    cir[inputn+1][2] = RI[inputm-1];
    cir[inputn+1][3] = cir[inputn+1][1] / cir[inputn+1][2];

    //qDebug() << cir[inputn+1][0];
    //qDebug() << cir[inputn+1][1];
    //qDebug() << cir[inputn+1][2];
    //qDebug() << cir[inputn+1][3];


    for (int i = 0; i < inputm; ++i)
    {
        endw[i] = 0;
        for (int j = 0; j < inputn; ++j)
        {
            endw[i] += nn_ww[j*(inputn+2) + (inputn+1)] * n_mm_ww[j][i*(inputm+2) + (inputm+1)];
        }
        //qDebug() << endw[i];
    }
    //刷新结果表
    endnum->clear();
    endnum->setColumnCount(inputn+2);//
    endnum->setRowCount(inputm + 1 + 4);//

    QStringList header;  //QString类型的List容器

    char top[32];
    header << "W*";
    for (int i = 0; i < inputm; ++i)
    {
        sprintf(top, "企业%d", i+1);
        header << top;
        for (int j = 0; j < inputn; ++j)
            endnum->setItem(i+1, j, new QTableWidgetItem(QString::number(n_mm_ww[j][i*(inputm+2)+(inputm+1)])));
    }
    header << "入max" << "CI" << "RI" << "CR";
    endnum->setVerticalHeaderLabels(header);
    header.clear();

    for (int i = 0; i < inputn; ++i)
    {
        sprintf(top, "指标%d", i+1);
        header << top;
        endnum->setItem(0, i, new QTableWidgetItem(QString::number(nn_ww[i*(inputn+2) + (inputn+1)])));
    }
    header << "汇总" << "排名 及\n入* CI* RI* CR*";
    endnum->setHorizontalHeaderLabels(header);//设置表头内容
    header.clear();


    for (int i = 0; i < inputm; ++i)
        endnum->setItem(i+1, inputn, new QTableWidgetItem(QString::number(endw[i])));

    for (int i = 1; i < inputn+2; ++i)
    {
        for (int j = 0; j < 4; ++j)
            endnum->setItem(inputm + 1 + j, i-1, new QTableWidgetItem(QString::number(cir[i][j])));
    }
    endnum->setItem(inputm+1, inputn, new QTableWidgetItem(QString("表*数据=>>")));
    for (int i = 0; i < inputm; ++i)
    {
        int dex= 1;
        for (int j = 0; j < inputm; ++j)
            if (endw[i] < endw[j])
                ++dex;
        endnum->setItem(i+1, inputn+1, new QTableWidgetItem(QString::number(dex)));
    }

    for (int i = 0; i < 4; ++i)
        endnum->setItem(inputm+1 + i, inputn+1, new QTableWidgetItem(QString::number(cir[0][i])));
}

void Widget::two()//修改数据
{
    one();
}
void Widget::three()//清空
{
    int lastn = inputn;
    int lastm = inputm;
    inputn = 0;
    inputm = 0;
    now = 0;

    if (ppt)
    {
        for (int i = 0; i < lastn+1; ++i)
            delete ppt[i];
        delete[] ppt;
        ppt = nullptr;
    }



    if (nn_ww)
    {
        delete[] nn_ww;
        nn_ww = nullptr;
    }
    if (n_mm_ww)
    {
        for (int i = 0; i < lastn; ++i)
            delete[] n_mm_ww[i];
        delete[] n_mm_ww;
        n_mm_ww = nullptr;
    }
    if (cir)
    {
        delete[] cir;
        cir = nullptr;
    }
    if (endw)
    {
        delete[] endw;
        endw = nullptr;
    }

    endnum->clear();
}
Widget::~Widget()
{

}

double Widget::toD(QString text)
{
        if (text == "")
            return 0;
        double data = text.toDouble();
        if (data == 0)
        {
            double a = 0, b = 1;
            sscanf(text.toLatin1().data(), "%lf/%lf", &a, &b);
            data = a/b;
        }
        //qDebug() << data;
        return data;
}

void Widget::lastsolt()
{
    now -= 2;
    ppt[now+2]->close();
}
