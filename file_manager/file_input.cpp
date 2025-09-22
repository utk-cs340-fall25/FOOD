#include "file_manager.hpp"
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <winerror.h>

void File_Manager::Read_Recipe(const std::string path)
{
    // For saving an already loaded file
    if (buffered_rcp == true)
    {
        bool complete = false;

        do
        {
            char ans;
            std::cout << "There is a buffered recipe called " << rcp_name << ". Would you like to save it? (y/n)";
            std::cin >> ans;

            // If the file is to be saved or not
            if (ans == 'y')
            {
                std::filesystem::path out_path;
                std::cout << "Enter the path of the file you wish to write to:";
                std::cin >> out_path;
                
                // Finding a valid or acceptable path to write to
                do
                {
                    if (std::filesystem::exists(out_path))
                    {
                        std::cout << "File already exists. Do you wish to overwrite it? (y/n)";
                        std::cin >> ans;

                        // Answer checking
                        if (ans == 'y')
                        {
                            Write_Recipe(out_path.string());
                            complete = true;
                        }
                        else if (ans == 'n')
                        {
                            std::cout << "Enter the path of the file you wish to write to:";
                            std::cin >> out_path;
                            continue;
                        }
                        else throw (std::invalid_argument("Invalid answer of " + ans + '\n'));
                    } 
                    else
                    {
                        Write_Recipe(out_path.string());
                        complete = true;
                        break;
                    }
                } while (true);     
            }
            else if (ans == 'n')
            {
                Reset();
                complete = true;
            }
            else throw (std::invalid_argument("Invalid answer of " + ans + '\n'));

        } while (!complete);
    }

    file_input.open(path);
    if (!file_input.is_open())
    {
        throw (ERROR_FILE_NOT_FOUND);
        return;
    }

    // File Reading

    return;
}