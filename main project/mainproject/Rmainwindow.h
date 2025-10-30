#ifndef RMAINWINDOW_H
#define RMAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QList>
#include "Rrsearchwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RMainWindow; }
QT_END_NAMESPACE

class RMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    RMainWindow(QWidget *parent = nullptr);
    ~RMainWindow();

private:
    Ui::RMainWindow *ui;
    RRSearchWindow *searchWidget;
};

#endif // RMAINWINDOW_H
