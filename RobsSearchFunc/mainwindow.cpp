#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- Initialize UI elements ---
    // Search bar for name
    searchBar = new QLineEdit(ui->mainContentWidget);
    searchBar->setPlaceholderText("Search by recipe name...");

    // Region filter
    regionBox = new QComboBox(ui->mainContentWidget);
    regionBox->addItem("All Regions");

    // Tier filter
    tierBox = new QComboBox(ui->mainContentWidget);
    tierBox->addItem("All Tiers");
    tierBox->addItem("1 - Very Light");
    tierBox->addItem("2 - Light");
    tierBox->addItem("3 - Medium");
    tierBox->addItem("4 - Hearty");
    tierBox->addItem("5 - Stuffed/Complex");

    // Recipe list
    recipeList = new QListWidget(ui->mainContentWidget);

    // Details panel
    detailsTitle = new QLabel("Recipe Name", ui->mainContentWidget);
    detailsRegion = new QLabel("Region: ", ui->mainContentWidget);
    detailsTier = new QLabel("Tier: ", ui->mainContentWidget);
    detailsIngredients = new QTextEdit(ui->mainContentWidget);
    detailsIngredients->setReadOnly(true);

    // Layout inside mainContentWidget
    QVBoxLayout* mainLayout = new QVBoxLayout(ui->mainContentWidget);
    QHBoxLayout* topFiltersLayout = new QHBoxLayout();
    topFiltersLayout->addWidget(searchBar);
    topFiltersLayout->addWidget(regionBox);
    topFiltersLayout->addWidget(tierBox);
    mainLayout->addLayout(topFiltersLayout);
    mainLayout->addWidget(recipeList);
    mainLayout->addWidget(detailsTitle);
    mainLayout->addWidget(detailsRegion);
    mainLayout->addWidget(detailsTier);
    mainLayout->addWidget(detailsIngredients);

    // --- Load recipes ---
    loadRecipes();

    // Populate ingredientListWidget
    ingredientListWidget = ui->ingredientListWidget;
    QSet<QString> allIngredients;
    for (const Recipe &r : allRecipes) {
        QStringList ingList = r.ingredients.split(",", Qt::SkipEmptyParts);
        for (QString ing : ingList)
            allIngredients.insert(ing.trimmed());
    }
    QStringList sortedIngredients = allIngredients.values();
    sortedIngredients.sort();
    for (const QString &ing : sortedIngredients) {
        QListWidgetItem *item = new QListWidgetItem(ing, ingredientListWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    // --- Connections ---
    connect(searchBar, &QLineEdit::textChanged, this, &MainWindow::updateFilter);
    connect(regionBox, &QComboBox::currentTextChanged, this, &MainWindow::updateFilter);
    connect(tierBox, &QComboBox::currentTextChanged, this, &MainWindow::updateFilter);
    connect(ingredientListWidget, &QListWidget::itemChanged, this, &MainWindow::updateFilter);
    connect(recipeList, &QListWidget::itemClicked, this, &MainWindow::showRecipeDetails);

    // Initial display
    refreshDisplay(allRecipes);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadRecipes()
{
    QFile file("recipes.txt");
    if (!file.exists()) {
        QMessageBox::warning(this, "File Missing", "recipes.txt not found in project directory.");
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open recipes.txt for reading.");
        return;
    }

    QTextStream in(&file);
    allRecipes.clear();

    // Skip header
    if (!in.atEnd())
        in.readLine();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split("|");
        if (fields.size() < 8) continue;

        Recipe r;
        r.name = fields[0].trimmed();
        r.ingredients = fields[1].trimmed();
        r.region = fields[6].trimmed();
        r.tier = fields[7].trimmed();
        allRecipes.append(r);

        // Add region to regionBox if new
        if (regionBox->findText(r.region) == -1)
            regionBox->addItem(r.region);
    }

    file.close();
    qDebug() << "Loaded" << allRecipes.size() << "recipes.";
}
void MainWindow::resetFilters()
{
    // Example: uncheck all ingredients
    for (int i = 0; i < ingredientListWidget->count(); ++i) {
        ingredientListWidget->item(i)->setCheckState(Qt::Unchecked);
    }

    // Reset text and combo boxes
    searchBar->clear();
    regionBox->setCurrentIndex(0);
    tierBox->setCurrentIndex(0);

    // Refresh the recipe list
    refreshDisplay(allRecipes);
}

void MainWindow::updateFilter()
{
    QString searchText = searchBar->text().trimmed();
    QString selectedRegion = regionBox->currentText();
    QString selectedTier = tierBox->currentText();

    // Collect selected ingredients
    QStringList selectedIngredients;
    for (int i = 0; i < ingredientListWidget->count(); ++i) {
        QListWidgetItem *item = ingredientListWidget->item(i);
        if (item->checkState() == Qt::Checked)
            selectedIngredients.append(item->text());
    }

    QList<Recipe> filtered;
    for (const Recipe &r : allRecipes) {
        // Filter by name
        if (!searchText.isEmpty() && !r.name.contains(searchText, Qt::CaseInsensitive))
            continue;

        // Filter by region
        if (selectedRegion != "All Regions" && r.region != selectedRegion)
            continue;

        // Filter by tier
        if (selectedTier != "All Tiers") {
            if (!r.tier.startsWith(selectedTier.left(1))) continue;
        }

        // Filter by ingredients
        bool include = true;
        for (const QString &ing : selectedIngredients) {
            if (!r.ingredients.contains(ing, Qt::CaseInsensitive)) {
                include = false;
                break;
            }
        }
        if (!include) continue;

        filtered.append(r);
    }

    refreshDisplay(filtered);
}

void MainWindow::refreshDisplay(const QList<Recipe> &recipes)
{
    recipeList->clear();
    for (const Recipe &r : recipes)
        recipeList->addItem(r.name);

    if (!recipes.isEmpty())
        showRecipeDetails(recipeList->item(0));
    else {
        detailsTitle->setText("No recipe selected");
        detailsRegion->setText("");
        detailsTier->setText("");
        detailsIngredients->setText("");
    }
}

void MainWindow::showRecipeDetails(QListWidgetItem *item)
{
    if (!item) return;

    QString name = item->text();
    for (const Recipe &r : allRecipes) {
        if (r.name == name) {
            detailsTitle->setText(r.name);
            detailsRegion->setText("Region: " + r.region);
            detailsTier->setText("Tier: " + r.tier);
            detailsIngredients->setText(r.ingredients);
            break;
        }
    }
}
