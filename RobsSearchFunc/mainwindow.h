#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QTextEdit>
#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadRecipes();
    void updateFilter();
    void resetFilters();
    void showRecipeDetails(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;

    // Data
    QList<Recipe> allRecipes;

    // UI elements
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

#endif // MAINWINDOW_H
