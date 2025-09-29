#include "file_manager.hpp"
#include "../error_handlers/error.hpp"

#include <iostream>
#include <cstdint>
#include <cctype>
#include <string>
#include <filesystem>
#include <stdexcept>

int File_Manager::Read_Recipe(const std::string path, struct Recipe &output_recipe)
{
    if (path.substr(path.length() - 4, 4) != ".rcp") return ERROR_BAD_EXTENSION;
    
    file_input.open(path);
    if (!file_input.is_open())
    {
        if (!std::filesystem::exists(path)) return ERROR_FILE_NOT_FOUND;
        else return ERROR_OPEN_FAILED;
    }

    // File Reading
    struct Recipe recipe;
    std::string temp_string;

    file_input >> recipe.name;
    recipe.name = to_lower(recipe.name);

    while (to_lower(temp_string) != "instructions")
    {
        std::string::size_type sz;
        struct Ingredient temp_ingredient;
        file_input >> temp_ingredient.amount_s >> temp_ingredient.unit >> temp_ingredient.name;
        temp_ingredient.name = to_lower(temp_ingredient.name);
        try
        {
            temp_ingredient.amount_d = std::stod(temp_string, &sz);
        }
        catch(std::invalid_argument) {return ERROR_INVALID_DATA;}

        recipe.ingredients.push_back(temp_ingredient);
    }

    while (std::getline(file_input, temp_string, '\n'))
    {
        recipe.instructions.push_back(temp_string);
    }

    output_recipe = recipe;
    Reset();
    return ERROR_SUCCESS;
}