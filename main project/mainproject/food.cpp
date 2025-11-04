#include "food.h"
#include <map>
#include <cstdint>
#include <string>
#include <iostream>
//#include <iomanip>
#include <vector>
#include <experimental/filesystem>
#include <fstream>
#include <sstream>
#include <future>

#include <QString>

// The path to the recipes folder
QString RECIPES_PATH = "./";
QString INGREDIENTS_PATH = "./";
// Associated status messages
std::map<STATUS, std::string> ERROR_MSGS;

void LOG_PRINTER(std::string message)
{
    // The error log
    std::ofstream output("msg-log.log", std::ios::app);
    output << message << '\n';
    output.close();
    return;
}
void LOG_PRINTER(QString message)
{
    LOG_PRINTER(message.toStdString());
    return;
}
void STATUS_PRINTER(STATUS status, std::string prefix)
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
        message = "Invalid or undefined error code: ";
    }

    output << prefix << ": " << message << status <<  '\n';
    output.close();
    return;
}
void STATUS_PRINTER(STATUS status, QString message)
{
    STATUS_PRINTER(status, message.toStdString());
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

STATUS INIT(std::map<QString, Recipe>& recipes, std::map<QString, bool> &ingredients)
{
    STATUS status; // STATUS variable used for determining errors
    STATUS_LOADER(); // Loading statuses for error printing

    // Clearing the msg log
    std::ofstream log("msg-log.log", std::ios::trunc);
    log.close();

    /*//  //  //  //     Loading configuration options     \\  \\  \\  \\*/
    std::ifstream config("config.cfg");
    if (!config.is_open())
    {
        std::string msg = "config.cfg";
        STATUS_PRINTER(STATUS_OPEN_FAILED, msg);
        return STATUS_OPEN_FAILED;
    }
    std::string temp;
    config >> temp;
    RECIPES_PATH = QString::fromStdString(temp);
    config >> temp;
    INGREDIENTS_PATH = QString::fromStdString(temp);
    config.close();
    /*//  //  //  //     Configuration options loaded     \\  \\  \\  \\*/


    /*//  //  //  //     Loading stored ingredients     \\  \\  \\  \\*/
    std::string ing_path = INGREDIENTS_PATH.toStdString() + "ingredients.txt";
    std::ifstream ingredient_pool(ing_path);
    if (!ingredient_pool.is_open())
    {
        QString msg = "ingredients.txt";
        STATUS_PRINTER(STATUS_OPEN_FAILED, ing_path);
        return STATUS_OPEN_FAILED;
    }

    int val;
    QString name_final;
    std::string temp_name;
    while (ingredient_pool >> val)
    {
        std::getline(ingredient_pool, temp_name, '\n');
        if (val != 1 && val != 0) continue;
        name_final = QString::fromStdString(temp_name);
        ingredients[name_final] = true;
    }
    ingredient_pool.close();
    /*//  //  //  //     Stored ingredients loaded     \\  \\  \\  \\*/

    /*//  //  //  //     Loading stored recipes     \\  \\  \\  \\*/
    // Mulithreading variables
    std::vector<QString> file_names;
    std::vector<Recipe> loaded_recipes;
    std::vector<std::future<STATUS>> threads;

    // Finding how many recipes there are
    uint64_t file_num = 0;
    for (const std::experimental::filesystem::directory_entry& entry : std::experimental::filesystem::directory_iterator(RECIPES_PATH.toStdString()))
    {
        file_num += 1;
    }

    loaded_recipes.resize(file_num);
    file_names.resize(file_num);
    threads.resize(file_num);

    // Reading each file with its own dedicated thread.
    uint64_t i = 0;
    for (const std::experimental::filesystem::directory_entry& entry : std::experimental::filesystem::directory_iterator(RECIPES_PATH.toStdString()))
    {
        std::string path_std = entry.path().u8string(); // converting a path to a std::string
        QString file_path = QString::fromStdString(path_std);
        threads.at(i) = std::async(&Read_Recipe, file_path, &loaded_recipes.at(i), false);
        file_names.at(i) = file_path;
        i += 1;
    }

    // Moving the read recipes to the output list.
    for (uint64_t i = 0; i < file_num; i += 1)
    {
        status = threads.at(i).get();

        // If a file failed to be read, don't load that file.
        if (status != STATUS_SUCCESS)
        {
            //STATUS_PRINTER(status, file_names.at(i));
            continue;
        }

        recipes[loaded_recipes.at(i).name] = loaded_recipes.at(i);
    }
    /*//  //  //  //     Recipes have been loaded     \\  \\  \\  \\*/

    /*//  //  //  //     Initialization succeeded     \\  \\  \\  \\*/
    std::string temp_str = "Initialization";
    STATUS_PRINTER(STATUS_SUCCESS, temp_str);
    temp_str = "";
    LOG_PRINTER(temp_str);
    return STATUS_SUCCESS;
}
STATUS DEINIT(std::map<QString, Recipe>& recipes, std::map<QString, bool> &ingredients)
{
    /*//  //  //  //     Storing owned ingredients     \\  \\  \\  \\*/
    std::string ing_path = INGREDIENTS_PATH.toStdString() + "ingredients.txt";
    std::ofstream ingredient_pool(ing_path, std::ofstream::trunc);
    if (!ingredient_pool.is_open())
    {
        STATUS_PRINTER(STATUS_OPEN_FAILED, ing_path);
    }
    else
    {
        for (std::map<QString, bool>::iterator it = ingredients.begin(); it != ingredients.end(); it++)
        {
            if (it->second == true) { ingredient_pool << "1 "; }
            else if (it->second == true) { ingredient_pool << "0 "; }
            else { continue; }
            ingredient_pool << it->first.toStdString() << '\n';
        }
    }
    ingredient_pool.close();
    /*//  //  //  //     Ingredients have been stored    \\  \\  \\  \\*/

    /*//  //  //  //     Storing all recipes     \\  \\  \\  \\*/
    std::vector<std::future<STATUS>> threads;
    std::vector<QString> file_names;
    threads.resize(recipes.size());

    uint64_t i = 0;
    for (std::map<QString, Recipe>::iterator rcp_it = recipes.begin(); rcp_it != recipes.end(); rcp_it++)
    {
        QString file_name = rcp_it->second.name;
        file_names.push_back(file_name);
        threads.at(i) = std::async(&Write_Recipe, RECIPES_PATH + file_name, &rcp_it->second, false);
        i += 1;
    }

    for (i = 0; i < threads.size(); i += 1)
    {
        STATUS status = threads.at(i).get();
        if (status != STATUS_SUCCESS)
        {
            //STATUS_PRINTER(status, file_names.at(i));
        }
    }
    /*//  //  //  //     All recipes stored     \\  \\  \\  \\*/
    std::string temp = "Deinitialization";
    STATUS_PRINTER(STATUS_SUCCESS, temp);
    return STATUS_SUCCESS;
}

QString to_lower(QString string)
{
    std::string output = string.toStdString();

    for (uint64_t i = 0; i < string.size(); i += 1)
    {
        output.at(i) = static_cast<char>(tolower(output.at(i)));
    }

    return QString::fromStdString(output);
};
STATUS fraction_to_double(QString input_str, double& result)
{
    // The numerator and denominator in string form
    std::string numerator_str, denominator_str;
    // The numerator and denominator in double form
    double numerator_d, denominator_d;
    denominator_d = 0;

    std::stringstream stream;
    stream.str(input_str.toStdString());

    std::getline(stream, numerator_str, '/');
    std::getline(stream, denominator_str);
    stream.clear();

    // Converting the string to a double
    try
    {
        numerator_d = static_cast<double>(std::stod(numerator_str));
        if (denominator_str.size() != 0) denominator_d = static_cast<double>(std::stod(denominator_str));
    }
    catch (std::invalid_argument)
    {
        result = 0;
        return STATUS_BAD_ARGUMENTS;
    }
    if (denominator_d == 0L) denominator_d = 1L;
    result = numerator_d / denominator_d;

    return (result == 0L) ? STATUS_BAD_ARGUMENTS : STATUS_SUCCESS;
}

STATUS Read_Recipe(QString path, struct Recipe* output_recipe, bool individual)
{
    if (path.toStdString().substr(path.size() - 4, 4) != ".rcp") { return STATUS_BAD_EXTENSION; }

    std::ifstream file_input(path.toStdString(), std::ios::in);
    if (!file_input.is_open())
    {
        if (!std::experimental::filesystem::exists(path.toStdString())) { return STATUS_FILE_NOT_FOUND; }
        else { return STATUS_OPEN_FAILED; }
    }

    // File Reading
    struct Recipe recipe;
    std::string temp_string;

    std::getline(file_input, temp_string, '\n');
    recipe.name = QString::fromStdString(temp_string);
    recipe.name = to_lower(recipe.name);
    temp_string.resize(1);

    // Reading all the ingredients
    std::getline(file_input, temp_string, '\n');
    for (;to_lower(QString::fromStdString(temp_string)) != "instructions"; std::getline(file_input, temp_string, '\n'))
    {
        if (temp_string.size() == 0) { continue; }
        STATUS status;
        struct Ingredient temp_ingredient;
        std::istringstream splitter(temp_string);

        splitter >> temp_string;
        temp_ingredient.amount_s = QString::fromStdString(temp_string);
        splitter >> temp_string;
        temp_ingredient.unit = QString::fromStdString(temp_string);
        std::getline(splitter, temp_string, '\n');
        temp_ingredient.name = to_lower(QString::fromStdString(temp_string));

        status = fraction_to_double(temp_ingredient.amount_s, temp_ingredient.amount_d);
        if (status != STATUS_SUCCESS)
        {
            file_input.close();
            splitter.clear();
            return status;
        }

        recipe.ingredients.push_back(temp_ingredient);

        splitter.clear();
    }

    // Reading all instructions
    temp_string.clear();
    for (;to_lower(QString::fromStdString(temp_string)) != "tags"; std::getline(file_input, temp_string, '\n'))
    {
        if (temp_string.size() == 0) { continue; }
        recipe.instructions.push_back(QString::fromStdString(temp_string));
    }

    // Reading all tags
    temp_string.clear();
    while (std::getline(file_input, temp_string, '\n'))
    {
        if (temp_string.size() == 0) { continue; }
        recipe.tags.push_back(QString::fromStdString(temp_string));
    }

    *output_recipe = recipe;
    file_input.close();

    STATUS_PRINTER(STATUS_SUCCESS, "Read_Recipe: " + path);
    temp_string = "";
    if (individual) { LOG_PRINTER(temp_string); }

    return STATUS_SUCCESS;
}
STATUS Write_Recipe(const std::string path, struct Recipe* output_recipe, bool individual)
{
    if (output_recipe->name.size() == 0) { return STATUS_INVALID_DATA; }
    // Checking the file extenstion
    if (path.substr(path.size() - 4, 4) != ".rcp") { return STATUS_BAD_EXTENSION; }
    // Checking if the file can be overwritten

    // Opening/creating the ouput file
    std::ofstream file_output(path, std::ios::trunc | std::ios::out);
    if (!file_output.is_open())
    {
        if (!std::experimental::filesystem::exists(path)) { return STATUS_FILE_NOT_FOUND; }
        else return STATUS_OPEN_FAILED;
    }

    file_output << output_recipe->name.toStdString() << "\n\n";
    for (uint64_t i = 0; i < output_recipe->ingredients.size(); i += 1)
    {
        file_output << output_recipe->ingredients[i].amount_s.toStdString() << ' ';
        file_output << output_recipe->ingredients[i].unit.toStdString() << ' ';
        file_output << output_recipe->ingredients[i].name.toStdString() << '\n';
    }

    file_output << "\ninstructions\n";
    for (uint64_t i = 0; i < output_recipe->instructions.size(); i += 1)
    {
        file_output << output_recipe->instructions[i].toStdString() << '\n';
    }

    file_output << "\ntags\n";
    for (uint64_t i = 0; i < output_recipe->tags.size(); i += 1)
    {
        file_output << output_recipe->tags[i].toStdString() << '\n';
    }

    file_output.close();

    STATUS_PRINTER(STATUS_SUCCESS, "Write_Recipe: " + path);
    std::string temp_string = "";
    if (individual) { LOG_PRINTER(temp_string); }

    return STATUS_SUCCESS;
}
