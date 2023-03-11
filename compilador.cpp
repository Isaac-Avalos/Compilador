#include <iostream>
#include <string>
#include <vector>


const int ERROR = -1;
const int ACPEPT = 99;
int idx = 0;
bool cERR = false;
std::string token = "";
std::string lexema = "";
bool check_bool_main = false;
int line = 1;
int col = 1;

void compileError(std::string error_type, std::string desc){
    std::cout << "[" << line << "]" << "[" << "]" << error_type << " " << desc << std::endl;
    cERR = true;
}

std::vector<std::vector<int>> transition_table = {

};

int main(){
    std::cout << "hola mundo!";

    return 0;
}