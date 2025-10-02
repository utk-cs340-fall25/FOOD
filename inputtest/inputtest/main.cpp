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
    map<QString, bool> ingredients;
    map<QString, bool>::iterator iit;

    // temporary map fillings
    ingredients["Zucchini"] = false;
    ingredients["Salt"] = false;
    ingredients["Oil"] = false;

    // temporary list fillings
    QListWidgetItem *item3 = new QListWidgetItem("Oil", selection);
    item3->setFlags(item3->flags() | ItemIsUserCheckable);
    item3->setCheckState(Unchecked);

    QListWidgetItem *item2 = new QListWidgetItem("Salt", selection);
    item2->setFlags(item2->flags() | ItemIsUserCheckable);
    item2->setCheckState(Unchecked);

    QListWidgetItem *item1 = new QListWidgetItem("Zucchini", selection);
    item1->setFlags(item1->flags() | ItemIsUserCheckable);
    item1->setCheckState(Unchecked);

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

    input.setLayout(layout);
    layout->addWidget(selection);
    layout->addWidget(output);

    input.show();
    return a.exec();
}
