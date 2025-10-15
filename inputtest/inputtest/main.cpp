#include "input.h"

#include <QApplication>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QLabel>
#include <map>
#include <QString>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>

using namespace Qt;
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Input input;
    QVBoxLayout *layout = new QVBoxLayout(&input);
    QListWidget *selection = new QListWidget(&input);
    //QLabel *output = new QLabel();
    QTextEdit *output = new QTextEdit(&input);
    QLineEdit *searchBar = new QLineEdit(&input);
    QPushButton *enter = new QPushButton("Search");
    map<QString, bool> ingredients;
    map<QString, bool>::iterator iit;

    // temporary map fillings
    ingredients["Zucchini"] = false;
    ingredients["Salt"] = false;
    ingredients["Oil"] = false;
    ingredients["Bihon Noodles"] = false;

    // temporary list fillings
    //QListWidgetItem *item3 = new QListWidgetItem("Oil", selection);
    //item3->setFlags(item3->flags() | ItemIsUserCheckable);
    //item3->setCheckState(Unchecked);

    // dynamically creating the list from the map
    for(iit = ingredients.begin(); iit != ingredients.end(); iit++)
    {
        QListWidgetItem *item = new QListWidgetItem(iit->first, selection);
        item->setFlags(item->flags() | ItemIsUserCheckable);
        item->setCheckState(Unchecked);
    }

    // for the list of ingridents had & set default text
    output->setReadOnly(true);
    output->setText("Ingredients Owned:");

    // checkbox selection
    QObject::connect(selection, &QListWidget::itemChanged, [&](const QListWidgetItem *item)
    {
        if(item->checkState() == Checked)
        {
            // checked ingrident had
            ingredients[item->text()] = true;
        } else
        {
            // unchecked ingrident no had
            ingredients[item->text()] = false;
        }

        // outputting the list of ingredients we have
        output->clear();
        output->append("Ingredients Owned:");
        for( iit = ingredients.begin(); iit != ingredients.end(); iit++)
        {
            // if we have it, add it on
            if( iit->second == true )
            {
                output->append(iit->first);
            }
        }
    });

    // search function
    QObject::connect(enter, &QPushButton::clicked, [&]()
    {
        // restore search when emtpy
        if( searchBar->text().isEmpty() )
        {
            for( int i = 0; i < selection->count(); i++ )
            {
                QListWidgetItem *temp = selection->item(i);
                temp->setHidden(false);

            }
        } else
        {
            // go thru the list and hide the ones that do not have the search term & show the ones that do
            for( int i = 0; i < selection->count(); i++ )
            {
                QListWidgetItem *temp = selection->item(i);
                if( !( temp->text().contains( searchBar->text() ) ) )
                {
                    temp->setHidden(true);
                } else
                {
                    temp->setHidden(false);
                }
            }
        }
    });

    input.setLayout(layout);
    layout->addWidget(searchBar);
    layout->addWidget(enter);
    layout->addWidget(selection);
    layout->addWidget(output);

    input.show();

    // freeing on quit
    QObject::connect(&a, &QApplication::lastWindowClosed, [&]()
    {
        for( int i = 0; i < selection->count(); i++ )
        {
            QListWidgetItem *temp = selection->item(i);
            delete temp;
        }

        delete enter;
        delete searchBar;
        delete output;
        delete selection;
        delete layout;
    });

    return a.exec();
}
