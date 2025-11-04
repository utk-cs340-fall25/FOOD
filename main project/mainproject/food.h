#pragma once

#include <cstdint>
#include <string>
//#include <list>
#include <map>
#include <vector>

#include <QString>

// The STATUS type.
typedef uint64_t STATUS;
// The STATUS codes.
enum CODES : STATUS
{
    // Success Status.
    STATUS_SUCCESS,
    // A file failed to open.
    STATUS_OPEN_FAILED,
    // A file was not found.
    STATUS_FILE_NOT_FOUND,
    // Invalid data was present. Context dependent.
    STATUS_INVALID_DATA,
    // Invalid arguments were passed. Context dependent.
    STATUS_BAD_ARGUMENTS,
    // A file already exists. Functions as O_EXCL
    STATUS_FILE_EXISTS,
    // A file with an invalid extension is used.
    STATUS_BAD_EXTENSION
};

// Structure for an ingredient
struct Ingredient
{
    QString name; // Name of the ingredient. Should be all lowercase letters.
    QString amount_s; // The amount of an ingredient in a string format.
    QString unit; // The unit of how much and ingredient is present. (Such as cups, ozs, tsps, etc..)
    double amount_d = 0; // How much of an ingredient is present in a numerical format.
};
// Structure for a recipe
struct Recipe
{
    QString name; // The name of the recipe.
    std::vector<Ingredient> ingredients; // The ingredients in a recipe.
    std::vector<QString> instructions; // The instructions of a recipe.
    std::vector<QString> tags;
};

// Loads status messages
void STATUS_LOADER();
// Prints a message to the msg-log.log file.
void LOG_PRINTER(std::string);
void LOG_PRINTER(QString);
// Prints status messages to msg-log.log file.
void STATUS_PRINTER(STATUS, std::string);
void STATUS_PRINTER(STATUS, QString);
// Initializes and loads basic data for the app.
STATUS INIT(std::map<QString, Recipe>&, std::map<QString, bool>&);
// Deinititializes and stores basic data for the app.
STATUS DEINIT(std::map<QString, Recipe>&, std::map<QString, bool>&);

// Converts a string to all lowercase.
QString to_lower(QString);
// Converts a string-fraction into a double.
STATUS fraction_to_double(QString, double&);

// This will read a .rcp file.
STATUS Read_Recipe(QString, struct Recipe*, bool);
// This will write to a .rcp file. The name of the file should be the 
// same as the name of the recipe.
STATUS Write_Recipe(QString, struct Recipe*, bool);

