#include "file_manager.hpp"
#include <iostream>
#include <cstdint>
#include <cctype>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <winerror.h>

std::string to_lower(std::string string)
{
    std::string output = string;

    for (uint64_t i = 0; i < string.length(); i += 1)
    {
        output.at(i) = (char)tolower((char)output.at(i));
    }

    return output;
};

int File_Manager::Read_Recipe(const std::string path, struct Recipe &output_recipe)
{
    file_input.open(path);
    if (!file_input.is_open()) return ERROR_OPEN_FAILED;

    // File Reading
    struct Recipe recipe;
    std::string temp_string;

    file_input >> recipe.name;

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

int File_Manager::Write_Recipe(const std::string path, const struct Recipe &output_recipe)
{
    std::filesystem::path out_path(path);
    if (std::filesystem::exists(out_path)) return ERROR_FILE_EXISTS;

    file_output.open(path);
    if (!file_output.is_open()) return ERROR_OPEN_FAILED;

    if (output_recipe.name == "") return ERROR_INVALID_DATA;
    output_buffer.push_back(output_recipe.name + "\n\n");

    for (uint64_t i = 0; i < output_recipe.ingredients.size(); i += 1)
    {
        if (output_recipe.ingredients.at(i).amount_s == "") return ERROR_INVALID_DATA;
        if (output_recipe.ingredients.at(i).unit == "") return ERROR_INVALID_DATA;

        output_buffer.push_back(output_recipe.ingredients.at(i).amount_s + ' ' + 
                                output_recipe.ingredients.at(i).unit + '\n');
    }

    for (uint64_t i = 0; i < output_recipe.instructions.size(); i += 1)
    {
        if (output_recipe.instructions.at(i) == "") return ERROR_INVALID_DATA;
        output_buffer.push_back(output_recipe.instructions.at(i) + '\n');
    }

    for (uint64_t i = 0; i < output_buffer.size(); i += 1)
    {
        file_output << output_buffer.front();
        output_buffer.pop_front();
    }

    Reset();
    return ERROR_SUCCESS;
}