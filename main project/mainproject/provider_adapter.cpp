#include "provider_adapter.h"
#include <QString>
#include <QDebug>

// Include the teammate's header-based API (use the repository headers)
#include "../../headers/food.hpp"

bool load_header_recipes(QList<RRecipe> &out)
{
    std::map<std::string, Recipe> raw_recipes;
    std::map<std::string, Ingredient> raw_ingredients;

    STATUS st = INIT(raw_recipes, raw_ingredients);
    if (st != STATUS_SUCCESS) {
        return false;
    }

    for (const auto &p : raw_recipes) {
        const Recipe &rcp = p.second;
        RRecipe r;
        r.name = QString::fromStdString(rcp.name);

        QString ingredientsStr;
        for (const auto &ing : rcp.ingredients) {
            ingredientsStr += QString::fromStdString(ing.amount_s);
            if (!ing.unit.empty()) ingredientsStr += " " + QString::fromStdString(ing.unit);
            ingredientsStr += " " + QString::fromStdString(ing.name) + "\n";
        }
        r.ingredients = ingredientsStr;

        QString stepsStr;
        for (const auto &s : rcp.instructions) {
            stepsStr += QString::fromStdString(s) + "\n";
        }
        r.steps = stepsStr;

        QString tagsStr;
        for (const auto &t : rcp.tags) tagsStr += QString::fromStdString(t) + " ";
        r.tags = tagsStr.trimmed();

        r.time = 0;
        r.difficulty = "";
        r.region = "";
        r.tier = "";

        out.append(r);
    }

    return true;
}
