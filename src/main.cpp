#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

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

    std::string insert = copy_block("layout.html", tag);
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



int main() {
    std::fstream syntax("syntax.list");

    std::vector<std::string> tag_list_vect;
    std::string tag_list;

    if (syntax.is_open()) {
        while (std::getline(syntax, tag_list)) {
            tag_list_vect.push_back(tag_list);
        }
    } else {
        std::cerr << "File may not exist: syntax.list" << std::endl;
        return 1;
    }

    for (const auto& for_tag : tag_list_vect) {
        std::cout << copy_block("layout.html", for_tag) << std::endl;

        insert_block("child.html", for_tag);
    }

    return 0;
}
