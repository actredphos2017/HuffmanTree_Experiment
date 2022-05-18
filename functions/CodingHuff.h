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

Huffman coding(string path){
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
    huffman.BuildCode();
    huffman.Code_Output();
    string ofname = filedir(path) + filenamewithoutext(fn) + ".huff";
    cout << "Output file name: " << ofname << endl;
    ofstream oF(ofname, ios::binary);
    bool flag = true;
    for(auto &line : lines){
        if(!flag)
            oF << huffman.GetCode('\n');
        else
            flag = false;
        for(auto &c : line)
            oF << huffman.GetCode(c);
    }
    oF.close();
    string _ofname = filedir(path) + filenamewithoutext(fn) + ".huff.txt";
    ofstream _oF(_ofname, ios::binary);
    huffman.Hfmtree_Output(_oF, char_weight);
    _oF.close();
    return huffman;
}