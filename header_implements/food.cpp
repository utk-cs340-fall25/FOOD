#include "../headers/food.hpp"
#include "../headers/file_manager.hpp"

#include <map>
#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <future>

struct Ingredient
{
    std::string name;
    std::string amount_s;
    std::string unit;
    double amount_d;
};
struct Recipe
{
    std::string name;
    std::vector<Ingredient> ingredients;
    std::vector<std::string> instructions;
};

std::map<int64_t, std::string> ERROR_MSGS;

void ERROR_PRINTER(int64_t status, std::string prefix)
{
    std::cerr << prefix << ": ";
    if (ERROR_MSGS.find(status) == ERROR_MSGS.end())
    {
        std::cerr << "Invalid or undefined error code";
        return;
    }

    std::cerr << ERROR_MSGS[status];
    return;
}
void STATUS_LOADER()
{
    ERROR_MSGS[STATUS_SUCCESS] = "Successfuly performed";
    ERROR_MSGS[STATUS_OPEN_FAILED] = "Invalid permissions";
    ERROR_MSGS[STATUS_FILE_NOT_FOUND] = "No file found at path";
    ERROR_MSGS[STATUS_INVALID_DATA] = "Invalid data in file";
    ERROR_MSGS[STATUS_BAD_ARGUMENTS] = "Invalid arguments were passed";
    ERROR_MSGS[STATUS_FILE_EXISTS] = "File already exists";
    ERROR_MSGS[STATUS_BAD_EXTENSION] = "File has an improper extension";

    return;
}
void INIT(std::list<Recipe> &saved_recipes)
{
    STATUS_LOADER();

    std::vector <std::string> file_names;
    std::vector<Recipe> loaded_recipes;
    std::vector<std::future<uint64_t>> threads;
    std::vector<uint64_t> statuses;

    uint64_t file_num;
    for(const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator("../recipes"))
    {
        file_num += 1;
    }

    loaded_recipes.resize(file_num);
    file_names.resize(file_num);
    threads.resize(file_num);
    statuses.resize(file_num);

    uint64_t i = 0;
    for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator{"../recipes"})
    {
        threads.at(i) = std::async(Read_Recipe, entry.path().u8string(), loaded_recipes.at(i));
        file_names.at(i) = entry.path().u8string();
        i += 1;
    }

    for (uint64_t i = 0; i < file_num; i += 1) { statuses.at(i) = threads.at(i).get(); }
    for (uint64_t i = 0; i < file_num; i += 1)
    {
        if (statuses.at(i) != STATUS_SUCCESS) { ERROR_PRINTER(statuses.at(i), file_names.at(i));}
    }

    return;
}
void DEINIT()
{

}

std::string to_lower(std::string string)
{
    std::string output = string;

    for (uint64_t i = 0; i < string.length(); i += 1)
    {
        output.at(i) = static_cast<char>(tolower(output.at(i)));
    }

    return output;
};
STATUS fraction_to_double(std::string input_str, double &result)
{
    std::string numerator_str, denominator_str;
    double numerator_d, denominator_d;
    std::stringstream stream;
    stream.str(input_str);

    std::getline(stream, numerator_str, '/');
    std::getline(stream, denominator_str);
    stream.clear();

    try
    {
        numerator_d = static_cast<double>(std::stoi(numerator_str));
        denominator_d = static_cast<double>(std::stoi(denominator_str));
    }
    catch(std::invalid_argument) { return STATUS_BAD_ARGUMENTS; }

    result = numerator_d;
    result /= denominator_d;

    return STATUS_SUCCESS;
}

/*
** Read a file from the given path.
*/
STATUS Read_Recipe(const std::string path, struct Recipe& output_recipe)
{
    if (path.substr(path.length() - 4, 4) != ".rcp") return STATUS_BAD_EXTENSION;

    std::ifstream file_input(path, std::ios::in);
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
            temp_ingredient.amount_d = fraction_to_double(temp_ingredient.amount_s);
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
    file_input.close();
    return STATUS_SUCCESS;
}
STATUS Write_Recipe(const std::string path, const struct Recipe& output_recipe, bool overwrite)
{
    if (path.substr(path.length() - 4, 4) != ".rcp") return STATUS_BAD_EXTENSION;
    if (std::filesystem::exists(path) && !overwrite) return STATUS_FILE_EXISTS;

    std::ofstream file_output(path, std::ios::trunc | std::ios::out);
    if (!file_output.is_open())
    {
        if (!std::filesystem::exists(path)) return STATUS_FILE_NOT_FOUND;
        else return STATUS_OPEN_FAILED;
    }
    if (output_recipe.name == "") return STATUS_INVALID_DATA;

    std::list<std::string> output_buffer;
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

    file_output.close();
    return STATUS_SUCCESS;
}