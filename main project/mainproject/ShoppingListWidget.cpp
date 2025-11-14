#include "ShoppingListWidget.h"
#include <QLayout>
#include <QTextStream>

ShoppingListWidget::ShoppingListWidget(const std::map<QString, Recipe>& recipesRef,
                                       QWidget *parent)
    : QWidget(parent), recipes(recipesRef)
{
    QVBoxLayout *root = new QVBoxLayout(this);

    QDateTime currentTime = QDateTime::currentDateTime();
    QLabel *title = new QLabel("Shopping List - " + currentTime.toString("yyyy-MM-dd"));
    title->setAlignment(Qt::AlignCenter);
    root->addWidget(title);

    // Upper split layout: recipes (left) and ingredients (middle), actions (right)
    QHBoxLayout *topLayout = new QHBoxLayout();
    root->addLayout(topLayout);

    // Recipe list
    QVBoxLayout *recipesCol = new QVBoxLayout();
    recipesCol->addWidget(new QLabel("Recipes"));
    recipeList = new QListWidget();
    recipeList->setSelectionMode(QAbstractItemView::SingleSelection);
    recipesCol->addWidget(recipeList);
    topLayout->addLayout(recipesCol, 1);

    // Populate recipes
    for (const auto &pair : recipes) {
        recipeList->addItem(pair.first);
    }

    // Ingredient list + filter
    QVBoxLayout *ingredientsCol = new QVBoxLayout();
    ingredientsCol->addWidget(new QLabel("Ingredients"));
    ingredientFilter = new QLineEdit();
    ingredientFilter->setPlaceholderText("Filter ingredients...");
    ingredientsCol->addWidget(ingredientFilter);
    ingredientList = new QListWidget();
    ingredientList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ingredientsCol->addWidget(ingredientList);
    topLayout->addLayout(ingredientsCol, 1);

    // Actions column
    QVBoxLayout *actionsCol = new QVBoxLayout();
    actionsCol->addWidget(new QLabel("Add Ingredients"));
    browsePrioritySelector = new QComboBox();
    browsePrioritySelector->addItem("Low Priority", SL_LOW);
    browsePrioritySelector->addItem("Medium Priority", SL_MEDIUM);
    browsePrioritySelector->addItem("High Priority", SL_HIGH);
    actionsCol->addWidget(browsePrioritySelector);

    addSelectedButton = new QPushButton("Add Selected");
    addAllButton      = new QPushButton("Add All From Recipe");
    actionsCol->addWidget(addSelectedButton);
    actionsCol->addWidget(addAllButton);
    actionsCol->addStretch();
    topLayout->addLayout(actionsCol, 0);

    // Shopping list section
    root->addWidget(new QLabel("Shopping List"));
    shoppingList = new QListWidget();
    shoppingList->setContextMenuPolicy(Qt::CustomContextMenu);
    root->addWidget(shoppingList);

    // Custom item entry
    QGroupBox *customGroup = new QGroupBox("Add Custom Item");
    QHBoxLayout *customLayout = new QHBoxLayout(customGroup);
    customItemEdit = new QLineEdit();
    prioritySelector = new QComboBox();
    prioritySelector->addItem("Low Priority", SL_LOW);
    prioritySelector->addItem("Medium Priority", SL_MEDIUM);
    prioritySelector->addItem("High Priority", SL_HIGH);
    addCustomButton = new QPushButton("Add");
    customLayout->addWidget(customItemEdit);
    customLayout->addWidget(prioritySelector);
    customLayout->addWidget(addCustomButton);
    root->addWidget(customGroup);

    statusBar = new QStatusBar();
    statusBar->showMessage("Double-click ingredient or use buttons. Right-click list items to set priority.");
    root->addWidget(statusBar);

    // Connections
    connect(recipeList, &QListWidget::currentItemChanged,
            this, &ShoppingListWidget::onRecipeSelectionChanged);
    connect(ingredientList, &QListWidget::itemDoubleClicked,
            this, &ShoppingListWidget::onIngredientDoubleClicked);
    connect(addSelectedButton, &QPushButton::clicked,
            this, &ShoppingListWidget::addSelectedIngredients);
    connect(addAllButton, &QPushButton::clicked,
            this, &ShoppingListWidget::addAllIngredients);
    connect(addCustomButton, &QPushButton::clicked,
            this, &ShoppingListWidget::addCustomItem);
    connect(shoppingList, &QListWidget::customContextMenuRequested,
            this, &ShoppingListWidget::showItemContextMenu);
    connect(ingredientFilter, &QLineEdit::textChanged, [this](const QString &text){
        for (int i = 0; i < ingredientList->count(); ++i) {
            QListWidgetItem *it = ingredientList->item(i);
            bool match = text.isEmpty() || it->text().contains(text, Qt::CaseInsensitive);
            it->setHidden(!match);
        }
    });

    // Initialize first recipe selection
    if (recipeList->count() > 0) {
        recipeList->setCurrentRow(0);
    }
}

void ShoppingListWidget::onRecipeSelectionChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    Q_UNUSED(previous);
    if (!current) return;
    refreshIngredientsFor(current->text());
}

void ShoppingListWidget::refreshIngredientsFor(const QString &recipeName) {
    ingredientList->clear();
    auto it = recipes.find(recipeName);
    if (it == recipes.end()) return;
    const Recipe &r = it->second;
    for (const auto &ing : r.ingredients) {
        // Display ingredient name (optionally include amount)
        ingredientList->addItem(ing.name);
    }
    // Apply current filter
    QString filter = ingredientFilter->text();
    if (!filter.isEmpty()) {
        for (int i = 0; i < ingredientList->count(); ++i) {
            QListWidgetItem *itItem = ingredientList->item(i);
            bool match = itItem->text().contains(filter, Qt::CaseInsensitive);
            itItem->setHidden(!match);
        }
    }
}

void ShoppingListWidget::onIngredientDoubleClicked(QListWidgetItem *item) {
    if (!item) return;
    SLPriority p = static_cast<SLPriority>(browsePrioritySelector->currentData().toInt());
    addIngredientToShoppingList(item->text(), p);
    statusBar->showMessage("Added ingredient: " + item->text(), 2000);
}

void ShoppingListWidget::addSelectedIngredients() {
    QList<QListWidgetItem*> selected = ingredientList->selectedItems();
    if (selected.isEmpty()) {
        statusBar->showMessage("No ingredients selected", 1500);
        return;
    }
    SLPriority p = static_cast<SLPriority>(browsePrioritySelector->currentData().toInt());
    int added = 0;
    for (QListWidgetItem *it : selected) {
        if (!shoppingItems.contains(it->text())) {
            addIngredientToShoppingList(it->text(), p);
            ++added;
        }
    }
    statusBar->showMessage(QString("Added %1 ingredient(s)").arg(added), 2000);
}

void ShoppingListWidget::addAllIngredients() {
    QListWidgetItem *current = recipeList->currentItem();
    if (!current) return;
    auto it = recipes.find(current->text());
    if (it == recipes.end()) return;
    SLPriority p = static_cast<SLPriority>(browsePrioritySelector->currentData().toInt());

    int added = 0;
    for (const auto &ing : it->second.ingredients) {
        if (!shoppingItems.contains(ing.name)) {
            addIngredientToShoppingList(ing.name, p);
            ++added;
        }
    }
    statusBar->showMessage(QString("Added %1 ingredient(s) from %2")
                           .arg(added).arg(current->text()), 2000);
}

void ShoppingListWidget::addCustomItem() {
    QString name = customItemEdit->text().trimmed();
    if (name.isEmpty()) return;
    if (shoppingItems.contains(name)) {
        statusBar->showMessage("Item already in shopping list", 1500);
        return;
    }
    SLPriority p = static_cast<SLPriority>(prioritySelector->currentData().toInt());
    addIngredientToShoppingList(name, p);
    customItemEdit->clear();
    statusBar->showMessage("Custom item added: " + name, 2000);
}

void ShoppingListWidget::addIngredientToShoppingList(const QString &ingredientName, SLPriority p) {
    shoppingItems.insert(ingredientName);
    priorities[ingredientName] = p;
    QListWidgetItem *item = new QListWidgetItem(ingredientName);
    shoppingList->addItem(item);
    updateItemVisual(item);
}

void ShoppingListWidget::showItemContextMenu(const QPoint &pos) {
    QListWidgetItem *item = shoppingList->itemAt(pos);
    if (!item) return;
    QMenu menu(this);
    QAction *lowAct = menu.addAction("Low Priority");
    QAction *medAct = menu.addAction("Medium Priority");
    QAction *highAct = menu.addAction("High Priority");

    QAction *chosen = menu.exec(shoppingList->viewport()->mapToGlobal(pos));
    if (!chosen) return;
    if (chosen == lowAct) setPriority(item, SL_LOW);
    else if (chosen == medAct) setPriority(item, SL_MEDIUM);
    else if (chosen == highAct) setPriority(item, SL_HIGH);
}

void ShoppingListWidget::setPriority(QListWidgetItem *item, SLPriority p) {
    priorities[item->text()] = p;
    updateItemVisual(item);
    statusBar->showMessage("Priority updated: " + item->text(), 1500);
}

void ShoppingListWidget::updateItemVisual(QListWidgetItem *item) {
    QString name = item->text();
    if (!priorities.contains(name)) return;
    switch (priorities[name]) {
        case SL_HIGH:
            item->setForeground(Qt::red);
            item->setIcon(QIcon::fromTheme("emblem-important"));
            break;
        case SL_MEDIUM:
            item->setForeground(QColor(255,165,0));
            item->setIcon(QIcon());
            break;
        case SL_LOW:
        default:
            item->setForeground(Qt::black);
            item->setIcon(QIcon());
            break;
    }
}