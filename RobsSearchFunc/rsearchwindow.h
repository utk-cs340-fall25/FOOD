#ifndef RSEARCHWINDOW_H
#define RSEARCHWINDOW_H

#include <QLabel>
#include <QTextEdit>
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class RSearchWindow; }
QT_END_NAMESPACE

struct Recipe {
    QString name;
    QString ingredients;
    QString steps;
    int time;
    QString difficulty;
    QString tags;
    QString region; // For filtering
    QString tier;   // For filtering
};

class RSearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RSearchWindow(QWidget *parent = nullptr);
    ~RSearchWindow();

private slots:
    void loadRecipes();
    void updateFilter();
    void resetFilters();
    void showRecipeDetails(QListWidgetItem *item);

private:
    Ui::RSearchWindow *ui;

    QList<Recipe> allRecipes;

    QListWidget *ingredientListWidget;
    QListWidget* recipeList;
    QTextEdit* detailsPanel;
    QLineEdit* searchBar;
    QLineEdit* ingredientBar;
    QComboBox* regionBox;
    QComboBox* tierBox;
    QPushButton* resetButton;

    QLabel *detailsTitle;
    QLabel *detailsRegion;
    QLabel *detailsTier;
    QTextEdit *detailsIngredients;

    void refreshDisplay(const QList<Recipe>& recipes);
    void populateFilterBoxes();
};

#endif // RSEARCHWINDOW_H
