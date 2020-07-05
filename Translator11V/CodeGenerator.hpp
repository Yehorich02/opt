#pragma ocnce
#include "TranslatorConstants.hpp"
#include <iterator>

class CodeGenerator {

public:    
    CodeGenerator(){};
    ~CodeGenerator(){};

    bool code_generator_analysis(list<node*> tree, string output_file_name);

private:
    list<string> asm_code;
    list<string> asm_data;
    list<node*> tree;
    list<label*> labels;

    string program_name;
    string err;
    int port_ax = -1;
    int port_dx = -1;
    bool was_error;

    bool generate( list<node*> :: iterator it);
    bool is_unsigned_integer(int);
    bool check_labels();
    bool check_correct_use_labels();
    label* get_label(string name);
    void make_listing(string);
};