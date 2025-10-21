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
#include <map>

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

    // // // on launch section // // //

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
                if( !( temp->text().contains( inputSearchBar->text() ) ) )
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




    // // // main application display - add your pages here! // // //

    // Note: Add your layouts to your page widget, not the tabs or mainwindow!
    //       See input section for example

    // tabs->addTab(yourPage, "Page Title");
    tabs->addTab(inputPage, "Input");


    mainWindowLayout->addWidget(tabs);
    mainWindow.resize(800, 600);
    mainWindow.show();

    // // // main application display end // // //



    // // // on close section // // //

    // This runs on close - clean up any resources you allocate!

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


        // main application frees
        delete tabs;
        delete mainWindowLayout;
    });

    // // // on close section end // // //



    return a.exec();
}
