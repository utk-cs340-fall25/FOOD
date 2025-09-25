#include "file_manager.hpp"

std::string to_lower(std::string string)
{
    std::string output = string;

    for (uint64_t i = 0; i < string.length(); i += 1)
    {
        output.at(i) = (char)tolower((char)output.at(i));
    }

    return output;
};

void File_Manager::Reset(void)
{
    file_input.close();
    file_output.close();
    input_buffer.clear();
    output_buffer.clear();

    buffered_rcp = false;
    rcp_name.clear();

    return;
}