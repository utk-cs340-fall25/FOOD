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
    std::map<std::string, Ingredient> ingredients_buffer;
    status = INIT(recipes, ingredients);
//    if (status != STATUS_SUCCESS) { goto exit; }
    // temporary map fillings - replace with loading of the map here!
    ingredients["Zucchini"] = false;
    ingredients["Salt"] = false;
    ingredients["Oil"] = false;
    ingredients["Bihon Noodles"] = false;

    // // // on launch section end // // //



    // // // inputPage section // // //

    // setting the layouts
    inputPageLayout->addWidget(inputSearchBar);
    inputPageLayout->addWidget(inputSearchButton);
    inputPageLayout->addWidget(inputSelection);
    inputPageLayout->addWidget(inputOutput);

    inputOutput->setReadOnly(true);
    inputOutput->setText("Ingredients Owned:");

    // dynamically creating the list from the map
    for(iit = ingredients.begin(); iit != ingredients.end(); iit++)
    {
        QListWidgetItem *item = new QListWidgetItem(iit->first, inputSelection);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
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

    // Feed data from INIT-loaded maps into the widget (avoid reading files)
    {
        QList<RRecipe> uiRecipes;
        uiRecipes.reserve(static_cast<int>(recipes.size()));
        for (const auto &kv : recipes)
        {
            const QString &name = kv.first;
            const Recipe &src = kv.second;

            RRecipe r;
            r.name = name;
            // Build ingredients text
            QStringList ingLines;
            for (const Ingredient &ing : src.ingredients) {
                QString line = ing.name;
                if (!ing.amount_s.isEmpty()) {
                    line += " - " + ing.amount_s;
                    if (!ing.unit.isEmpty()) line += " " + ing.unit;
                } else if (ing.amount_d > 0) {
                    line += " - " + QString::number(ing.amount_d);
                    if (!ing.unit.isEmpty()) line += " " + ing.unit;
                }
                ingLines << line;
            }
            r.ingredients = ingLines.join("\n");
            // Build steps and tags from std::vector<QString>
            QStringList stepsList;
            for (const QString &s : src.instructions) stepsList << s;
            r.steps = stepsList.join("\n");
            QStringList tagsList;
            for (const QString &t : src.tags) tagsList << t;
            r.tags = tagsList.join(", ");
            r.time = 0;
            r.difficulty = "";
            r.region = "";
            r.tier = "";
            uiRecipes.push_back(r);
        }
        rsearchPage->setRecipes(uiRecipes);
    }

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
