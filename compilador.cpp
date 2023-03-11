#include <iostream>
#include <string>
#include <vector>


const int ERR = -1;
const int ACP = 99;
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

    // let  dig del opa <   >   =   .   "
    {   1,  2,  6,  5,  10, 8,  7, ERR, 12}, //0
    {   1,  1, ACP, ACP,ACP,ACP,ACP,ACP,ACP}, // 1
    {   ACP,2, ACP, ACP,ACP,ACP,ACP, 3, ACP}, // 2
    {   ERR, 4,ERR,ERR,ERR,ERR,ERR,ERR, ERR}, // 3
    {   ACP, 4, ACP,ACP,ACP,ACP,ACP,ACP, ACP}, // 4
    {   ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP}, // 5
    {   ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP}, // 6
    {   ACP,ACP,ACP,ACP,ACP,ACP, 9, ACP, ACP}, // 7
    {   ACP,ACP,ACP,ACP,ACP,ACP, 9, ACP, ACP}, // 8
    {   ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP}, // 9
    {   ACP,ACP,ACP,ACP,ACP, 11, 9, ACP, ACP}, // 10
    {   ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP}, // 11
    {   12,  12, 12, 12, 12, 12, 12, 12, 13}, // 12
    {   ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP} // 13
};

int main(){
    std::cout << "hola mundo!";

    return 0;
}