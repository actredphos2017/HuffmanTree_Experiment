#pragma once

std::string filename(std::string path) {
    int i = path.length() - 1;
    while (path[i] != '/') {
        i--;
    }
    return path.substr(i + 1);
}

std::string filedir(std::string path) {
    int i = path.length() - 1;
    while (path[i] != '/') {
        i--;
    }
    return path.substr(0, i + 1);
}

std::string filenamewithoutext(std::string path) {
    int i = path.length() - 1;
    while (path[i] != '.') {
        i--;
    }
    return path.substr(0, i);
}

double string_to_double(string str){
    double x = 0;
    int i = 0;
    while(str[i] != '.'){
        x = x * 10 + str[i] - '0';
        i ++;
    }
    i ++;
    double y = 0;
    while(i < str.length()){
        y = y * 10 + str[i] - '0';
        i ++;
    }
    return x + y / pow(10, str.length() - i);
}

Huffman design_code(std::string path){
    std::string fn = filename(path);
    std::cout << "File name: " << fn << std::endl;
    std::ifstream iF(path);
    std::map<char, double> char_weight;
    for(std::string line; std::getline(iF, line); )
        if(line.substr(0,2) == "\\n")
            char_weight['\n'] == string_to_double(line.c_str() + 3);
        else
            char_weight[line[0]] = string_to_double(line.c_str() + 2);
    iF.close();
    Huffman huffman;
    huffman.BuildTree(char_weight);
    huffman.BuildCode(char_weight);
    return huffman;
}

Huffman build_code(std::string path){
    std::string fn = filename(path);
    std::cout << "File name: " << fn << std::endl;
    std::ifstream iF(path);
    std::vector<std::string> lines;
    for(std::string line; std::getline(iF, line); )
        lines.push_back(line);
    iF.close();
    std::map<char, int> char_weight;
    for(auto &line : lines){
        for(auto &c : line)
            char_weight[c] ++;
        char_weight['\n'] ++;
    }
    Huffman huffman;
    huffman.BuildTree(char_weight);
    huffman.BuildCode(char_weight);
    return huffman;
}