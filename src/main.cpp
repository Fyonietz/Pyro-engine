#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <filesystem>

std::vector<std::string> dir_listing(){
    std::filesystem::path path = "public/views";
    std::vector<std::string> file_list;
    try
    {
        for(const auto &entry : std::filesystem::recursive_directory_iterator(path)){
            if(std::filesystem::is_regular_file(entry)&& entry.path().extension()==".html"){
                std::filesystem::path rel_path = std::filesystem::relative(entry.path(),path);
                // std::cout << rel_path <<  std::endl;
                file_list.push_back(entry.path().string());
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return file_list;
}



std::string copy_block(std::string layout, std::string tag) {
    std::fstream file(layout);
    if (!file.is_open()) {
        std::cerr << "Cannot open layout file: " << layout << std::endl;
        return "";
    }

    std::stringstream file_buffer;
    file_buffer << file.rdbuf();
    std::string file_buffer_string(file_buffer.str());

    size_t scan_begin = file_buffer_string.find(tag);
    size_t scan_end = file_buffer_string.find("@end", scan_begin);

    std::string matching = file_buffer_string.substr(
        scan_begin + tag.length(),
        scan_end - (scan_begin + tag.length())
    );

    return matching;
}

void insert_block(std::string child, std::string tag) {
    std::ifstream file_input(child);
    if (!file_input.is_open()) {
        std::cerr << "Cannot open child file: " << child << std::endl;
        return;
    }

    std::stringstream file_input_stream;
    file_input_stream << file_input.rdbuf();
    std::string file_input_str = file_input_stream.str();
    file_input.close();

    std::string insert = copy_block("public/layout.html", tag);
    if (insert.empty()) return;

    size_t scan_begin = 0;
    while ((scan_begin = file_input_str.find(tag, scan_begin)) != std::string::npos) {
        size_t line_after_tag = file_input_str.find('\n', scan_begin);
        if (line_after_tag == std::string::npos) break;

        size_t next_tag = file_input_str.find(tag, scan_begin + 1);
        std::string after_tag = file_input_str.substr(line_after_tag + 1, insert.length());

        if (after_tag.find(insert) != 0) {
            file_input_str.insert(line_after_tag + 1, insert + "\n");
            scan_begin = line_after_tag + 1 + insert.length(); 
        } else {
            scan_begin = line_after_tag + 1 + insert.length(); 
        }
    }

    std::ofstream file_output(child);
    file_output << file_input_str;
}
std::string erase_block(const std::string& filepath, const std::string& tag) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filepath << '\n';
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::istringstream stream(content);
    std::ostringstream output;
    std::string line;
    bool skip_next = false;

    while (std::getline(stream, line)) {
        if (line.find(tag) != std::string::npos) {
            skip_next = true; 
            continue;
        }

        if (skip_next) {
            skip_next = false;
            continue; 
        }

        output << line << '\n';
    }

    return output.str();
}

int main() {
    std::fstream syntax("syntax.list");
    std::vector<std::string> tag_list_vect;
    std::vector<std::string> child_file_list_vect;
    std::string child_file_list;
    std::string tag_list;
    std::vector<std::string> dir= dir_listing();
    if (syntax.is_open()) {
        while (std::getline(syntax, tag_list)) {
            tag_list_vect.push_back(tag_list);
        }
    } else {
        std::cerr << "File may not exist: syntax.list" << std::endl;
        return 1;
    }
    std::string content;
    for (const auto& for_tag : tag_list_vect) {
        copy_block("public/layout.html", for_tag);
        for(const auto &file_list : dir){
              insert_block(file_list, for_tag);
              content = erase_block(file_list,for_tag);
              std::cout << content << std::endl;
                std::ofstream output(file_list+".htpp");
                output << content;
        }
      
    }
  
    

    return 0;
}
