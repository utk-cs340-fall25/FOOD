#include "tab.h"
#include "ui_tab.h"

tab::tab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::tab)
{
    ui->setupUi(this);
}

tab::~tab()
{
    delete ui;
}

