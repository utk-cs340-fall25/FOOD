#include <iostream>
#include <cstdint>
#include <map>

#include "file_manager.hpp"
#include "error.hpp"

std::map <int64_t, std::string> ERROR_MSGS;

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
    STATUS_LOADER();

    status = fm.Read_Recipe(argv[1], rcp);
    if (status != STATUS_SUCCESS)
    {
        Error_Printer(status, "Read_Recipe");
        return 1;
    }

    fm.print_recipe(rcp);

    status = fm.Write_Recipe(argv[2], rcp, false);
    if (status != STATUS_SUCCESS)
    {
        if (status == STATUS_FILE_EXISTS)
        {
            char ans;
            std::cout << "Do you wish to overwrite the file located at the path? (y/n)";
            std::cin >> ans;
            if (ans == 'y')
            {
                status = fm.Write_Recipe(argv[2], rcp, true);
                if (status != STATUS_SUCCESS)
                {
                    Error_Printer(status, "Write_Recipe");
                    return 1;
                }
            }
        }
        Error_Printer(status, "Write_Recipe");
        return 1;
    }

    return 0;
}