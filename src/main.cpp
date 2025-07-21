#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
    std::ifstream file("index.html");
    if (!file) {
        std::cerr << "Could not open the file.\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    std::ofstream output("output.html");

    const std::string startTag = "@menu\n";
    const std::string endTag = "@end";
    const std::string block;
    int count=0;
    size_t startPos = 0;
    while ((startPos = content.find(startTag, startPos)) != std::string::npos) {
        startPos += startTag.length();
        size_t endPos = content.find(endTag, startPos);
        if (endPos == std::string::npos) break;

        std::string block = content.substr(startPos, endPos - startPos);

      
         output  << block;
        startPos = endPos + endTag.length(); // move past this block
    }
       

    return 0;   
}
