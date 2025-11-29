#ifndef RRSEARCHWINDOW_H
#define RRSEARCHWINDOW_H

#include <QLabel>
#include <QCheckBox>
#include <QSet>
#include <QTextEdit>
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QList>
#include <QScrollArea>
#include <vector>
#include <utility>
#include <map>
#include "food.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RRSearchWindow; }
QT_END_NAMESPACE

class RRSearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RRSearchWindow(QWidget *parent = nullptr);
    ~RRSearchWindow();

    // Accept upstream data from Recipe map and refresh UI
    void setRecipes(const std::map<QString, Recipe> &recipeMap);
    void loadFromPairs(const std::vector<std::pair<std::string, std::string>> &nameAndIngredients);
    // Accept a set of ingredient names that the user has on-hand
    void setOwnedIngredients(const QSet<QString> &owned);
    // For tests: return the currently displayed recipe names
    QStringList visibleRecipes() const;

private slots:
    void loadRecipes();
    void updateFilter();
    void resetFilters();
    void showRecipeDetails(QListWidgetItem *item);

private:
    void setupUI();
    Ui::RRSearchWindow *ui;

    std::map<QString, Recipe> allRecipes;
    QStringList recipeNames; // Cache for recipe names for quick lookup

    QListWidget* recipeList;
    QTextEdit* detailsPanel;
    QLineEdit* searchBar;
    QComboBox* tagBox;
    QPushButton* resetButton;
    QCheckBox* ownedFilterCheckBox; // toggle: only show recipes that can be made with owned ingredients

    QLabel *detailsTitle;
    QTextEdit *detailsIngredients;
    QTextEdit *detailsInstructions;
    QTextEdit *detailsTags;

    void refreshDisplay(const QStringList& recipeNameList);
    void populateFilterBoxes();
    QSet<QString> ownedIngredients; // ingredients the user has (names, case-insensitive)
    QString formatIngredients(const std::vector<Ingredient>& ingredients) const;
    QString formatInstructions(const std::vector<QString>& instructions) const;
    QString formatTags(const std::vector<QString>& tags) const;
};

#endif // RRSEARCHWINDOW_H
