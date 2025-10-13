#include "../headers/file_manager.hpp"
#include "../headers/food.hpp"

#include <iostream>
#include <cstdint>
#include <cctype>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <cctype>

File_Manager::File_Manager()
{
    Reset();
}
File_Manager::~File_Manager()
{
    Reset();
}
void File_Manager::Reset()
{
    file_input.close();
    file_output.close();
    input_buffer.clear();
    output_buffer.clear();

    buffered_rcp = false;
    rcp_name.clear();

    return;
}

void File_Manager::print_recipe(const struct Recipe& recipe)
{
    output_buffer.push_back(to_lower(recipe.name) + "\n\n");

    for (uint64_t i = 0; i < recipe.ingredients.size(); i += 1)
    {
        output_buffer.push_back(recipe.ingredients.at(i).amount_s + ' ' +
            recipe.ingredients.at(i).unit + ' ' +
            recipe.ingredients.at(i).name + '\n');
    }

    output_buffer.push_back("\ninstructions\n");

    for (uint64_t i = 0; i < recipe.instructions.size(); i += 1)
    {
        output_buffer.push_back(recipe.instructions.at(i) + '\n');
    }

    while (output_buffer.size() > 0)
    {
        std::cout << output_buffer.front();
        output_buffer.pop_front();
    }

    Reset();
    return;
}

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

int File_Manager::Write_Recipe(const std::string path, const struct Recipe& output_recipe, bool overwrite)
{
    Reset();
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

    while (output_buffer.size() > 0)
    {
        file_output << output_buffer.front();
        output_buffer.pop_front();
    }

    Reset();
    return STATUS_SUCCESS;
}