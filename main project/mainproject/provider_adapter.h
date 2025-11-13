#pragma once

#include <QList>
#include <map>
#include <QString>
#include "Rrsearchwindow.h"

// Loads recipes using the team's header-based file API (headers/food.hpp)
// and converts them into the UI-facing RRecipe list.
// The adapter keeps the raw maps loaded so DEINIT can be called via provider_deinit().
bool load_header_recipes(QList<RRecipe> &out);

// Loads recipes and ingredients into UI-friendly structures.
// `outIngredients` is filled with ingredient name -> owned (bool). Owned defaults to false.
bool load_header_provider(QList<RRecipe> &out, std::map<QString, bool> &outIngredients);

// Call to persist and cleanup provider-loaded data (wraps DEINIT).
bool provider_deinit();
