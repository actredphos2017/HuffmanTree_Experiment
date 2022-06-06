#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../head/Huffman.h"

using namespace std;

string filename(string path) {
    int i = path.length() - 1;
    while (path[i] != '/') {
        i--;
    }
    return path.substr(i + 1);
}

string filedir(string path) {
    int i = path.length() - 1;
    while (path[i] != '/') {
        i--;
    }
    return path.substr(0, i + 1);
}

string filenamewithoutext(string path) {
    int i = path.length() - 1;
    while (path[i] != '.') {
        i--;
    }
    return path.substr(0, i);
}

Huffman design_code(string path){
    string fn = filename(path);
    cout << "File name: " << fn << endl;
    ifstream iF(path);
    map<char, int> char_weight;
    for(string line; getline(iF, line); )
        if(line.substr(0,2) == "\\n")
            char_weight['\n'] == atoi(line.c_str() + 3);
        else
            char_weight[line[0]] = atoi(line.c_str() + 2);
    iF.close();
    Huffman huffman;
    huffman.BuildTree(char_weight);
    huffman.BuildCode(char_weight);
    return huffman;
}

Huffman build_code(string path){
    string fn = filename(path);
    cout << "File name: " << fn << endl;
    ifstream iF(path);
    vector<string> lines;
    for(string line; getline(iF, line); )
        lines.push_back(line);
    iF.close();
    map<char, int> char_weight;
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