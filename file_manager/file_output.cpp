#include "file_manager.hpp"
#include <iostream>
#include <cstdint>
#include <cctype>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <winerror.h>

int File_Manager::Write_Recipe(const std::string path, const struct Recipe &output_recipe)
{
    std::filesystem::path out_path(path);
    if (std::filesystem::exists(out_path)) return ERROR_FILE_EXISTS;

    file_output.open(path);
    if (!file_output.is_open()) return ERROR_OPEN_FAILED;

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