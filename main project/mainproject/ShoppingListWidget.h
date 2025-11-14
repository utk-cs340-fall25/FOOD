#ifndef SHOPPINGLISTWIDGET_H
#define SHOPPINGLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QMap>
#include <QSet>
#include <QDateTime>
#include <QIcon>

#include "food.h"   // For Recipe / Ingredient

enum SLPriority { SL_LOW, SL_MEDIUM, SL_HIGH };

class ShoppingListWidget : public QWidget {
    Q_OBJECT
public:
    explicit ShoppingListWidget(const std::map<QString, Recipe>& recipesRef,
                                QWidget *parent = nullptr);

private slots:
    void onRecipeSelectionChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void onIngredientDoubleClicked(QListWidgetItem *item);
    void addSelectedIngredients();
    void addAllIngredients();
    void addCustomItem();
    void showItemContextMenu(const QPoint &pos);
    void setPriority(QListWidgetItem *item, SLPriority p);

private:
    void refreshIngredientsFor(const QString &recipeName);
    void addIngredientToShoppingList(const QString &ingredientName, SLPriority p);
    void updateItemVisual(QListWidgetItem *item);

    // Data source (not owned)
    const std::map<QString, Recipe>& recipes;

    // Widgets
    QListWidget *recipeList;
    QListWidget *ingredientList;
    QListWidget *shoppingList;
    QLineEdit   *ingredientFilter;
    QLineEdit   *customItemEdit;
    QComboBox   *prioritySelector;
    QComboBox   *browsePrioritySelector;
    QPushButton *addSelectedButton;
    QPushButton *addAllButton;
    QPushButton *addCustomButton;
    QStatusBar  *statusBar;

    // Tracking sets
    QSet<QString> shoppingItems;          // Ingredient/custom names present in shopping list
    QMap<QString, SLPriority> priorities; // Per-item priority
};

#endif // SHOPPINGLISTWIDGET_H