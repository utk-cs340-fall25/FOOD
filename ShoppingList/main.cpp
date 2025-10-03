#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QListWidget>
#include <QStringList>
#include <QSet>
#include <QVector>
#include <QMap>

struct Recipe {
    QString name;
    QStringList ingredients;
};

class ShoppingListWidget : public QWidget {
    Q_OBJECT
public:
    ShoppingListWidget(QWidget *parent = nullptr) : QWidget(parent) {
        // Hardcoded recipes
        recipes.append({"Pasta", {"pasta", "tomato sauce", "cheese", "basil"}});
        recipes.append({"Omelette", {"eggs", "cheese", "milk", "butter"}});
        recipes.append({"Salad", {"lettuce", "tomato", "cucumber", "olive oil"}});

        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        recipeBox = new QComboBox();
        for (const Recipe &r : recipes)
            recipeBox->addItem(r.name);
        mainLayout->addWidget(new QLabel("Select Recipe:"));
        mainLayout->addWidget(recipeBox);

        ingredientGroup = new QGroupBox("Ingredients you have");
        ingLayout = new QVBoxLayout();
        ingredientGroup->setLayout(ingLayout);
        mainLayout->addWidget(ingredientGroup);

        mainLayout->addWidget(new QLabel("Missing ingredients (shopping list):"));
        shoppingList = new QListWidget();
        mainLayout->addWidget(shoppingList);

        // Qt 5 Style SIGNAL/SLOT
        connect(recipeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateIngredients(int)));

        updateIngredients(0);
    }

private slots:
    void updateIngredients(int index) {
        // Clear previous checkboxes
        QLayoutItem *child;
        while ((child = ingLayout->takeAt(0)) != nullptr) {
            QWidget *w = child->widget();
            if (w) {
                w->deleteLater();
            }
            delete child;
        }
        checkboxes.clear();

        const Recipe &recipe = recipes[index];
        for (const QString &ing : recipe.ingredients) {
            QCheckBox *cb = new QCheckBox(ing);
            ingLayout->addWidget(cb);
            checkboxes[ing] = cb;
            connect(cb, SIGNAL(stateChanged(int)), this, SLOT(updateShoppingList()));
        }
        updateShoppingList();
    }

    void updateShoppingList() {
        int index = recipeBox->currentIndex();
        if (index < 0 || index >= recipes.size()) return;
        const Recipe &recipe = recipes[index];

        QSet<QString> inventory;
        for (const QString &ing : recipe.ingredients) {
            if (checkboxes[ing]->isChecked())
                inventory.insert(ing);
        }
        shoppingList->clear();
        for (const QString &ing : recipe.ingredients) {
            if (!inventory.contains(ing))
                shoppingList->addItem(ing);
        }
    }
private:
    QVector<Recipe> recipes;
    QComboBox *recipeBox;
    QGroupBox *ingredientGroup;
    QVBoxLayout *ingLayout;
    QMap<QString, QCheckBox*> checkboxes;
    QListWidget *shoppingList;
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ShoppingListWidget w;
    w.setWindowTitle("Recipe Shopping List");
    w.resize(400, 400);
    w.show();
    return app.exec();
}
