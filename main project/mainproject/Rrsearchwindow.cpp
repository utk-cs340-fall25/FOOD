#include "Rrsearchwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>

RRSearchWindow::RRSearchWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadRecipes();
}

RRSearchWindow::~RRSearchWindow()
{
}

void RRSearchWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Search bar
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Search recipes...");
    QPushButton *searchButton = new QPushButton("Search");
    searchLayout->addWidget(searchBar);
    searchLayout->addWidget(searchButton);
    
    // Filter controls
    QHBoxLayout *filterLayout = new QHBoxLayout();
    regionBox = new QComboBox();
    regionBox->addItem("All Regions");
    tierBox = new QComboBox();
    tierBox->addItem("All Tiers");
    resetButton = new QPushButton("Reset Filters");
    filterLayout->addWidget(new QLabel("Region:"));
    filterLayout->addWidget(regionBox);
    filterLayout->addWidget(new QLabel("Tier:"));
    filterLayout->addWidget(tierBox);
    filterLayout->addWidget(resetButton);
    filterLayout->addStretch();
    
    // Main content area
    QHBoxLayout *contentLayout = new QHBoxLayout();
    
    // Left side - Recipe list
    QVBoxLayout *leftLayout = new QVBoxLayout();
    recipeList = new QListWidget();
    leftLayout->addWidget(new QLabel("Recipes:"));
    leftLayout->addWidget(recipeList);
    
    // Right side - Details panel
    QVBoxLayout *rightLayout = new QVBoxLayout();
    detailsTitle = new QLabel("Select a recipe to view details");
    detailsTitle->setWordWrap(true);
    detailsRegion = new QLabel();
    detailsTier = new QLabel();
    detailsIngredients = new QTextEdit();
    detailsIngredients->setReadOnly(true);
    detailsPanel = new QTextEdit();
    detailsPanel->setReadOnly(true);
    
    rightLayout->addWidget(detailsTitle);
    rightLayout->addWidget(detailsRegion);
    rightLayout->addWidget(detailsTier);
    rightLayout->addWidget(new QLabel("Ingredients:"));
    rightLayout->addWidget(detailsIngredients);
    rightLayout->addWidget(new QLabel("Steps:"));
    rightLayout->addWidget(detailsPanel);
    
    contentLayout->addLayout(leftLayout, 1);
    contentLayout->addLayout(rightLayout, 1);
    
    // Add all layouts to main layout
    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(filterLayout);
    mainLayout->addLayout(contentLayout);
    
    // Connect signals
    connect(searchButton, &QPushButton::clicked, this, &RRSearchWindow::updateFilter);
    connect(searchBar, &QLineEdit::textChanged, this, &RRSearchWindow::updateFilter);
    connect(regionBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RRSearchWindow::updateFilter);
    connect(tierBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RRSearchWindow::updateFilter);
    connect(resetButton, &QPushButton::clicked, this, &RRSearchWindow::resetFilters);
    connect(recipeList, &QListWidget::itemClicked, this, &RRSearchWindow::showRecipeDetails);
}

void RRSearchWindow::loadRecipes()
{
    QFile file("recipes.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open recipes.txt");
        return;
    }
    
    QTextStream in(&file);
    QString currentRecipe = "";
    RRecipe recipe;
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        
        if (line.isEmpty()) {
            if (!currentRecipe.isEmpty()) {
                allRecipes.append(recipe);
                currentRecipe = "";
            }
            continue;
        }
        
        if (line.startsWith("Name:")) {
            recipe.name = line.mid(5).trimmed();
        } else if (line.startsWith("Ingredients:")) {
            recipe.ingredients = line.mid(12).trimmed();
        } else if (line.startsWith("Steps:")) {
            recipe.steps = line.mid(6).trimmed();
        } else if (line.startsWith("Time:")) {
            recipe.time = line.mid(5).trimmed().toInt();
        } else if (line.startsWith("Difficulty:")) {
            recipe.difficulty = line.mid(11).trimmed();
        } else if (line.startsWith("Tags:")) {
            recipe.tags = line.mid(5).trimmed();
        } else if (line.startsWith("Region:")) {
            recipe.region = line.mid(7).trimmed();
        } else if (line.startsWith("Tier:")) {
            recipe.tier = line.mid(5).trimmed();
        }
    }
    
    if (!currentRecipe.isEmpty()) {
        allRecipes.append(recipe);
    }
    
    file.close();
    
    populateFilterBoxes();
    refreshDisplay(allRecipes);
}

void RRSearchWindow::updateFilter()
{
    QString searchText = searchBar->text().toLower();
    QString selectedRegion = regionBox->currentText();
    QString selectedTier = tierBox->currentText();
    
    QList<RRecipe> filteredRecipes;
    
    for (const RRecipe &recipe : allRecipes) {
        bool matchesSearch = searchText.isEmpty() || 
                           recipe.name.toLower().contains(searchText) ||
                           recipe.ingredients.toLower().contains(searchText) ||
                           recipe.tags.toLower().contains(searchText);
        
        bool matchesRegion = selectedRegion == "All Regions" || recipe.region == selectedRegion;
        bool matchesTier = selectedTier == "All Tiers" || recipe.tier == selectedTier;
        
        if (matchesSearch && matchesRegion && matchesTier) {
            filteredRecipes.append(recipe);
        }
    }
    
    refreshDisplay(filteredRecipes);
}

void RRSearchWindow::resetFilters()
{
    searchBar->clear();
    regionBox->setCurrentIndex(0);
    tierBox->setCurrentIndex(0);
    refreshDisplay(allRecipes);
}

void RRSearchWindow::showRecipeDetails(QListWidgetItem *item)
{
    if (!item) return;
    
    QString recipeName = item->text();
    RRecipe *recipe = nullptr;
    
    for (RRecipe &r : allRecipes) {
        if (r.name == recipeName) {
            recipe = &r;
            break;
        }
    }
    
    if (recipe) {
        detailsTitle->setText(recipe->name);
        detailsRegion->setText("Region: " + recipe->region);
        detailsTier->setText("Tier: " + recipe->tier);
        detailsIngredients->setText(recipe->ingredients);
        detailsPanel->setText(recipe->steps);
    }
}

void RRSearchWindow::refreshDisplay(const QList<RRecipe>& recipes)
{
    recipeList->clear();
    for (const RRecipe &recipe : recipes) {
        recipeList->addItem(recipe.name);
    }
}

void RRSearchWindow::populateFilterBoxes()
{
    QSet<QString> regions, tiers;
    
    for (const RRecipe &recipe : allRecipes) {
        if (!recipe.region.isEmpty()) {
            regions.insert(recipe.region);
        }
        if (!recipe.tier.isEmpty()) {
            tiers.insert(recipe.tier);
        }
    }
    
    regionBox->clear();
    regionBox->addItem("All Regions");
    for (const QString &region : regions) {
        regionBox->addItem(region);
    }
    
    tierBox->clear();
    tierBox->addItem("All Tiers");
    for (const QString &tier : tiers) {
        tierBox->addItem(tier);
    }
}
