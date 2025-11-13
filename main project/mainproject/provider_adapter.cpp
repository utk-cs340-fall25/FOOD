#include "provider_adapter.h"
#include <QString>
#include <QDebug>

// Use the mainproject's QString-based API to avoid duplicate definitions.
#include "food.h"

// Keep the raw provider maps here so DEINIT can be called later.
static std::map<QString, Recipe> s_raw_recipes;
static std::map<QString, bool> s_raw_ingredients;

bool load_header_recipes(QList<RRecipe> &out)
{
    // Initialize provider maps (this calls the existing INIT in this project)
    STATUS st = INIT(s_raw_recipes, s_raw_ingredients);
    if (st != STATUS_SUCCESS) {
        return false;
    }

    out.clear();
    for (const auto &p : s_raw_recipes) {
        const Recipe &rcp = p.second;
        RRecipe r;
        r.name = rcp.name;

        QString ingredientsStr;
        for (const auto &ing : rcp.ingredients) {
            ingredientsStr += ing.amount_s;
            if (!ing.name.isEmpty()) ingredientsStr += " " + ing.name;
            ingredientsStr += "\n";
        }
        r.ingredients = ingredientsStr;

        QString stepsStr;
        for (const auto &s : rcp.instructions) {
            stepsStr += s + "\n";
        }
        r.steps = stepsStr;

        QString tagsStr;
        for (const auto &t : rcp.tags) tagsStr += t + " ";
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

    // Populate ingredients map using loaded s_raw_ingredients
    for (const auto &p : s_raw_ingredients) {
        outIngredients[p.first] = p.second;
    }
    return true;
}

bool provider_deinit()
{
    STATUS st = DEINIT(s_raw_recipes, s_raw_ingredients);
    s_raw_recipes.clear();
    s_raw_ingredients.clear();
    return (st == STATUS_SUCCESS);
}
