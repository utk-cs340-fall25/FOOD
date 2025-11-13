#include "provider_adapter.h"
#include <QString>
#include <QDebug>

// Include the teammate's header-based API (use the repository headers)
#include "../../headers/food.hpp"

// Keep the raw provider maps here so DEINIT can be called later.
static std::map<std::string, Recipe> s_raw_recipes;
static std::map<std::string, Ingredient> s_raw_ingredients;

bool load_header_recipes(QList<RRecipe> &out)
{
    // Initialize provider maps
    STATUS st = INIT(s_raw_recipes, s_raw_ingredients);
    if (st != STATUS_SUCCESS) {
        return false;
    }

    out.clear();
    for (const auto &p : s_raw_recipes) {
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

bool load_header_provider(QList<RRecipe> &out, std::map<QString, bool> &outIngredients)
{
    outIngredients.clear();
    if (!load_header_recipes(out)) return false;

    // Populate ingredients map (default owned=false; if amount_d>0 set true)
    for (const auto &p : s_raw_ingredients) {
        QString name = QString::fromStdString(p.first);
        bool owned = false;
        try {
            if (p.second.amount_d > 0.0) owned = true;
        } catch (...) { owned = false; }
        outIngredients[name] = owned;
    }
    return true;
}

bool provider_deinit()
{
    // Call the provider DEINIT to persist any changes
    STATUS st = DEINIT(s_raw_recipes, s_raw_ingredients);
    // clear caches regardless
    s_raw_recipes.clear();
    s_raw_ingredients.clear();
    return (st == STATUS_SUCCESS);
}
