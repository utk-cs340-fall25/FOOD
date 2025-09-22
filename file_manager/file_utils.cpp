#include "file_manager.hpp"

void File_Manager::Reset(void)
{
    file_input.close();
    file_output.close();
    input_buffer.clear();
    output_buffer.clear();

    return;
}