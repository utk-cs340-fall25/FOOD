#include "file_manager.hpp"
#include "error.hpp"

#include <iostream>
#include <cstdint>
#include <cctype>
#include <string>
#include <filesystem>
#include <stdexcept>

int File_Manager::Write_Recipe(const std::string path, const struct Recipe& output_recipe, bool overwrite)
{
    if (path.substr(path.length() - 4, 4) != ".rcp") return STATUS_BAD_EXTENSION;
    if (std::filesystem::exists(path) && !overwrite) return STATUS_FILE_EXISTS;

    file_output.open(path);
    if (!file_output.is_open())
    {
        if (!std::filesystem::exists(path)) return STATUS_FILE_NOT_FOUND;
        else return STATUS_OPEN_FAILED;
    }

    if (output_recipe.name == "") return STATUS_INVALID_DATA;
    output_buffer.push_back(to_lower(output_recipe.name) + "\n\n");

    for (uint64_t i = 0; i < output_recipe.ingredients.size(); i += 1)
    {
        if (output_recipe.ingredients.at(i).amount_s == "") return STATUS_INVALID_DATA;
        if (output_recipe.ingredients.at(i).unit == "") return STATUS_INVALID_DATA;

        output_buffer.push_back(output_recipe.ingredients.at(i).amount_s + ' ' +
            output_recipe.ingredients.at(i).unit + ' ' +
            output_recipe.ingredients.at(i).name + '\n');
    }

    output_buffer.push_back("\n");
    output_buffer.push_back("instructions\n");

    for (uint64_t i = 0; i < output_recipe.instructions.size(); i += 1)
    {
        if (output_recipe.instructions.at(i) == "") return STATUS_INVALID_DATA;
        output_buffer.push_back(output_recipe.instructions.at(i) + '\n');
    }

    for (uint64_t i = 0; i < output_buffer.size(); i += 1)
    {
        file_output << output_buffer.front();
        output_buffer.pop_front();
    }

    Reset();
    return STATUS_SUCCESS;
}