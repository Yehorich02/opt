#include "CodeGenerator.hpp"



bool CodeGenerator::code_generator_analysis(list<node*> tree, string output_file_name){
    list<node*> :: iterator it = tree.begin();
    was_error = !(generate(it));
    if(!check_correct_use_labels()){
        was_error = true;
    }
    make_listing(output_file_name);
    return !was_error;
} 

bool CodeGenerator::generate(list<node*> :: iterator it){
    if((*it)->code == END.code) return true;
    if((*it)->code == PROGRAM.code){
        while ((*it)->code != MIN_IDENTIFIER_CODE) ++it;
        program_name = (*it)->name;
        return generate(++it);
    }else if((*it)->code == LABEL.code){
        while ((*it)->name != ";"){
            if(is_unsigned_integer((*it)->code)){
                labels.push_back(new label((*it)->name, false, false));
            }
            ++it;
        }
        if(!check_labels()){
             err = "Code Generator Error, different labels cant have the same name";
             return false;
        }
        return generate(++it);
    }else if ((*it)->code == GOTO.code){
        label* l = get_label((*(++++it))->name);
        if(l == nullptr){
            err = "Code Generator Error, GOTO use with undeclarated label";
            return false;
        }
        l->wasCall = true;
        asm_code.push_back("jpm " + l->value);
        return generate(++it);
    }else if ((*it)->code == IN.code){
        port_ax = stoi((*(++++it))->name);
        asm_code.push_back("IN AX, " + to_string(port_ax));
        return generate(++it);
    }else if((*it)->code == OUT.code){
        port_dx = stoi((*(++++it))->name);
        asm_code.push_back("OUT DX, " + to_string(port_dx));
        return generate(++it);
    }else if ((*it)->code == LINK.code){
        string v_name = (*(++++++it))->name;
        int v = stoi((*(++++++++++it))->name);
        if(v == port_dx){
            asm_data.push_back(v_name + " DW 0h");
            asm_code.push_back("MOV " + v_name + ", DX");
        }else if (v == port_ax){
            asm_data.push_back(v_name + " DW 0h");
            asm_code.push_back("MOV " + v_name + ", AX");
        }else {
            err = "Code Generator Error: use LINK with undeclarated port";
            return false;
        }
        return generate(++it);
    }else if(is_unsigned_integer((*it)->code)){
        label* l = get_label((*it)->name);
        if(l == nullptr){
            err = "Code Generator Error, use undeclarated label: " + (*it)->name;
            return false;
        } 
        l->wasAdd = true; 
        asm_code.push_back(l->value + ":");
        return generate(++++it);
    }else
        return generate(++it);
}



void CodeGenerator::make_listing(string output_file_name){
    ofstream output_file(output_file_name);
    
    output_file << ".data" << endl;
    for(string s : asm_data){
        output_file << s << endl;
    }

    output_file << endl << ".code" << endl;
    
    for(string s: asm_code){
        output_file << s << endl;
    }
    if(err == "")
        output_file << "MOV AX, 4c00h" << endl << "int 21h"<<endl;
    else{ 
        output_file << err << endl;
        cout << err << endl;
    }
    output_file.close();
}

bool CodeGenerator::is_unsigned_integer(int lexem_code){
    return (lexem_code >= MIN_DIGIT_CODE && lexem_code < MIN_IDENTIFIER_CODE);
}

bool CodeGenerator::check_labels(){
    for(label* l1 : labels){
        for(label* l2 : labels){
            if((l1 != l2) && (l1->value == l2->value)) return false;
        }
    }

    return true;
}

label* CodeGenerator::get_label(string name){
    for(label* l : labels){
        if(l->value == name) return l;
    }

    return nullptr;
}

bool CodeGenerator::check_correct_use_labels(){
    for(label* l : labels){
        if(l->wasAdd == false && l->wasCall == true){
            err = "Code Generator Error: label was called but was not added: " + l->value;
            return false;
        }
    }

    return true;
}