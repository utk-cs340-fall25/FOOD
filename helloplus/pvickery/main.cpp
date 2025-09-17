#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //create a main window widget
    QWidget window;
    window.setWindowTitle("Ingredient Manager");

    //layout
    QVBoxLayout *layout = new QVBoxLayout(&window);

    //input field
    QLineEdit *ingredientInput = new QLineEdit();
    ingredientInput->setPlaceholderText("Enter an ingredient...");
    layout->addWidget(ingredientInput);

    //list to display ingredients
    QListWidget *ingredientList = new QListWidget();
    layout->addWidget(ingredientList);

    //add ingredient button
    QPushButton *addButton = new QPushButton("Add Ingredient");
    layout->addWidget(addButton);

    //clear ingredients button
    QPushButton *clearButton = new QPushButton("Clear Ingredients");
    layout->addWidget(clearButton);

    //connections
    QObject::connect(addButton, &QPushButton::clicked, [&]() {
        QString ingredient = ingredientInput->text();
        if (!ingredient.isEmpty()) {
            ingredientList->addItem(ingredient);
            ingredientInput->clear();
        }
    });

    QObject::connect(clearButton, &QPushButton::clicked, [&]() {
        ingredientList->clear();
    });

    //show window
    window.setLayout(layout);
    window.resize(300, 400);
    window.show();

    return a.exec();
}

