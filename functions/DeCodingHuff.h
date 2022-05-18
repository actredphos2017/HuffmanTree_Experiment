#pragma once

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>

using namespace std;

bool decoding(string huffpath, string filepath, string ofname){
    ifstream iF(huffpath);
    map<string, char> huffcode;
    int longest = 0;
    for(string line; getline(iF, line); ){
        string code;
        char c;
        if(line.find(" ") == line.size() - 1){
            code = line;
            code.pop_back();
            c = '\n';
            getline(iF, line);
        }
        else{
            c = line[line.find(" ") + 1];
            code = line.substr(0, line.find(" "));
        }
        cout << c << " " << code << endl;
        huffcode[code] = c;
        if(code.size() > longest)
            longest = code.size();
    }
    iF.close();
    ifstream iF2(filepath, ios::binary);
    ofstream oF(ofname);
    for(string line; getline(iF2, line); ){
        string code = "";
        for(auto &c : line){
            code += c;
            if(huffcode.find(code) != huffcode.end()){
                oF << huffcode[code];
                code = "";
            }
            if(code.size() > longest)
                return false;
        }
    }
    iF2.close();
    oF.close();
    return true;
}