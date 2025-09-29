#include <iostream>
#include <cstdint>
#include <map>

#include "file_manager.hpp"
#include "../error_handlers/error.hpp"

std::map <int64_t, std::string> ERROR_MSGS;

void ERROR_LOADER()
{
    ERROR_MSGS[ERROR_SUCCESS] = "Successfuly performed";
    ERROR_MSGS[ERROR_OPEN_FAILED] = "Invalid permissions";
    ERROR_MSGS[ERROR_FILE_NOT_FOUND] = "No file found at path";
    ERROR_MSGS[ERROR_INVALID_DATA] = "Invalid data in file";
    ERROR_MSGS[ERROR_BAD_ARGUMENTS] = "Invalid arguments were passed";
    ERROR_MSGS[ERROR_FILE_EXISTS] = "File already exists";
    
    return;
}

uint64_t Error_Printer(int64_t status, std::string prefix)
{
    std::cerr << prefix << ": ";
    if (ERROR_MSGS.find(status) == ERROR_MSGS.end())
    {
        std::cerr << "Invalid or undefined error code";
        return 0;
    }

    std::cerr << ERROR_MSGS[status];
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input_path> <output_path>\n", argv[0]);
        return 1;
    }

    File_Manager fm;
    struct Recipe rcp;

    int64_t status;

    status = fm.Read_Recipe(argv[1], rcp);
    if (status != ERROR_SUCCESS)
    {
        Error_Printer(status, "Read_Recipe");
        return 1;
    }

    fm.print_recipe(rcp);

    status = fm.Write_Recipe(argv[1], rcp);
    if (status != ERROR_SUCCESS)
    {
        Error_Printer(status, "Read_Recipe");
        return 1;
    }

    return 0;
}