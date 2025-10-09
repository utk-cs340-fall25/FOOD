#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Sample recipe list
    recipeList = {"Pancakes", "Spaghetti", "Salad", "Omelette", "Smoothie"};

    // Initially display all recipes
    ui->listWidget->addItems(recipeList);

    // Connect search bar to filter function
    connect(ui->searchBar, &QLineEdit::textChanged, this, &MainWindow::filterRecipes);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Function to filter recipes based on search text
void MainWindow::filterRecipes(const QString &text)
{
    ui->listWidget->clear();

    for (const QString &recipe : recipeList)
    {
        if (recipe.contains(text, Qt::CaseInsensitive))
        {
            ui->listWidget->addItem(recipe);
        }
    }
}
