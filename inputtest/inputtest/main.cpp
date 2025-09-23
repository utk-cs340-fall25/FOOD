#include "input.h"

#include <QApplication>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QLabel>

using namespace Qt;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Input input;
    QVBoxLayout *layout = new QVBoxLayout(&input);
    QListWidget *selection = new QListWidget(&input);
    QLabel *output = new QLabel();

    QListWidgetItem *item1 = new QListWidgetItem("Zucchini", selection);
    item1->setFlags(item1->flags() | ItemIsUserCheckable);
    item1->setCheckState(Unchecked);

    QListWidgetItem *item2 = new QListWidgetItem("Salt", selection);
    item2->setFlags(item2->flags() | ItemIsUserCheckable);
    item2->setCheckState(Unchecked);

    QListWidgetItem *item3 = new QListWidgetItem("Oil", selection);
    item3->setFlags(item3->flags() | ItemIsUserCheckable);
    item3->setCheckState(Unchecked);

    QObject::connect(selection, &QListWidget::itemChanged, [&](const QListWidgetItem *item)
    {
        if(item->checkState() == Checked)
        {
            output->setText(item->text());
        } else
        {
            output->setText("No " + item->text() + " :(");
        }
    });

    input.setLayout(layout);
    layout->addWidget(selection);
    layout->addWidget(output);

    input.show();
    return a.exec();
}
