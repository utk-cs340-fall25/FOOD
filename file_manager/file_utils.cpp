#include "file_manager.hpp"
#include "error.hpp"

#include <iostream>
#include <cstdint>

std::string to_lower(std::string string)
{
    std::string output = string;

    for (uint64_t i = 0; i < string.length(); i += 1)
    {
        output.at(i) = (char)tolower((char)output.at(i));
    }

    return output;
};

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