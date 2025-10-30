#ifndef RRSEARCHWINDOW_H
#define RRSEARCHWINDOW_H

#include <QLabel>
#include <QTextEdit>
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class RRSearchWindow; }
QT_END_NAMESPACE

struct RRecipe {
    QString name;
    QString ingredients;
    QString steps;
    int time;
    QString difficulty;
    QString tags;
    QString region; // For filtering
    QString tier;   // For filtering
};

class RRSearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RRSearchWindow(QWidget *parent = nullptr);
    ~RRSearchWindow();

private slots:
    void loadRecipes();
    void updateFilter();
    void resetFilters();
    void showRecipeDetails(QListWidgetItem *item);

private:
    void setupUI();
    Ui::RRSearchWindow *ui;

    QList<RRecipe> allRecipes;

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

    void refreshDisplay(const QList<RRecipe>& recipes);
    void populateFilterBoxes();
};

#endif // RRSEARCHWINDOW_H
