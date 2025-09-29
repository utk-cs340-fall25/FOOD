#include "file_manager.hpp"
#include "../error_handlers/error.hpp"

#include <iostream>
#include <cstdint>
#include <cctype>
#include <string>
#include <filesystem>
#include <stdexcept>

int File_Manager::Write_Recipe(const std::string path, const struct Recipe &output_recipe)
{
    if (path.substr(path.length() - 4, 4) != ".rcp") return ERROR_BAD_EXTENSION;
    
    file_output.open(path);
    if (!file_output.is_open())
    {
        if (!std::filesystem::exists(path)) return ERROR_FILE_NOT_FOUND;
        else return ERROR_OPEN_FAILED;
    }

    if (output_recipe.name == "") return ERROR_INVALID_DATA;
    output_buffer.push_back(to_lower(output_recipe.name) + "\n\n");

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