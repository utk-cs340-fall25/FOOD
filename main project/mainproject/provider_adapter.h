#pragma once

#include <QList>
#include "Rrsearchwindow.h"

// Loads recipes using the team's header-based file API (headers/food.hpp)
// and converts them into the UI-facing RRecipe list.
bool load_header_recipes(QList<RRecipe> &out);
