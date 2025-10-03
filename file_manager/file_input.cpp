#include "file_manager.hpp"
#include "error.hpp"

#include <iostream>
#include <cstdint>
#include <cctype>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <cctype>

int File_Manager::Read_Recipe(const std::string path, struct Recipe& output_recipe)
{
    Reset();
    if (path.substr(path.length() - 4, 4) != ".rcp") return STATUS_BAD_EXTENSION;

    file_input.open(path);
    if (!file_input.is_open())
    {
        if (!std::filesystem::exists(path)) return STATUS_FILE_NOT_FOUND;
        else return STATUS_OPEN_FAILED;
    }

    // File Reading
    struct Recipe recipe;
    std::string temp_string;

    file_input >> recipe.name;
    recipe.name = to_lower(recipe.name);
    temp_string.resize(1);

    while (to_lower(temp_string) != "i")
    {
        std::string::size_type sz;
        struct Ingredient temp_ingredient;
        file_input >> temp_ingredient.amount_s >> temp_ingredient.unit >> temp_ingredient.name;
        temp_ingredient.name = to_lower(temp_ingredient.name);
        try
        {
            //temp_ingredient.amount_d = std::stod(temp_string, &sz);
        }
        catch (std::invalid_argument) { return STATUS_INVALID_DATA; }

        recipe.ingredients.push_back(temp_ingredient);
        
        char temp_char = file_input.peek();
        while (isspace((int)temp_char))
        {
            file_input >> std::noskipws >> temp_char;
            temp_char = file_input.peek();
        }
        
        temp_string.at(0) = temp_char;
    }

    std::getline(file_input, temp_string, '\n');
    while (std::getline(file_input, temp_string, '\n'))
    {
        if (temp_string.length() == 0) continue;
        recipe.instructions.push_back(temp_string);
    }

    output_recipe = recipe;
    Reset();
    return STATUS_SUCCESS;
}