//
// Created by barth on 19/09/2022.
//

#include <string>
#include <fstream>
#include <iostream>
#include "utils.h"

void loadFileToBuffer(const char *filename, std::string &stringBuffer) {
    stringBuffer = ""; // empty buffer first
    std::ifstream reader(filename);
    if (reader.is_open()) {
        std::string lineBuffer;
        while (std::getline(reader, lineBuffer)) {
            stringBuffer += lineBuffer + "\n";
        }
        reader.close();
    } else std::cerr << "Cloud not open " << filename << std::endl;
}