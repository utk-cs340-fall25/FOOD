#ifndef RECIPEPARSER_H
#define RECIPEPARSER_H

#include <QVector>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// The existing Recipe struct from main.cpp
struct Recipe {
    QString name;
    QStringList ingredients;
};

// Function to load recipes from .rcp files in a directory
void loadRecipes(const QString &path, QVector<Recipe> &recipes) {
    QDir recipeDir(path);
    if (!recipeDir.exists()) {
        qWarning() << "Recipe directory not found:" << path;
        return;
    }

    QStringList filter;
    filter << "*.rcp";
    QFileInfoList fileList = recipeDir.entryInfoList(filter, QDir::Files);

    for (const QFileInfo &fileInfo : fileList) {
        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open recipe file:" << fileInfo.fileName();
            continue;
        }

        QTextStream in(&file);
        Recipe currentRecipe;
        currentRecipe.name = in.readLine();

        QString line;
        bool readingIngredients = true;
        while (!in.atEnd()) {
            line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            if (line.toLower() == "instructions") {
                readingIngredients = false;
                // We are not using instructions for now, but we can skip them
                while(!in.atEnd() && in.readLine().toLower() != "tags"){} // skip to tags
                continue;
            }

            if (readingIngredients) {
                // The format is measurement then ingredient name. We only need the name.
                QString ingredientName = in.readLine();
                if(!ingredientName.isNull()){
                    currentRecipe.ingredients.append(ingredientName);
                }
            }
        }
        if (!currentRecipe.name.isEmpty()) {
            recipes.append(currentRecipe);
        }
        file.close();
    }
}

#endif // RECIPEPARSER_H