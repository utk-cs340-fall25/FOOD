#include "food.hpp"

#include <map>
#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <future>

// The path to the recipes folder
std::string RECIPES_PATH;
// Associated status messages
std::map<STATUS, std::string> ERROR_MSGS;

void ERROR_PRINTER(STATUS status, std::string prefix)
{
    // The error log
    std::ofstream output("msg-log.log", std::ios::app);
    std::string message;
    try
    {
        message = ERROR_MSGS.at(status);
    }
    catch (std::out_of_range)
    {
        // Default error message
        message = "Invalid or undefined error code: " + status;
    }

    output << prefix << ": " << message << '\n';
    output.close();
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
STATUS INIT(std::map<std::string, Recipe>& recipes)
{
    STATUS status;
    STATUS_LOADER();

    // Loading configuration options
    std::ifstream config("config.cfg");
    if (!config.is_open())
    {
        ERROR_PRINTER(STATUS_OPEN_FAILED, "config.cfg");
        return STATUS_OPEN_FAILED;
    }
    config >> RECIPES_PATH;



    struct Recipe default_recipe;
    default_recipe.ingredients.clear();
    default_recipe.instructions.clear();
    default_recipe.name.clear();

    // Mulithreading variables
    std::vector <std::string> file_names;
    std::vector<Recipe> loaded_recipes;
    std::vector<std::future<STATUS>> threads;

    // Finding how many recipes there are
    uint64_t file_num = 0;
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(RECIPES_PATH))
    {
        file_num += 1;
    }

    loaded_recipes.resize(file_num);
    file_names.resize(file_num);
    threads.resize(file_num);

    // Reading each file with its own dedicated thread.
    uint64_t i = 0;
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(RECIPES_PATH))
    {
        std::string path = entry.path().u8string();
        threads.at(i) = std::async(&Read_Recipe, path, &loaded_recipes.at(i));
        file_names.at(i) = path;
        i += 1;
    }

    // Moving the read recipes to the output list.
    for (uint64_t i = 0; i < file_num; i += 1)
    {
        status = threads.at(i).get();
        // If a file failed to be read, don't load that file.
        if (status != STATUS_SUCCESS)
        {
            ERROR_PRINTER(status, file_names.at(i));
            continue;
        }

        recipes[loaded_recipes.at(i).name] = loaded_recipes.at(i);
    }

    return STATUS_SUCCESS;
}
STATUS DEINIT(const std::map<std::string, Recipe>& recipes)
{
    const std::string path_prefix = RECIPES_PATH;
    std::vector<std::future<STATUS>> threads;
    std::vector<std::string> file_names;
    std::map<std::string, Recipe>::const_iterator rcp_it;

    uint64_t i = 0;
    threads.resize(recipes.size());
    for (rcp_it = recipes.begin(); rcp_it != recipes.end(); rcp_it++)
    {
        std::string file_name = rcp_it->second.name + ".rcp";
        file_names.push_back(file_name);
        threads.at(i) = std::async(&Write_Recipe, RECIPES_PATH + file_name, rcp_it->second, true);
        i += 1;
    }

    for (i = 0; i < threads.size(); i += 1)
    {
        STATUS status = threads.at(i).get();
        if (status != STATUS_SUCCESS)
        {
            ERROR_PRINTER(status, file_names.at(i));
        }
    }

    return STATUS_SUCCESS;
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
STATUS fraction_to_double(std::string input_str, double& result)
{
    // The numerator and denominator in string form
    std::string numerator_str, denominator_str;
    // The numerator and denominator in double form
    double numerator_d, denominator_d;

    std::stringstream stream;
    stream.str(input_str);

    std::getline(stream, numerator_str, '/');
    std::getline(stream, denominator_str);
    stream.clear();
    
    // Converting the string to a double
    try
    {
        numerator_d = static_cast<double>(std::stod(numerator_str));
        denominator_d = static_cast<double>(std::stod(denominator_str));
    }
    catch (std::invalid_argument) 
    {
        result = 0;
        return STATUS_BAD_ARGUMENTS; 
    }

    result = numerator_d / denominator_d;

    return STATUS_SUCCESS;
}

STATUS Read_Recipe(const std::string path, struct Recipe* output_recipe)
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
        STATUS status;
        struct Ingredient temp_ingredient;
        file_input >> temp_ingredient.amount_s >> temp_ingredient.unit >> temp_ingredient.name;
        temp_ingredient.name = to_lower(temp_ingredient.name);

        status = fraction_to_double(temp_ingredient.amount_s, temp_ingredient.amount_d);
        if (status != STATUS_SUCCESS){ return status; }

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

    *output_recipe = recipe;
    file_input.close();
    return STATUS_SUCCESS;
}
STATUS Write_Recipe(const std::string path, const struct Recipe& output_recipe, bool overwrite)
{
    // Checking the file extenstion
    if (path.substr(path.length() - 4, 4) != ".rcp") { return STATUS_BAD_EXTENSION; }
    // Checking if the file can be overwritten
    if (!overwrite && std::filesystem::exists(path)) { return STATUS_FILE_EXISTS; }

    // Opening/creating the ouput file
    std::ofstream file_output(path, std::ios::trunc | std::ios::out);
    if (!file_output.is_open())
    {
        if (!std::filesystem::exists(path)) return STATUS_FILE_NOT_FOUND;
        else return STATUS_OPEN_FAILED;
    }
    if (output_recipe.name == "") return STATUS_INVALID_DATA;

    // Buffer for writing to the file all at once
    std::list<std::string> output_buffer;

    // Pushing the file's name
    output_buffer.push_back(to_lower(output_recipe.name) + "\n\n");

    // Pushing the file's ingredients
    for (uint64_t i = 0; i < output_recipe.ingredients.size(); i += 1)
    {
        if (output_recipe.ingredients.at(i).amount_s == "") return STATUS_INVALID_DATA;
        if (output_recipe.ingredients.at(i).unit == "") return STATUS_INVALID_DATA;

        output_buffer.push_back(output_recipe.ingredients.at(i).amount_s + ' ' +
            output_recipe.ingredients.at(i).unit + ' ' +
            output_recipe.ingredients.at(i).name + '\n');
    }
    output_buffer.push_back("\n");

    // Pushing the file's instructions
    output_buffer.push_back("instructions\n");
    for (uint64_t i = 0; i < output_recipe.instructions.size(); i += 1)
    {
        if (output_recipe.instructions.at(i) == "") return STATUS_INVALID_DATA;
        output_buffer.push_back(output_recipe.instructions.at(i) + '\n');
    }

    // Writing from the buffer to the file.
    while (output_buffer.size() > 0)
    {
        file_output << output_buffer.front();
        output_buffer.pop_front();
    }

    // Cleanup.
    file_output.close();
    return STATUS_SUCCESS;
}