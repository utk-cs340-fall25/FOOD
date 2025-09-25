#include <fstream>
#include <string>
#include <list>
#include <map>

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
    std::vector <Ingredient> ingredients;
    std::vector <std::string> instructions;
};

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

        int Read_Recipe(const std::string, struct Recipe&); // This will read a .rcp file
        int Write_Recipe(const std::string, const struct Recipe&); // This will write to a .rcp file
};