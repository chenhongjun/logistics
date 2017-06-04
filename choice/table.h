#ifndef TABLE_H
#define TABLE_H
#include <QTableWidget>
#include <QWidget>
#include <QPushButton>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
class Widget;
class Table : public QDialog
{
    Q_OBJECT
    friend class Widget;
public:
    Table(int w, int h, QWidget *parent = 0);
    ~Table();
private:
    int mywidth_;
    int myheight_;

private:
    QTableWidget* data;
    QPushButton* last;
    QPushButton* next;

    QVBoxLayout* mainLayout;
    QHBoxLayout* dataLayout;
    QHBoxLayout* pushLayout;
};
#endif // TABLE_H
