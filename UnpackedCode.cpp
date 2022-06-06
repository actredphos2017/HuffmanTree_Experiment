#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <ctime>
#include <cmath>
#include <list>
#include <algorithm>

#define SEP ":"

using namespace std;

bool have_decimal(double d){
    return d - (int)d != 0;
}

int count_of_decimal_digits(double x){
    int count = 0;
    while(have_decimal(x)){
        x *= 10;
        count ++;
    }
    return count;
}

void Binary_Output(string file_name, string binary_text){
    ofstream oF(file_name, ios_base::binary);
    int bad_count = 8 - binary_text.size() % 8;
    string bad_code = "";
    while(-- bad_count)
        bad_code += '0';
    bad_code += '1';
    binary_text = bad_code + binary_text;
    uint8_t single[8];
    unsigned int n = 1;
    for(auto it = binary_text.begin(); it != binary_text.end(); it ++, n ++){
        single[n-1] = (uint8_t)(*it - '0' == 1);
        if(n == 8){
            uint8_t temp = 0x00;
            for(int i = 0; i < 8; i ++)
                temp |= single[i] << (7 - i);
            oF.write((char*)&temp, sizeof(uint8_t));
            n = 0;
        }
    }
    oF.close();
    return;
}

string Binary_Input(string file_name){
    ifstream iF(file_name);
    iF.seekg(0, ios::end);
    size_t length = iF.tellg();
    iF.seekg(0, ios::beg);
    uint8_t* data = new uint8_t[length];
    iF.read((char*)data, length * sizeof(uint8_t));
    iF.close();
    string binary_text = "";
    for(int i = 0; i < length; i ++)
        for(int j = 7; j >= 0; j --)
            binary_text += ('0' + ((data[i] >> j) % 2));
    for(int i = 0; i < 8; i ++)
        if(binary_text[i] == '1')
            return binary_text.substr(++ i, string::npos);
    return binary_text;
}

struct HTree_Node{
        char data;
        int weight;
        int parent;
        int lchild;
        int rchild;
};

typedef vector<HTree_Node> HTreeType;

struct NodeType{
        int no;
        char data;
        int weight;
};

list<NodeType> Get(map<char, int> char_weight){
    list<NodeType> ht;
    for(auto &c : char_weight){
        NodeType node;
        node.no = ht.size();
        node.data = c.first;
        node.weight = c.second;
        ht.push_back(node);
    }
    return ht;
}

NodeType push_min(list<NodeType>& hl){
    NodeType min;
    list<NodeType>::iterator min_loc = hl.begin();
    for(auto i = hl.begin(); i != hl.end(); i ++){
        if(i == hl.begin()){
            min = *i;
            min_loc = i;
        }
        else if(i->weight < min.weight){
            min = *i;
            min_loc = i;
        }
    }
    hl.erase(min_loc);
    return min;
}

char random_char(){
    char c = '\0';
    while(c == '\0')
        c = rand() % 127;
    return c;
}

bool ifRepeat(char c, vector<char> v){
    for(auto i : v)
        if(i == c)
            return true;
    return false;
}

void filful_str(string &str, int n){
    while(str.size() < n)
        str += " ";
}

string ctos(char c){
    string str = "";
    str += c;
    return str;
}

string data_put(HTree_Node htn, vector<char> mcode){
    if(htn.data == '\0')
        return " ";
    if(htn.data == '\n')
        return string("\\n") + SEP + to_string(htn.weight);
    if(find(mcode.begin(), mcode.end(), htn.data) != mcode.end())
        return ctos(htn.data) + SEP + to_string(htn.weight);
    return ctos('^') + SEP + to_string(htn.weight);
}

class Huffman{
    public:
        HTreeType tree;
        map<char, string> code;
        vector<char> cmap;
    public:
        void BuildTree(map<char, int> &char_weight){
            srand(time(NULL));
            for(auto &c : char_weight)
                tree.push_back(HTree_Node{c.first, c.second, -1, -1, -1});
            list<NodeType> l = Get(char_weight);
            vector<char> v;
            v.push_back('\0');
            for(auto &c : char_weight){
                v.push_back(c.first);
                this->cmap.push_back(c.first);
            }
            while(l.size() > 1){
                NodeType min[2];
                min[0] = push_min(l);
                min[1] = push_min(l);
                char temp = random_char();
                while(ifRepeat(temp,v))
                    temp = random_char();
                v.push_back(temp);
                tree.push_back(HTree_Node{temp, min[0].weight + min[1].weight, -1, min[0].no, min[1].no});
                tree[min[0].no].parent = tree.size() - 1;
                tree[min[1].no].parent = tree.size() - 1;
                l.push_back(NodeType{(int)tree.size() - 1, temp, min[0].weight + min[1].weight});
            }
        }

        void BuildCode(map<char, int> &char_weight){
            for(int i = 0; i < char_weight.size(); i ++){
                code[tree[i].data] = "";
                int p = i;
                while(tree[p].parent != -1){
                    if(p == tree[tree[p].parent].lchild)
                        code[tree[i].data] = "0" + code[tree[i].data];
                    else
                        code[tree[i].data] = "1" + code[tree[i].data];
                    p = tree[p].parent;
                }
            }
        }

        void BuildCode(size_t n){
            for(int i = 0; i < n; i ++){
                code[tree[i].data] = "";
                int p = i;
                while(tree[p].parent != -1){
                    if(p == tree[tree[p].parent].lchild)
                        code[tree[i].data] = "0" + code[tree[i].data];
                    else
                        code[tree[i].data] = "1" + code[tree[i].data];
                    p = tree[p].parent;
                }
            }
        }

        void clear(){
            this->code.clear();
            this->cmap.clear();
            this->tree.clear();
        }

        void Hfmtree_Output(ofstream &oF, map<char, int> &char_weight){
            for(auto &c : this->code)
                if(char_weight.find(c.first) != char_weight.end())
                    oF << c.second << " " << c.first << endl;
        }
        
        int Depth(){
            int max = 0;
            for(auto &line : this->code)
                if(line.second.size() > max)
                    max = line.second.size();
            return max + 1;
        }

        void BuildFullTree(HTreeType &Tree, int loc, int depth){
            depth --;
            if(depth == 0)
                return;
            if(Tree[loc].lchild == -1){
                Tree[loc].lchild = Tree.size();
                Tree.push_back(HTree_Node{'\0', 0, loc, -1, -1});
            }
            if(Tree[loc].rchild == -1){
                Tree[loc].rchild = Tree.size();
                Tree.push_back(HTree_Node{'\0', 0, loc, -1, -1});
            }
            BuildFullTree(Tree, Tree[loc].lchild, depth);
            BuildFullTree(Tree, Tree[loc].rchild, depth);
            return;
        }

        vector<HTree_Node> Full_Level_Tree(){
            HTreeType copy_tree = this->tree;
            BuildFullTree(copy_tree, copy_tree.size() - 1, this->Depth());
            vector<char> mainc;
            vector<HTree_Node> res;
            res.push_back(this->tree.back());
            list<HTree_Node> l;
            l.push_back(copy_tree[tree.size()-1]);
            while(!l.empty()){
                if(l.front().lchild != -1){
                    l.push_back(copy_tree[l.front().lchild]);
                    res.push_back(copy_tree[l.front().lchild]);
                }
                if(l.front().rchild != -1){
                    l.push_back(copy_tree[l.front().rchild]);
                    res.push_back(copy_tree[l.front().rchild]);
                }
                l.pop_front();
            }
            return res;
        }

        void VisualPrint(){
            int t;
            vector<HTree_Node> full_tree = this->Full_Level_Tree();
            auto it = full_tree.rbegin();
            vector<string> lines;
            list<int> l;
            list<bool> has_value;
            for(int k = this->Depth() - 1; k >= 0; k --){
                if(k == this->Depth() - 1){
                    string temp = "";
                    int flag = 0;
                    for(int i = 0; i < (int)pow(2, k); i ++){
                        has_value.push_back(it->data != '\0');
                        temp += data_put(*(it ++), this->cmap);
                        flag ++;
                        if(flag % 2 == 1)
                            l.push_back(temp.size()-1);
                        if(i != (int)pow(2, (this->Depth()-1)) - 1)
                            temp += "  ";
                    }
                    lines.push_back(temp);
                }else{
                    string temp = "";
                    int flag = 0;
                    list<int> _l = l;
                    l.clear();
                    while(!_l.empty()){
                        has_value.push_back(it->data != '\0');
                        filful_str(temp, _l.front());
                        _l.pop_front();
                        string str = data_put(*it, this->cmap);
                        temp += str;
                        flag ++;
                        if(flag % 2 == 0)
                            l.push_back((temp.size()-str.size()-t)/2+t-1);
                        else
                            t = temp.size();
                        it ++;
                    }
                    lines.push_back(temp);
                }
                if(k != 0){
                    string temp = "";
                    list<int> _l = l;
                    while(!_l.empty()){
                        filful_str(temp, _l.front());
                        _l.pop_front();
                        temp += has_value.front() ? "/ " : "  ";
                        has_value.pop_front();
                        temp += has_value.front() ? "\\" : " ";
                        has_value.pop_front();
                    }
                    lines.push_back(temp);
                }
            }
            cout << endl;
            cout << "==========================================" << endl;
            cout << "         Visual Huffman Tree" << endl << endl;
            for(auto rit = lines.rbegin(); rit != lines.rend(); rit ++)
                cout << *rit << endl;
            cout << endl;
            cout << "==========================================" << endl;
        }
};


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
    cout << "======= System Status =======" << endl;
    
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
        cout << "    c. Save huffman code in local" << endl;
        cout << "    d. Save huffman tree in local" << endl;
        cout << "    e. Load huffman tree in local" << endl << endl;

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
                cin.get();
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
                cin.get();
                getline(cin, file_name[2], '\n');
                while(!isFileExists_ifstream(file_name[2])){
                    cout << " Error! The file is not existed!" << endl;
                    cout << "Please enter again" << endl << "  " << endl;
                    getline(cin, file_name[2], '\n');
                }
                cout << "Now Checking..." << endl;
                huff_tree = design_code(file_name[2]);
                huff_code = huff_tree.code;
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
                    cin.get();
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
                huff_code = huff_tree.code;
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
                cin.get();
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
                stringstream ss;
                bool flag = true;
                for(auto &line : lines){
                    if(!flag)
                        ss << huff_code['\n'];
                    else
                        flag = false;
                    for(auto &c : line)
                        ss << huff_code[c];
                }
                Binary_Output(ofname, ss.str());
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
                string temp_code = Binary_Input(file_name[1]);
                cout << temp_code << endl;
                map<string, char> inverse_code = inverse_huff_code();
                string code = "";
                for(auto &c : temp_code){
                    code += c;
                    if(inverse_code.find(code) != inverse_code.end()){
                        oF << inverse_code[code];
                        code = "";
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

            case 'd':{ // Save huffman tree in local
                if(!huffman_tree_exist){
                    cout << "Error! The huffman code is not existed!" << endl;
                    system("pause");
                    break;
                }
                string ofname = "output/HuffTree.hfmTree";
                int temp_name = 1;
                if(isFileExists_ifstream(ofname)){
                    ofname = filenamewithoutext(ofname);
                    while(isFileExists_ifstream(ofname + "(" + to_string(temp_name) + ").hfmTree"))
                        temp_name ++;
                    ofname = ofname + "(" + to_string(temp_name) + ").hfmTree";
                }
                ofstream oF(ofname);
                for(auto it : huff_tree.tree)
                    if(it.data == '\n')
                        oF << "\\n" << " " << it.weight << " " << it.parent << " " << it.lchild << " " << it.rchild << endl;
                    else
                        oF << it.data << " " << it.weight << " " << it.parent << " " << it.lchild << " " << it.rchild << endl;
                oF.close();
                cout << "Output file path: " << ofname << endl;
                cout << "Finish!" << endl;
                system("pause");
            }break;

            case 'e':{ // Load huffman tree in local
                head_print();
                cout << "Please enter the path of the file" << endl;
                cout << "  " << endl;
                cin.get();
                string ifname = "";
                getline(cin, ifname, '\n');
                while(!isFileExists_ifstream(ifname)){
                    cout << " Error! The file is not existed!" << endl;
                    cout << "Please enter again" << endl << "  " << endl;
                    getline(cin, ifname, '\n');
                }
                huff_tree.clear();
                ifstream iF(ifname);
                string temp_line;
                int n = 0;
                while(getline(iF, temp_line)){
                    stringstream ss(temp_line);
                    string temp_data, temp_weight, temp_parent, temp_lchild, temp_rchild;
                    getline(ss, temp_data, ' ');
                    getline(ss, temp_weight, ' ');
                    getline(ss, temp_parent, ' ');
                    getline(ss, temp_lchild, ' ');
                    getline(ss, temp_rchild, '\n');
                    cout << "  " << temp_data << " " << temp_weight << " " << temp_parent << " " << temp_lchild << " " << temp_rchild << endl;
                    if(temp_data == "\\n")
                        huff_tree.tree.push_back(HTree_Node{'\n', stoi(temp_weight), stoi(temp_parent), stoi(temp_lchild), stoi(temp_rchild)});
                    else
                        huff_tree.tree.push_back(HTree_Node{temp_data[0], stoi(temp_weight), stoi(temp_parent), stoi(temp_lchild), stoi(temp_rchild)});
                    n ++;
                }
                iF.close();
                for(int i = 0; i < (n + 1) / 2; i ++)
                    huff_tree.cmap.push_back(huff_tree.tree[i].data);
                cout << "Huffman tree has been created!" << endl;
                huffman_tree_exist = true;
                if(huffman_code_exist){
                    cout << "Warning! The huffman code is existed!" << endl;
                    cout << "Do you want to delete it then build? (y/n)" << endl;
                    char temp_char;
                    cin >> temp_char;
                    while(temp_char != 'y' && temp_char != 'n'){
                        cout << "Error! Please enter again" << endl;
                        cin >> temp_char;
                    }
                    if(temp_char == 'y'){
                        huff_code.clear();
                        huff_tree.BuildCode((n + 1) / 2);
                        cout << "Huffman code has been recreated!" << endl;
                    }
                }else{
                    huff_tree.BuildCode((n + 1) / 2);
                    cout << "Huffman code has been created!" << endl;
                    huffman_code_exist = true;
                }
                cout << "Finish!" << endl;
                system("pause");
                break;
            }

            case 'q': case 'Q':{
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