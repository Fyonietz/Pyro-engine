#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ostream>
#include <vector>

void copy_block(std::string layout,std::string tag){
    std::fstream file(layout);

    std::stringstream file_buffer;
    file_buffer << file.rdbuf();
    std::string file_buffer_string(file_buffer.str());
    size_t scan_begin = file_buffer_string.find(tag);
    size_t scan_end = file_buffer_string.find("@@end");
    std::string matching = file_buffer_string.substr(scan_begin+tag.length(),scan_end-scan_begin-tag.length());

    while(scan_begin != std::string::npos){
        scan_end = file_buffer_string.find("@@end",scan_begin);
        if(!scan_end){
            std::cerr<< "Error" << std::endl;
        }
        matching = file_buffer_string.substr(scan_begin+tag.length(),scan_end-scan_begin-tag.length());
        scan_begin = file_buffer_string.find(tag,scan_begin+1);
        std::cout << matching << std::endl;
    }
}




int main(){
    std::fstream syntax("syntax.list");

    std::vector<std::string> tag_list_vect;
    std::string tag_list;

    if(syntax.is_open()){
        while(std::getline(syntax,tag_list)){
        tag_list_vect.push_back(tag_list);
        }
    }else{
        std::cerr << "File May Not Exits" << std::endl;
    }
 
    for(const auto &for_tag : tag_list_vect){
        copy_block("layout.html",for_tag);
    }
    


    return 0;
}