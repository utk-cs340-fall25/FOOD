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
#include <QSet>
#include <QScrollArea>

RRSearchWindow::RRSearchWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
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
    searchBar->setPlaceholderText("Search recipes by name or tags...");
    QPushButton *searchButton = new QPushButton("Search");
    searchLayout->addWidget(searchBar);
    searchLayout->addWidget(searchButton);
    
    // Filter controls
    QHBoxLayout *filterLayout = new QHBoxLayout();
    tagBox = new QComboBox();
    tagBox->addItem("All Tags");
    resetButton = new QPushButton("Reset Filters");
    filterLayout->addWidget(new QLabel("Filter by Tag:"));
    filterLayout->addWidget(tagBox);
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
    
    // Create scroll area for details
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    
    detailsTitle = new QLabel("Select a recipe to view details");
    detailsTitle->setWordWrap(true);
    detailsTitle->setStyleSheet("font-weight: bold; font-size: 14px;");
    scrollLayout->addWidget(detailsTitle);
    
    scrollLayout->addWidget(new QLabel("Ingredients:"));
    detailsIngredients = new QTextEdit();
    detailsIngredients->setReadOnly(true);
    detailsIngredients->setMaximumHeight(120);
    scrollLayout->addWidget(detailsIngredients);
    
    scrollLayout->addWidget(new QLabel("Instructions:"));
    detailsInstructions = new QTextEdit();
    detailsInstructions->setReadOnly(true);
    detailsInstructions->setMinimumHeight(150);
    scrollLayout->addWidget(detailsInstructions);
    
    scrollLayout->addWidget(new QLabel("Tags:"));
    detailsTags = new QTextEdit();
    detailsTags->setReadOnly(true);
    detailsTags->setMaximumHeight(80);
    scrollLayout->addWidget(detailsTags);
    
    scrollLayout->addStretch();
    scrollArea->setWidget(scrollWidget);
    rightLayout->addWidget(scrollArea);
    
    contentLayout->addLayout(leftLayout, 1);
    contentLayout->addLayout(rightLayout, 1);
    
    // Add all layouts to main layout
    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(filterLayout);
    mainLayout->addLayout(contentLayout);
    
    // Connect signals
    connect(searchButton, &QPushButton::clicked, this, &RRSearchWindow::updateFilter);
    connect(searchBar, &QLineEdit::textChanged, this, &RRSearchWindow::updateFilter);
    connect(tagBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RRSearchWindow::updateFilter);
    connect(resetButton, &QPushButton::clicked, this, &RRSearchWindow::resetFilters);
    connect(recipeList, &QListWidget::itemClicked, this, &RRSearchWindow::showRecipeDetails);
}

QString RRSearchWindow::formatIngredients(const std::vector<Ingredient>& ingredients) const
{
    if (ingredients.empty()) {
        return "No ingredients listed.";
    }
    
    QString result;
    for (const auto& ingredient : ingredients) {
        result += ingredient.amount_s + " " + ingredient.name + "\n";
    }
    return result;
}

QString RRSearchWindow::formatInstructions(const std::vector<QString>& instructions) const
{
    if (instructions.empty()) {
        return "No instructions listed.";
    }
    
    QString result;
    int stepNum = 1;
    for (const auto& instruction : instructions) {
        result += QString::number(stepNum) + ". " + instruction + "\n";
        stepNum++;
    }
    return result;
}

QString RRSearchWindow::formatTags(const std::vector<QString>& tags) const
{
    if (tags.empty()) {
        return "No tags.";
    }
    
    QString result;
    for (int i = 0; i < tags.size(); ++i) {
        result += tags[i];
        if (i < tags.size() - 1) {
            result += ", ";
        }
    }
    return result;
}

void RRSearchWindow::loadRecipes()
{
    // This function is kept for compatibility but recipes are set via setRecipes()
}

void RRSearchWindow::setRecipes(const std::map<QString, Recipe> &recipeMap)
{
    allRecipes = recipeMap;
    recipeNames.clear();
    for (const auto& entry : allRecipes) {
        recipeNames.append(entry.first);
    }
    populateFilterBoxes();
    refreshDisplay(recipeNames);
}

void RRSearchWindow::loadFromPairs(const std::vector<std::pair<std::string, std::string>> &pairs)
{
    // This function converts pairs into Recipe objects
    // Note: pairs only contain name and ingredients string, so other fields will be empty
    std::map<QString, Recipe> loaded;
    
    for (const auto &p : pairs) {
        Recipe recipe;
        recipe.name = QString::fromStdString(p.first);
        
        // Parse ingredients string into Ingredient vector
        QString ingredientsStr = QString::fromStdString(p.second);
        QStringList ingredientLines = ingredientsStr.split(";", Qt::SkipEmptyParts);
        for (const auto& line : ingredientLines) {
            QStringList parts = line.split(",");
            if (parts.size() >= 2) {
                Ingredient ing;
                ing.name = parts[0].trimmed();
                ing.amount_s = parts[1].trimmed();
                recipe.ingredients.push_back(ing);
            }
        }
        
        loaded[recipe.name] = recipe;
    }
    
    setRecipes(loaded);
}

void RRSearchWindow::updateFilter()
{
    QString searchText = searchBar->text().toLower();
    QString selectedTag = tagBox->currentText();
    
    QStringList filteredRecipes;
    
    for (const auto& entry : allRecipes) {
        const Recipe& recipe = entry.second;
        
        // Check if recipe name matches search
        bool matchesSearch = searchText.isEmpty() || 
                           recipe.name.toLower().contains(searchText);
        
        // Check if recipe contains tag (if a specific tag is selected)
        bool matchesTag = selectedTag == "All Tags";
        if (!matchesTag) {
            for (const auto& tag : recipe.tags) {
                if (tag.toLower() == selectedTag.toLower()) {
                    matchesTag = true;
                    break;
                }
            }
        }
        
        // Also search in ingredients and tags
        if (!matchesSearch) {
            for (const auto& ingredient : recipe.ingredients) {
                if (ingredient.name.toLower().contains(searchText)) {
                    matchesSearch = true;
                    break;
                }
            }
        }
        
        if (!matchesSearch) {
            for (const auto& tag : recipe.tags) {
                if (tag.toLower().contains(searchText)) {
                    matchesSearch = true;
                    break;
                }
            }
        }
        
        if (matchesSearch && matchesTag) {
            filteredRecipes.append(recipe.name);
        }
    }
    
    refreshDisplay(filteredRecipes);
}

void RRSearchWindow::resetFilters()
{
    searchBar->clear();
    tagBox->setCurrentIndex(0);
    refreshDisplay(recipeNames);
}

void RRSearchWindow::showRecipeDetails(QListWidgetItem *item)
{
    if (!item) return;
    
    QString recipeName = item->text();
    
    // Find the recipe in the map
    auto it = allRecipes.find(recipeName);
    if (it != allRecipes.end()) {
        const Recipe& recipe = it->second;
        
        detailsTitle->setText(recipe.name);
        detailsIngredients->setText(formatIngredients(recipe.ingredients));
        detailsInstructions->setText(formatInstructions(recipe.instructions));
        detailsTags->setText(formatTags(recipe.tags));
    }
}

void RRSearchWindow::refreshDisplay(const QStringList& recipeNameList)
{
    recipeList->clear();
    for (const QString &recipeName : recipeNameList) {
        recipeList->addItem(recipeName);
    }
}

void RRSearchWindow::populateFilterBoxes()
{
    QSet<QString> tags;
    
    // Collect all unique tags from all recipes
    for (const auto& entry : allRecipes) {
        const Recipe& recipe = entry.second;
        for (const auto& tag : recipe.tags) {
            tags.insert(tag);
        }
    }
    
    // Populate tag combo box
    tagBox->blockSignals(true);
    int currentIndex = tagBox->currentIndex();
    tagBox->clear();
    tagBox->addItem("All Tags");
    for (const QString &tag : tags) {
        tagBox->addItem(tag);
    }
    tagBox->setCurrentIndex(0);
    tagBox->blockSignals(false);
}
