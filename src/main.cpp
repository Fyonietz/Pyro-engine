#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Reads content between @@start and @@end from input_file
std::string extract_block(const std::string& input_file, const std::string& startTag, const std::string& endTag) {
    std::ifstream file(input_file);
    if (!file) {
        std::cerr << "Failed to open " << input_file << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    size_t start_pos = content.find(startTag);
    size_t end_pos = content.find(endTag, start_pos);

    if (start_pos == std::string::npos || end_pos == std::string::npos) {
        std::cerr << "Tags not found in " << input_file << "\n";
        return "";
    }

    start_pos += startTag.length();
    return content.substr(start_pos, end_pos - start_pos);
}

void replace_all_blocks_safe(
    const std::string& output_file,
    const std::string& insert_block,
    const std::string& startTag,
    const std::string& endTag
) {
    std::ifstream file(output_file);
    if (!file) {
        std::cerr << "Failed to open " << output_file << " for reading.\n";
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    size_t search_pos = 0;
    int replaced_count = 0;

    while (true) {
        size_t start_pos = content.find(startTag, search_pos);
        if (start_pos == std::string::npos)
            break;

        size_t end_pos = content.find(endTag, start_pos + startTag.length());
        if (end_pos == std::string::npos) {
            std::cerr << "Warning: unmatched " << startTag << " found at position " << start_pos << "\n";
            break; // or `search_pos = start_pos + startTag.length(); continue;` to skip and keep going
        }

        // Replace between tags
        size_t insert_start = start_pos + startTag.length();
        content.replace(insert_start, end_pos - insert_start, "\n" + insert_block + "\n");

        search_pos = end_pos + endTag.length(); // continue after end tag
        replaced_count++;
    }

    std::ofstream out(output_file);
    if (!out) {
        std::cerr << "Failed to open " << output_file << " for writing.\n";
        return;
    }

    out << content;
    out.close();

    std::cout << "Replaced " << replaced_count << " block(s) safely.\n";
}

int main() {
    std::string insert_block = extract_block("index.html", "@@start", "@@end");

    if (insert_block.empty()) {
        std::cerr << "No block found to insert.\n";
        return 1;
    }

    replace_all_blocks_safe("output.html", insert_block, "@start", "@start_end");
    return 0;
}
