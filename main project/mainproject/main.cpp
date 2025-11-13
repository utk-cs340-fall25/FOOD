#include "food.h"
#include "Rrsearchwindow.h"
//#include "../../ShoppingList/ShoppingListWidget.h"
#include "Rinventorymodel.h"
#include "Radditemdialog.h"
#include "Rmainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QTabWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QTextStream>
#include <map>
#include <QStackedWidget>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    // main application variables
    QApplication a(argc, argv);
    QWidget mainWindow;
    QVBoxLayout *mainWindowLayout = new QVBoxLayout(&mainWindow);
    QTabWidget *tabs = new QTabWidget(&mainWindow);

    // input variables
    QWidget *inputPage = new QWidget(&mainWindow);
    QListWidget *inputSelection = new QListWidget(inputPage);
    QPushButton *inputSearchButton = new QPushButton("Search", inputPage);
    QVBoxLayout *inputPageLayout = new QVBoxLayout(inputPage);
    QTextEdit *inputOutput = new QTextEdit(inputPage);
    QLineEdit *inputSearchBar = new QLineEdit(inputPage);
    std::map<QString, bool> ingredients;
    std::map<QString, bool>::iterator iit;

    STATUS status;
    std::map<QString, Recipe> recipes;

    // // // on launch section // // //
    status = INIT(recipes, ingredients);
//    if (status != STATUS_SUCCESS) { goto exit; }

    // temporary map fillings - replace with loading of the map here!

    // // // on launch section end // // //



    // // // inputPage section // // //

    // setting the layouts
    inputPageLayout->addWidget(inputSearchBar);
    inputPageLayout->addWidget(inputSearchButton);
    inputPageLayout->addWidget(inputSelection);
    inputPageLayout->addWidget(inputOutput);

    inputOutput->setReadOnly(true);
    inputOutput->clear();
    inputOutput->append("Ingredients Owned:");

    // dynamically creating the list from the map
    for(iit = ingredients.begin(); iit != ingredients.end(); iit++)
    {
        QListWidgetItem *item = new QListWidgetItem(iit->first, inputSelection);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        if(iit->second == false) item->setCheckState(Qt::Unchecked);
        else
        {
            item->setCheckState(Qt::Checked);
            // if we have it, add it on to the display
            inputOutput->append(iit->first);
        }
    }

    // search function
    QObject::connect(inputSearchButton, &QPushButton::clicked, [&]()
    {
        // restore search when emtpy
        if( inputSearchBar->text().isEmpty() )
        {
            for( int i = 0; i < inputSelection->count(); i++ )
            {
                QListWidgetItem *temp = inputSelection->item(i);
                temp->setHidden(false);

            }
        } else
        {
            // go thru the list and hide the ones that do not have the search term & show the ones that do
            for( int i = 0; i < inputSelection->count(); i++ )
            {
                QListWidgetItem *temp = inputSelection->item(i);        

                // to lower for case insentive search
                if( !( temp->text().toLower().contains( inputSearchBar->text().toLower() ) ) )
                {
                    temp->setHidden(true);
                } else
                {
                    temp->setHidden(false);
                }
            }
        }
    });

    // checkbox selection
    QObject::connect(inputSelection, &QListWidget::itemChanged, [&](const QListWidgetItem *item)
    {
        if(item->checkState() == Qt::Checked)
        {
            // checked ingredient had
            ingredients[item->text()] = true;
        } else
        {
            // unchecked ingredient no had
            ingredients[item->text()] = false;
        }

        // outputting the list of ingredients we have
        inputOutput->clear();
        inputOutput->append("Ingredients Owned:");
        for( iit = ingredients.begin(); iit != ingredients.end(); iit++)
        {
            // if we have it, add it on to the display
            if( iit->second == true )
            {
                inputOutput->append(iit->first);
            }
        }
    });


    // // // inputPage section end // // //


    // // // Shopping List section // // //

    // Create the Shopping List widget
    //ShoppingListWidget *shoppingListPage = new ShoppingListWidget(&mainWindow);

    // // // Shopping List section end // // //

    // // // RsearchFunc section // // //
    
    // Create the RsearchFunc widget
    RRSearchWindow *rsearchPage = new RRSearchWindow(&mainWindow);
    
    // Convert loaded recipes to RRecipe format for the search window
    QList<RRecipe> convertedRecipes;
    for (auto& recipeEntry : recipes) {
        RRecipe rrecipe;
        rrecipe.name = recipeEntry.first;   // if recipeEntry.first is already a QString
        
        // Combine ingredients into a string
        QString ingredientStr;
        for (const auto& ingredient : recipeEntry.second.ingredients) {
            if (!ingredientStr.isEmpty()) ingredientStr += ", ";
                ingredientStr += ingredient.name + " " + ingredient.amount_s + "\n";
        }
        rrecipe.ingredients = ingredientStr;
        
        // Combine instructions into a string
        QString instructionStr;
        for (const auto& instruction : recipeEntry.second.instructions) {
            if (!instructionStr.isEmpty()) instructionStr += "\n";
            instructionStr += instruction;      // if instruction is already a QString
        }
        rrecipe.steps = instructionStr;
        
        rrecipe.time = 0;  // Not available in Recipe struct
        rrecipe.difficulty = "";
        rrecipe.tags = "";
        rrecipe.region = "";
        rrecipe.tier = "";
        
        convertedRecipes.append(rrecipe);
    }
    
    // Set recipes in the search window
    rsearchPage->setRecipes(convertedRecipes);

    // // // RsearchFunc section end // // //

    // Recipe Display
    QWidget *recipeDisplayPage = new QWidget(&mainWindow);
    QVBoxLayout *recipeDisplayLayout = new QVBoxLayout(recipeDisplayPage);
    QStackedWidget *recipeDetails = new QStackedWidget();
    QListWidget *recipeList = new QListWidget();

    QHBoxLayout *recipeLayout = new QHBoxLayout();
    recipeLayout->addWidget(recipeList);
    recipeLayout->addWidget(recipeDetails);

    recipeDisplayLayout->addLayout(recipeLayout);
    QFile file("recipe.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        recipeList->addItem("Error: Could not open recipe.txt");
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
            if (!recipeName.isEmpty()) {
                recipeList->addItem(recipeName);
                QWidget *page = new QWidget();
                QVBoxLayout *pageLayout = new QVBoxLayout(page);
                pageLayout->addWidget(new QLabel(recipeName + " Recipe:\n" + ingredients.join("\n")));
                recipeDetails->addWidget(page);

                recipeName.clear();
                ingredients.clear();
            }
        } else {
            if (recipeName.isEmpty()) {
                recipeName = line;
            } else {
                ingredients << line;
            }
        }
    }

    if (!recipeName.isEmpty()) {
        recipeList->addItem(recipeName);
        QWidget *page = new QWidget();
        QVBoxLayout *pageLayout = new QVBoxLayout(page);
        pageLayout->addWidget(new QLabel(recipeName + " Recipe:\n" + ingredients.join("\n")));
        recipeDetails->addWidget(page);
    }
}

    QObject::connect(recipeList, &QListWidget::currentRowChanged,
                 recipeDetails, &QStackedWidget::setCurrentIndex);
    recipeList->setCurrentRow(0);
    // // // main application display - add your pages here! // // //

    // Note: Add your layouts to your page widget, not the tabs or mainwindow!
    //       See input section for example

    // tabs->addTab(yourPage, "Page Title");
    tabs->addTab(inputPage, "Input");
    tabs->addTab(rsearchPage, "Recipe Search");
    tabs->addTab(recipeDisplayPage, "Recipe Display");
    //tabs->addTab(shoppingListPage, "Shopping List");

    mainWindowLayout->addWidget(tabs);
    mainWindow.resize(800, 600);
    mainWindow.show();

    // // // main application display end // // //



    // // // on close section // // //
    // This runs on close - clean up any resources you allocate!

    // Label for immediately closing the application due to errors.
    // This label should always be above any code in the "on close" section.
    //exit:

    QObject::connect(&a, &QApplication::lastWindowClosed, [&]()
    {
        // input frees
        for( int i = 0; i < inputSelection->count(); i++ )
        {
            QListWidgetItem *temp = inputSelection->item(i);
            delete temp;
        }

        delete inputSelection;
        delete inputOutput;
        delete inputSearchBar;
        delete inputSearchButton;
        delete inputPageLayout;
        delete inputPage;

        // Shopping List frees
        //delete shoppingListPage;

        // RsearchFunc frees
        delete rsearchPage;

        // main application frees
        delete tabs;
        delete mainWindowLayout;
    });

    status = DEINIT(recipes, ingredients);

    // // // on close section end // // //

    return a.exec();
}
