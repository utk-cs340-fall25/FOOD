#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create display widget
    QWidget window;
    window.setWindowTitle("Recipe Display");

    // layout
    QVBoxLayout *layout = new QVBoxLayout(&window);

    //List of recipes
    QStackedWidget *recipeDetails = new QStackedWidget();

    QListWidget *recipeList = new QListWidget();
    layout->addWidget(recipeList);
    recipeList->addItem("Spaghetti Bolognese");
    recipeList->addItem("Fried Rice");
    recipeList->addItem("Chocolate Chip Cookie");

    // create recipe pages
    QWidget *spaghettiPage = new QWidget();
    QVBoxLayout *spaghettiLayout = new QVBoxLayout(spaghettiPage);
    spaghettiLayout->addWidget(new QLabel("Spaghetti Bolognese Recipe:\n 1 lb Ground Beef\n 1 lb Pasta \n 1 Jar Tomato Sauce "));

    QWidget *friedRicePage = new QWidget();
    QVBoxLayout *friedRiceLayout = new QVBoxLayout(friedRicePage);
    friedRiceLayout->addWidget(new QLabel("Fried Rice Recipe:\n White Rice\n Eggs\n Vegetable Oil\n Soy Sauce\n Carrots\n Peas\n White Onion"));

    QWidget *cookiePage = new QWidget();
    QVBoxLayout *cookieLayout = new QVBoxLayout(cookiePage);
    cookieLayout->addWidget(new QLabel("Chocolate Chip Cookie Recipe:\n Eggs\n Flour\n Vanilla extract\n Salt\n Baking Soda\n Butter\n Sugar\n Brown Sugar\n Chocolate Chips"));

    // add recipe pages to stacked widget
    recipeDetails->addWidget(spaghettiPage);
    recipeDetails->addWidget(friedRicePage);
    recipeDetails->addWidget(cookiePage);

    // connect selection change to stacked widget index
    QObject::connect(recipeList, &QListWidget::currentRowChanged,
                     recipeDetails, &QStackedWidget::setCurrentIndex);
    // layout: list on left, details on right
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(recipeList);
    mainLayout->addWidget(recipeDetails);

    layout->addLayout(mainLayout);

    // set default selection
    recipeList->setCurrentRow(0);
    window.setLayout(layout);
    window.show();

    return a.exec();
}
