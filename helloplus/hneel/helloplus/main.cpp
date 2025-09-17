#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QtWidgets/QComboBox>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    QVBoxLayout *layout = new QVBoxLayout(&w);
    QPushButton *button = new QPushButton("Click click");
    QComboBox *selection = new QComboBox(&w);
    QLabel *output = new QLabel();
    QLabel *buttonPress = new QLabel();
    int count = 0;

    selection->addItem("Hello");
    selection->addItem("World");
    selection->addItem("Plus");
    selection->addItem("!");

    layout->addWidget(button);
    layout->addWidget(selection);
    layout->addWidget(output);
    layout->addWidget(buttonPress);

    QObject::connect(selection, &QComboBox::currentTextChanged, [&](const QString &text)
    {
        output->setText("Selection: " + text);
    });

    QObject::connect(button, &QPushButton::clicked, [&]()
    {
        count++;
        buttonPress->setText("Clicky clicked: " + QString::number(count));
    });

    w.setLayout(layout);
    w.resize(800, 400);
    w.show();

    return a.exec();
}
