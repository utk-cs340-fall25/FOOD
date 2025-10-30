#ifndef SHOPPINGLISTWIDGET_H
#define SHOPPINGLISTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QListWidget>
#include <QStringList>
#include <QSet>
#include <QVector>
#include <QMap>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QMenu>
#include <QAction>
#include <QDateTime>
#include <QStatusBar>

// Priority levels for shopping list items
enum Priority { LOW, MEDIUM, HIGH };

struct ShoppingListRecipe {
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

        // Date and time display
        QDateTime currentTime = QDateTime::currentDateTime();
        QLabel *dateTimeLabel = new QLabel("Shopping List - " + currentTime.toString("yyyy-MM-dd"));
        dateTimeLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(dateTimeLabel);

        // Recipe selection section
        QHBoxLayout *recipeSelectionLayout = new QHBoxLayout();
        QLabel *recipeLabel = new QLabel("Select Recipe:");
        recipeBox = new QComboBox();
        for (const ShoppingListRecipe &r : recipes)
            recipeBox->addItem(r.name);

        addButton = new QPushButton("Add to Menu");

        recipeSelectionLayout->addWidget(recipeLabel);
        recipeSelectionLayout->addWidget(recipeBox);
        recipeSelectionLayout->addWidget(addButton);

        mainLayout->addLayout(recipeSelectionLayout);

        // Selected recipes section
        selectedRecipesGroup = new QGroupBox("Selected Recipes");
        selectedRecipesLayout = new QVBoxLayout();
        selectedRecipesGroup->setLayout(selectedRecipesLayout);
        mainLayout->addWidget(selectedRecipesGroup);

        // Ingredients you have section
        ingredientGroup = new QGroupBox("Ingredients you have");
        ingLayout = new QVBoxLayout();
        ingredientGroup->setLayout(ingLayout);
        mainLayout->addWidget(ingredientGroup);

        // Shopping list section
        mainLayout->addWidget(new QLabel("Missing ingredients (shopping list):"));
        shoppingList = new QListWidget();
        shoppingList->setContextMenuPolicy(Qt::CustomContextMenu);
        mainLayout->addWidget(shoppingList);

        // Custom item section
        QGroupBox *customItemGroup = new QGroupBox("Add Custom Item");
        QHBoxLayout *customItemLayout = new QHBoxLayout();
        customItemInput = new QLineEdit();
        QPushButton *addCustomItemButton = new QPushButton("Add");
        QComboBox *priorityComboBox = new QComboBox();

        priorityComboBox->addItem("Low Priority", LOW);
        priorityComboBox->addItem("Medium Priority", MEDIUM);
        priorityComboBox->addItem("High Priority", HIGH);
        customItemPrioritySelector = priorityComboBox;

        customItemLayout->addWidget(customItemInput);
        customItemLayout->addWidget(priorityComboBox);
        customItemLayout->addWidget(addCustomItemButton);
        customItemGroup->setLayout(customItemLayout);
        mainLayout->addWidget(customItemGroup);

        // Clear button
        clearAllButton = new QPushButton("Clear All");
        mainLayout->addWidget(clearAllButton);

        // Status bar for help text
        statusBar = new QStatusBar();
        statusBar->showMessage("Right-click on shopping list items to set priority");
        mainLayout->addWidget(statusBar);

        // Connect signals/slots
        connect(addButton, &QPushButton::clicked, this, &ShoppingListWidget::addRecipeToMenu);
        connect(clearAllButton, &QPushButton::clicked, this, &ShoppingListWidget::clearAll);
        connect(addCustomItemButton, &QPushButton::clicked, this, &ShoppingListWidget::addCustomItem);
        connect(shoppingList, &QListWidget::customContextMenuRequested,
                this, &ShoppingListWidget::showItemContextMenu);

        // Initial update of ingredients display
        updateIngredientsDisplay();
    }

private slots:
    void addRecipeToMenu() {
        int index = recipeBox->currentIndex();
        if (index < 0 || index >= recipes.size()) return;

        // Check if the recipe is already added
        QString recipeName = recipes[index].name;
        if (selectedRecipes.contains(recipeName)) {
            QMessageBox::information(this, "Already Added",
                                    "This recipe is already in your menu.");
            return;
        }

        // Add recipe to selected list
        selectedRecipes.insert(recipeName);

        // Add checkbox to selected recipes section
        QCheckBox *recipeCheckBox = new QCheckBox(recipeName);
        selectedRecipesLayout->addWidget(recipeCheckBox);
        recipeCheckBoxes[recipeName] = recipeCheckBox;

        // Connect checkbox signal to update ingredients when toggled
        connect(recipeCheckBox, &QCheckBox::stateChanged, this, &ShoppingListWidget::updateIngredientsDisplay);

        // Update the display with the new recipe
        updateIngredientsDisplay();
    }

    void updateIngredientsDisplay() {
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

        // Create a set of all ingredients from selected recipes
        QSet<QString> allIngredients;
        for (const QString &recipeName : selectedRecipes) {
            // Only include ingredients for checked recipes
            QCheckBox *recipeCheckBox = recipeCheckBoxes.value(recipeName);
            if (recipeCheckBox && recipeCheckBox->isChecked()) {
                for (const ShoppingListRecipe &r : recipes) {
                    if (r.name == recipeName) {
                        for (const QString &ing : r.ingredients) {
                            allIngredients.insert(ing);
                        }
                        break;
                    }
                }
            }
        }

        // Create checkboxes for all unique ingredients
        for (const QString &ing : allIngredients) {
            QCheckBox *cb = new QCheckBox(ing);
            ingLayout->addWidget(cb);
            checkboxes[ing] = cb;
            connect(cb, &QCheckBox::stateChanged, this, &ShoppingListWidget::updateShoppingList);
        }

        updateShoppingList();
    }

    void updateShoppingList() {
        // Store current priorities before clearing the list
        QMap<QString, Priority> currentPriorities;
        for (int i = 0; i < shoppingList->count(); i++) {
            QListWidgetItem* item = shoppingList->item(i);
            QString itemText = item->text();
            if (itemPriorities.contains(itemText)) {
                currentPriorities[itemText] = itemPriorities[itemText];
            }
        }

        // Create a set of all ingredients from selected recipes
        QSet<QString> requiredIngredients;
        for (const QString &recipeName : selectedRecipes) {
            // Only include ingredients for checked recipes
            QCheckBox *recipeCheckBox = recipeCheckBoxes.value(recipeName);
            if (recipeCheckBox && recipeCheckBox->isChecked()) {
                for (const ShoppingListRecipe &r : recipes) {
                    if (r.name == recipeName) {
                        for (const QString &ing : r.ingredients) {
                            requiredIngredients.insert(ing);
                        }
                        break;
                    }
                }
            }
        }

        // Check which ingredients user has
        QSet<QString> inventory;
        for (const QString &ing : requiredIngredients) {
            if (checkboxes.contains(ing) && checkboxes[ing]->isChecked())
                inventory.insert(ing);
        }

        // Update the shopping list
        shoppingList->clear();

        // First, add recipe ingredients
        for (const QString &ing : requiredIngredients) {
            if (!inventory.contains(ing)) {
                QListWidgetItem* item = new QListWidgetItem(ing);
                shoppingList->addItem(item);

                // Restore or set priority
                if (currentPriorities.contains(ing)) {
                    itemPriorities[ing] = currentPriorities[ing];
                } else if (!itemPriorities.contains(ing)) {
                    itemPriorities[ing] = LOW; // Default priority
                }
                updateItemDisplay(item);
            }
        }

        // Then, add custom items (keep them in the list)
        for (const QString &customItem : customItems) {
            QListWidgetItem* item = new QListWidgetItem(customItem);
            shoppingList->addItem(item);
            updateItemDisplay(item);
        }
    }

    void addCustomItem() {
        QString item = customItemInput->text().trimmed();
        if (!item.isEmpty()) {
            // Check if item already exists
            bool alreadyExists = false;
            for (int i = 0; i < shoppingList->count(); i++) {
                if (shoppingList->item(i)->text() == item) {
                    alreadyExists = true;
                    break;
                }
            }

            if (!alreadyExists) {
                // Get selected priority
                Priority priority = static_cast<Priority>(
                    customItemPrioritySelector->currentData().toInt());

                // Add item to our custom items list
                customItems.insert(item);

                // Set the priority
                itemPriorities[item] = priority;

                // Add to the shopping list
                QListWidgetItem* newItem = new QListWidgetItem(item);
                shoppingList->addItem(newItem);
                updateItemDisplay(newItem);

                // Clear the input field
                customItemInput->clear();

                statusBar->showMessage("Custom item added: " + item, 2000);
            } else {
                statusBar->showMessage("Item already in list", 2000);
            }
        }
    }

    void showItemContextMenu(const QPoint &pos) {
        QListWidgetItem *item = shoppingList->itemAt(pos);
        if (!item)
            return;

        QMenu contextMenu(tr("Item Priority"), this);

        QAction *lowAction = new QAction(tr("Low Priority"), this);
        QAction *mediumAction = new QAction(tr("Medium Priority"), this);
        QAction *highAction = new QAction(tr("High Priority"), this);

        connect(lowAction, &QAction::triggered, [this, item]() {
            setPriority(item, LOW);
        });

        connect(mediumAction, &QAction::triggered, [this, item]() {
            setPriority(item, MEDIUM);
        });

        connect(highAction, &QAction::triggered, [this, item]() {
            setPriority(item, HIGH);
        });

        contextMenu.addAction(lowAction);
        contextMenu.addAction(mediumAction);
        contextMenu.addAction(highAction);

        contextMenu.exec(shoppingList->viewport()->mapToGlobal(pos));
    }

    void setPriority(QListWidgetItem *item, Priority priority) {
        if (!item) return;

        QString itemText = item->text();
        itemPriorities[itemText] = priority;
        updateItemDisplay(item);

        statusBar->showMessage("Priority updated for: " + itemText, 2000);
    }

    void updateItemDisplay(QListWidgetItem* item) {
        QString name = item->text();
        if (itemPriorities.contains(name)) {
            switch(itemPriorities[name]) {
                case HIGH:
                    item->setForeground(Qt::red);
                    item->setIcon(QIcon::fromTheme("emblem-important"));
                    break;
                case MEDIUM:
                    item->setForeground(QColor(255, 165, 0)); // Orange
                    break;
                case LOW:
                default:
                    item->setForeground(Qt::black);
                    break;
            }
        }
    }

    void clearAll() {
        // Clear selected recipes
        selectedRecipes.clear();

        // Clear custom items
        customItems.clear();

        // Optionally clear item priorities or keep them for next time
        // itemPriorities.clear();

        // Remove all recipe checkboxes
        QLayoutItem *child;
        while ((child = selectedRecipesLayout->takeAt(0)) != nullptr) {
            QWidget *w = child->widget();
            if (w) {
                w->deleteLater();
            }
            delete child;
        }
        recipeCheckBoxes.clear();

        // Clear ingredients and shopping list
        updateIngredientsDisplay();

        statusBar->showMessage("All items cleared", 2000);
    }

private:
    QVector<ShoppingListRecipe> recipes;
    QComboBox *recipeBox;
    QPushButton *addButton;
    QPushButton *clearAllButton;
    QGroupBox *selectedRecipesGroup;
    QVBoxLayout *selectedRecipesLayout;
    QGroupBox *ingredientGroup;
    QVBoxLayout *ingLayout;
    QMap<QString, QCheckBox*> checkboxes;
    QMap<QString, QCheckBox*> recipeCheckBoxes;
    QListWidget *shoppingList;
    QSet<QString> selectedRecipes;
    QSet<QString> customItems;
    QMap<QString, Priority> itemPriorities;
    QLineEdit *customItemInput;
    QComboBox *customItemPrioritySelector;
    QStatusBar *statusBar;
};

#endif // SHOPPINGLISTWIDGET_H
