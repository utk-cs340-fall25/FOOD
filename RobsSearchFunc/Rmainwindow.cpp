#include "Rmainwindow.h"
#include "ui_rmainwindow.h"

RMainWindow::RMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RMainWindow)
{
    ui->setupUi(this);
    
    // Create the search widget
    searchWidget = new RRSearchWindow(this);
    
    // Set it as the central widget
    setCentralWidget(searchWidget);
}

RMainWindow::~RMainWindow()
{
    delete ui;
}
