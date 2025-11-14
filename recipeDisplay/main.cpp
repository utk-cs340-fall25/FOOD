#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //create display widget
    QWidget window;
    window.setWindowTitle("Recipe Display");

     //layout
    QVBoxLayout *layout = new QVBoxLayout(&window);
    QStackedWidget *recipeDetails = new QStackedWidget();
    QListWidget *recipeList = new QListWidget();

    layout->addWidget(recipeList);

    //open and read recipes file
    QFile file("../main project/mainproject/recipes.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        recipeList->addItem("Error: Could not open recipes.txt");
    } else {
        QTextStream in(&file);
        QStringList allLines;
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            allLines << line;
        }
        file.close();

        QString recipeName;
        QStringList ingredients;


        for (const QString &line : allLines) {
            if (line.isEmpty()) {
                //blank line signals end of current recipe
                if (!recipeName.isEmpty()) {
                    recipeList->addItem(recipeName);

                    //create new widget and layout for recipe
                    QWidget *page = new QWidget();
                    QVBoxLayout *pageLayout = new QVBoxLayout(page);
                    pageLayout->addWidget(new QLabel(recipeName + " Recipe:\n" + ingredients.join("\n")));
                    recipeDetails->addWidget(page);

                    //reset for next recipe
                    recipeName.clear();
                    ingredients.clear();
                }
            } else {
                if (recipeName.isEmpty()) {
                    recipeName = line;       //first non-blank line is recipe name
                } else {
                    ingredients << line;     //subsequent lines are ingredients
                }
            }
        }

        //handle last recipe if file does not end with a blank line
        if (!recipeName.isEmpty()) {
            recipeList->addItem(recipeName);
            QWidget *page = new QWidget();
            QVBoxLayout *pageLayout = new QVBoxLayout(page);
            pageLayout->addWidget(new QLabel(recipeName + " Recipe:\n" + ingredients.join("\n")));
            recipeDetails->addWidget(page);
        }
    }
    //connect selection change to stacked widget index
    QObject::connect(recipeList, &QListWidget::currentRowChanged,
                     recipeDetails, &QStackedWidget::setCurrentIndex);

    //layout: list of recipes on left, recipe details on right
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(recipeList);
    mainLayout->addWidget(recipeDetails);

    layout->addLayout(mainLayout);
    recipeList->setCurrentRow(0);

    window.setLayout(layout);
    window.show();

    return a.exec();
}
