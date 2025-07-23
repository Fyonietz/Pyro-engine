#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ostream>
#include <vector>

int main(){
    std::fstream file("example.txt");
    std::ofstream file_output("syntax.list");
    std::stringstream file_buffer;
    file_buffer << file.rdbuf();
    std::string file_buffer_string(file_buffer.str());

    std::vector<std::string> after_scan;
    size_t scan_begin = file_buffer_string.find("@");
    while (scan_begin != std::string::npos)
    {
        // std::cout << file_buffer_string << std::endl;
        scan_begin = file_buffer_string.find("@",scan_begin-1);
        after_scan.push_back(file_buffer_string);
    }
    for(auto cs : after_scan){
        std::cout << cs ;
        file_output << cs;
    }
    


    return 0;
}