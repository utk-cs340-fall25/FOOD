#include <fstream>
#include <string>
#include <list>
#include <map>



class File_Manager
{
    private:
        // Input
        std::ifstream file_input;
        std::list <std::string> input_buffer;

        // Output
        std::ofstream file_output;
        std::list <std::string> output_buffer;

        // Utils
        bool buffered_rcp;
        std::string rcp_name;

        void Reset(void);
    public:
        File_Manager();

        void Read_Recipe(const std::string); // This will read a .rcp file
        void Write_Recipe(const std::string); // This will write to a .rcp file
};