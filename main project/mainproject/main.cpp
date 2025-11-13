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
#include <QLabel>
#include <map>
#include <QStackedWidget>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include "provider_adapter.h"

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
    std::map<QString, Recipe> recipes; // kept for fallback but not used when provider available

    // // // on launch section // // //
    // Prefer provider-supplied in-memory data (Ethan). Do not read/write files here.
    QList<RRecipe> providerRecipes;
    bool providerLoaded = load_header_provider(providerRecipes, ingredients);
    if (!providerLoaded) {
        // Fallback: call local INIT (legacy behavior)
        status = INIT(recipes, ingredients);
    } else {
        status = STATUS_SUCCESS;
    }

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
    
    // Load recipes from provider 
    QList<RRecipe> recipeList;
    if (!load_header_recipes(recipeList)) {
        // Fallback: convert local Recipe map (older code path)
        for (auto it = recipes.begin(); it != recipes.end(); ++it) {
            RRecipe rrecipe;
            rrecipe.name = it->second.name;

            // Convert ingredients vector to string
            QString ingredientsStr;
            for (const auto& ingredient : it->second.ingredients) {
                ingredientsStr += ingredient.amount_s + " " + ingredient.name + "\n";
            }
            rrecipe.ingredients = ingredientsStr;

            // Convert instructions vector to string
            QString stepsStr;
            for (const auto& instruction : it->second.instructions) {
                stepsStr += instruction + "\n";
            }
            rrecipe.steps = stepsStr;

            // Convert tags vector to string
            QString tagsStr;
            for (const auto& tag : it->second.tags) {
                tagsStr += tag + " ";
            }
            rrecipe.tags = tagsStr.trimmed();

            rrecipe.time = 0;
            rrecipe.difficulty = "";
            rrecipe.region = "";
            rrecipe.tier = "";

            recipeList.append(rrecipe);
        }
    }
    rsearchPage->setRecipes(recipeList);

    // // // RsearchFunc section end // // //

    // Recipe Display
    QWidget *recipeDisplayPage = new QWidget(&mainWindow);
    QVBoxLayout *recipeDisplayLayout = new QVBoxLayout(recipeDisplayPage);
    QStackedWidget *recipeDetails = new QStackedWidget();
    QListWidget *recipeDisplayList = new QListWidget();

    QHBoxLayout *recipeLayout = new QHBoxLayout();
    recipeLayout->addWidget(recipeDisplayList);
    recipeLayout->addWidget(recipeDetails);

    recipeDisplayLayout->addLayout(recipeLayout);
    
    // Populate recipe display from loaded recipes
    for (const auto& rrecipe : recipeList) {
        recipeDisplayList->addItem(rrecipe.name);
        
        QWidget *page = new QWidget();
        QVBoxLayout *pageLayout = new QVBoxLayout(page);
        
        QLabel *titleLabel = new QLabel(rrecipe.name + " Recipe:");
        titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
        pageLayout->addWidget(titleLabel);
        
        QLabel *ingredientsLabel = new QLabel("Ingredients:");
        ingredientsLabel->setStyleSheet("font-weight: bold;");
        pageLayout->addWidget(ingredientsLabel);
        
        QTextEdit *ingredientsText = new QTextEdit();
        ingredientsText->setPlainText(rrecipe.ingredients);
        ingredientsText->setReadOnly(true);
        pageLayout->addWidget(ingredientsText);
        
        QLabel *stepsLabel = new QLabel("Steps:");
        stepsLabel->setStyleSheet("font-weight: bold;");
        pageLayout->addWidget(stepsLabel);
        
        QTextEdit *stepsText = new QTextEdit();
        stepsText->setPlainText(rrecipe.steps);
        stepsText->setReadOnly(true);
        pageLayout->addWidget(stepsText);
        
        pageLayout->addStretch();
        
        recipeDetails->addWidget(page);
    }

    QObject::connect(recipeDisplayList, &QListWidget::currentRowChanged,
                 recipeDetails, &QStackedWidget::setCurrentIndex);
    if (recipeDisplayList->count() > 0) {
        recipeDisplayList->setCurrentRow(0);
    }
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

    if (!provider_deinit()) {
        // if provider not used or deinit failed, try legacy DEINIT
        status = DEINIT(recipes, ingredients);
    }

    // // // on close section end // // //

    return a.exec();
}
