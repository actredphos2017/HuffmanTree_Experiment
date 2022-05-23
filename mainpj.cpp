#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "functions/DesignHuff.h"

using namespace std;

string file_name[3];
char ch;

bool huffman_tree_exist = false;
Huffman huff_tree;

bool huffman_code_exist = false;
map<char, string> huff_code;
map<string, char> inverse_huff_code(){
    map<string, char> res;
    for(auto &c : huff_code)
        res[c.second] = c.first;
    return res;
}

bool isFileExists_ifstream(string name) {
    ifstream f(name.c_str());
    bool res = f.good();
    f.close();
    return res;
}

void head_print(){
    system("cls");
    cout << "======= Huffman Tree Status =======" << endl;
    
    cout << "Target Code File: ";
    if(file_name[0].size() == 0)
        cout << "NULL" << endl;
    else
        cout << file_name[0] << endl;
    cout << "Target Origin File: ";
    if(file_name[1].size() == 0)
        cout << "NULL" << endl;
    else
        cout << file_name[1] << endl;
    cout << "Huffman Tree Status: ";
    if(huffman_tree_exist)
        cout << "Existed" << endl;
    else
        cout << "Unexisted" << endl;
    cout << "Huffman Code Status: ";
    if(huffman_code_exist)
        cout << "Existed" << endl;
    else
        cout << "Unexisted" << endl;
    cout << endl;
    return;
}

bool bool_cheak(char c, bool &b){
    if(c == 'Y' || c == 'y'){
        b = true;
        return true;
    }
    if(c == 'N' || c == 'n'){
        b = false;
        return true;
    }
    return false;
}

int main(){
    while(1){
        head_print();
        cout << "Menu:" << endl << endl;

        cout << " About Code File:" << endl;
        cout << "    1. Switch code file in local" << endl;
        cout << "    2. Design code from char-weight file" << endl;
        cout << "    3. Enter temporary code file into Terminal" << endl;
        cout << "    4. Build code file from origin file" << endl << endl;

        cout << " About Origin File:" << endl;
        cout << "    A. Switch origin file to coding/decoding" << endl;
        cout << "    B. Coding origin file with existed huffman code" << endl;
        cout << "    C. Decoding origin file with existed huffman code" << endl << endl;

        cout << " About Huffman:" << endl;
        cout << "    a. Visual print huffman tree" << endl;
        cout << "    b. Output the huffman code" << endl;
        cout << "    c. Save huffman code in local" << endl << endl;

        cout << "    Q. Quit" << endl << endl;

        cout << "Enter your choice: ";
        cin >> ch;
        switch(ch){
            case '1':{ // Switch code file in local
                head_print();
                if(huffman_code_exist){
                    cout << "Huffman code has been existed!" << endl;
                    cout << "Do you want to destory it and continue? (Y/N) ";
                    char bool_ch = '#';
                    bool bool_res;
                    while(!bool_cheak(bool_ch, bool_res))
                        cin >> bool_ch;
                    if(bool_res)
                        huff_code.clear();
                    else
                        break;
                }
                head_print();
                cout << "Please enter the path of the code file (.huff.key)" << endl;
                cout << "  " << endl;
                getline(cin, file_name[0], '\n');
                while(!isFileExists_ifstream(file_name[0])){
                    cout << " Error! The file is not existed!" << endl;
                    cout << "Please enter again" << endl << "  " << endl;
                    getline(cin, file_name[0], '\n');
                }
                ifstream iF(file_name[0]);
                for(string line; getline(iF, line); )
                    if(line.substr(0,2) == "\\n")
                        huff_code['\n'] = line.substr(3);
                    else
                        huff_code[line[0]] = line.substr(2);
                iF.close();
                cout << "Done!" << endl;
                huffman_code_exist = true;
                cout << "Huffman code has been created!" << endl;
                system("pause");
            }break;

            case '2':{ // Design code from char-weight file
                head_print();
                if(huffman_tree_exist){
                    cout << "Huffman tree and code has been existed!" << endl;
                    cout << "Do you want to destory them and continue? (Y/N) ";
                    char bool_ch = '#';
                    bool bool_res;
                    while(!bool_cheak(bool_ch, bool_res))
                        cin >> bool_ch;
                    if(bool_res){
                        huff_code.clear();
                        huff_tree.clear();
                    }
                    else
                        break;
                }
                else if(huffman_code_exist){
                    cout << "Huffman code has been existed!" << endl;
                    cout << "Do you want to destory it and continue? (Y/N) ";
                    char bool_ch = '#';
                    bool bool_res;
                    while(!bool_cheak(bool_ch, bool_res))
                        cin >> bool_ch;
                    if(bool_res)
                        huff_code.clear();
                    else
                        break;
                }
                head_print();
                cout << "Please enter the path of the code-weight file (.txt)" << endl;
                cout << "The text file should be like this:" << endl;
                cout << "A 14" << endl;
                cout << "B 9" << endl;
                cout << "C 27" << endl;
                cout << "D 1" << endl;
                cout << "\\n 5                         <---- \"\\n\" means enter key" << endl;
                cout << "..." << endl << endl;
                cout << "  " << endl;
                getline(cin, file_name[2], '\n');
                while(!isFileExists_ifstream(file_name[2])){
                    cout << " Error! The file is not existed!" << endl;
                    cout << "Please enter again" << endl << "  " << endl;
                    getline(cin, file_name[2], '\n');
                }
                cout << "Now Checking..." << endl;
                huff_tree = design_code(file_name[2]);
                huff_code = huff_tree.GetCode();
                cout << "Huffman tree has been created!" << endl;
                cout << "Huffman code has been created!" << endl;
                huffman_tree_exist = true;
                huffman_code_exist = true;
                system("pause");
            }break;

            case '3':{ // Enter temporary code file into Terminal
                head_print();
                if(huffman_code_exist){
                    cout << "Huffman code has been existed!" << endl;
                    cout << "Do you want to destory it and continue? (Y/N) ";
                    char bool_ch = '#';
                    bool bool_res;
                    while(!bool_cheak(bool_ch, bool_res))
                        cin >> bool_ch;
                    if(bool_res)
                        huff_code.clear();
                    else
                        break;
                }
                int n;
                string line;
                head_print();
                cout << "Please enter the count of the code: ";
                cin >> n;
                cout << "Next type \"char code\" for " << n << " times" << endl;
                cout << "Enter key should be enter as \"\\n\"" << endl << endl;
                while(n --){
                    getline(cin, line, '\n');
                    if(line.substr(0, 2) == "\\n")
                        huff_code['\n'] = line.substr(3);
                    else
                        huff_code[line[0]] = line.substr(2);
                }
                cout << "Huffman code has been created!" << endl;
                huffman_code_exist = true;
                system("pause");
            }break;

            case '4':{ // Build code file from origin file
                head_print();
                if(file_name[1].size() == 0){
                    cout << " Error! The origin file path is not existed!" << endl;
                    system("pause");
                    break;
                }
                if(huffman_tree_exist){
                    cout << "Huffman tree and code has been existed!" << endl;
                    cout << "Do you want to destory them and continue? (Y/N) ";
                    char bool_ch = '#';
                    bool bool_res;
                    while(!bool_cheak(bool_ch, bool_res))
                        cin >> bool_ch;
                    if(bool_res){
                        huff_code.clear();
                        huff_tree.clear();
                    }
                    else
                        break;
                }
                else if(huffman_code_exist){
                    cout << "Huffman code has been existed!" << endl;
                    cout << "Do you want to destory it and continue? (Y/N) ";
                    char bool_ch = '#';
                    bool bool_res;
                    while(!bool_cheak(bool_ch, bool_res))
                        cin >> bool_ch;
                    if(bool_res)
                        huff_code.clear();
                    else
                        break;
                }
                head_print();
                cout << "Now building from origin file, please wait..." << endl;
                huff_tree = build_code(file_name[1]);
                huff_code = huff_tree.GetCode();
                cout << "Huffman Tree has been created!" << endl;
                cout << "Huffman Code has been created!" << endl;
                huffman_tree_exist = true;
                huffman_code_exist = true;
                system("pause");
            }break;

            case 'A':{ // Switch origin file to coding/decoding
                head_print();
                cout << "Please enter the path of the file" << endl;
                cout << "  " << endl;
                getline(cin, file_name[1], '\n');
                while(!isFileExists_ifstream(file_name[1])){
                    cout << " Error! The file is not existed!" << endl;
                    cout << "Please enter again" << endl << "  " << endl;
                    getline(cin, file_name[1], '\n');
                }
                cout << "The origin file is targeted" << endl;
                system("pause");
            }break;

            case 'B':{ // Coding origin file with existed huffman code
                if(!huffman_code_exist){
                    cout << "Error! The huffman code is not existed!" << endl;
                    system("pause");
                    break;
                }
                if(file_name[1].size() == 0){
                    cout << "Error! The origin file path is not existed!" << endl;
                    system("pause");
                    break;
                }
                string ofname = string("output/") + filenamewithoutext(filename(file_name[1])) + ".codefile";
                int temp_name = 1;
                if(isFileExists_ifstream(ofname)){
                    ofname = filenamewithoutext(ofname);
                    while(isFileExists_ifstream(ofname + "(" + to_string(temp_name) + ").codefile"))
                        temp_name ++;
                    ofname = ofname + "(" + to_string(temp_name) + ").codefile";
                }
                cout << "Origin file name: " << filename(file_name[1]) << endl;
                ifstream iF(file_name[1]);
                vector<string> lines;
                for(string line; getline(iF, line); )
                    lines.push_back(line);
                iF.close();
                ofstream oF(ofname, ios::binary);
                bool flag = true;
                for(auto &line : lines){
                    if(!flag)
                        oF << huff_code['\n'];
                    else
                        flag = false;
                    for(auto &c : line)
                        oF << huff_code[c];
                }
                oF.close();
                cout << "Output file path: " << ofname << endl;
                cout << "Finish!" << endl;
                system("pause");
            }break;

            case 'C':{ // Decoding origin file with existed huffman code
                if(!huffman_code_exist){
                    cout << "Error! The huffman code is not existed!" << endl;
                    system("pause");
                    break;
                }
                if(file_name[1].size() == 0){
                    cout << "Error! The origin file path is not existed!" << endl;
                    system("pause");
                    break;
                }
                string ofname = string("output/") + filenamewithoutext(filename(file_name[1])) + ".txt";
                int temp_name = 1;
                if(isFileExists_ifstream(ofname)){
                    ofname = filenamewithoutext(ofname);
                    while(isFileExists_ifstream(ofname + "(" + to_string(temp_name) + ").txt"))
                        temp_name ++;
                    ofname = ofname + "(" + to_string(temp_name) + ").txt";
                }
                ofstream oF(ofname);
                ifstream iF(file_name[1]);
                map<string, char> inverse_code = inverse_huff_code();
                for(string line; getline(iF, line); ){
                    string code = "";
                    for(auto &c : line){
                        code += c;
                        if(inverse_code.find(code) != inverse_code.end()){
                            oF << inverse_code[code];
                            code = "";
                        }
                    }
                }
                oF.close();
                cout << "Origin file name: " << filename(file_name[1]) << endl;
                cout << "Output file path: " << ofname << endl;
                cout << "Finish!" << endl;
                system("pause");
            }break;

            case 'a':{ // Visual print huffman tree
                if(!huffman_tree_exist){
                    cout << "Error! The huffman tree is not existed!" << endl;
                    system("pause");
                    break;
                }
                huff_tree.VisualPrint();
                system("pause");
            }break;

            case 'b':{ // Output the huffman code
                if(!huffman_code_exist){
                    cout << "Error! The huffman code is not existed!" << endl;
                    system("pause");
                    break;
                }
                cout << "======== Huffman Code ========" << endl;
                cout << "  Char     Code" << endl;
                for(auto &c : huff_code)
                    if(c.first == '\n')
                        cout << "  \\n  " << c.second << endl;
                    else
                        cout << "  " << c.first << "  " << c.second << endl;
                cout << "==============================" << endl;
                system("pause");
            }break;

            case 'c':{ // Save huffman code in local
                if(!huffman_code_exist){
                    cout << "Error! The huffman code is not existed!" << endl;
                    system("pause");
                    break;
                }
                string ofname = "output/CodeKeys.huffkey";
                int temp_name = 1;
                if(isFileExists_ifstream(ofname)){
                    ofname = filenamewithoutext(ofname);
                    while(isFileExists_ifstream(ofname + "(" + to_string(temp_name) + ").huffkey"))
                        temp_name ++;
                    ofname = ofname + "(" + to_string(temp_name) + ").huffkey";
                }
                ofstream oF(ofname);
                for(auto &c : huff_code)
                    if(c.first == '\n')
                        oF << "\\n " << c.second << endl;
                    else
                        oF << c.first << " " << c.second << endl;
                oF.close();
                cout << "Output file path: " << ofname << endl;
                cout << "Finish!" << endl;
                system("pause");
            }break;

            case 'q':
            case 'Q':{
                system("pause");
                return 0;
            }

            default:{
                cout << "Error Code! Please enter again!" << endl;
                system("pause");
            }break;
        }
    }
    system("pause");
    return 0;
}