#include "tab.h"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget window;
    QWidget dummyPage;
    QWidget dummyPage2;
    QPushButton *button = new QPushButton("Button", &dummyPage);
    QPushButton *button2 = new QPushButton("Button2", &dummyPage2);
    QTextEdit *input = new QTextEdit("This is the input page", &dummyPage);
    QVBoxLayout *layout = new QVBoxLayout(&window);
    QVBoxLayout *page1Layout = new QVBoxLayout(&dummyPage);
    QVBoxLayout *page2Layout = new QVBoxLayout(&dummyPage2);
    QTabWidget *tabs = new QTabWidget(&window);


    // add the layout to the page widget directly, NOT to the tab widget !
    dummyPage.setLayout(page1Layout);
    page1Layout->addWidget(button);
    page1Layout->addWidget(input);

    dummyPage2.setLayout(page2Layout);
    page2Layout->addWidget(button2);

    tabs->addTab(&dummyPage, "Input");
    tabs->addTab(&dummyPage2, "Recipes");

    window.setLayout(layout);
    layout->addWidget(tabs);

    window.resize(800, 600);

    window.show();

    QObject::connect(&a, &QApplication::lastWindowClosed, [&]()
    {
        delete layout;
        delete page1Layout;
        delete page2Layout;
        delete button;
        delete input;
        delete button2;
        delete tabs;
    });

    return a.exec();
}
