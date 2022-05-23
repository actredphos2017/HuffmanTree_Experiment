#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>

#define SEP ":"

using namespace std;

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
    private:
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
        void clear(){
            this->code.clear();
            this->cmap.clear();
            this->tree.clear();
        }

        map<char, string> GetCode(){
            return this->code;
        }

        vector<char> GetCharMap(){
            return this->cmap;
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