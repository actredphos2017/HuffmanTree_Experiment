#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

struct HTree_Node{
        char data;
        int weight;
        int parent;
        int lchild;
        int rchild;
};

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

class Huffman{
    private:
        vector<HTree_Node> tree;
        map<char, string> code;
    public:
        void BuildTree(map<char, int> &char_weight){
            srand(time(NULL));
            for(auto &c : char_weight)
                tree.push_back(HTree_Node{c.first, c.second, -1, -1, -1});
            list<NodeType> l = Get(char_weight);
            vector<char> v;
            for(auto &c : char_weight)
                v.push_back(c.first);
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
        void BuildCode(){
            list<HTree_Node> l;
            l.push_back(tree.back());
            while(!l.empty()){
                if(l.front().lchild != -1){
                    l.push_back(tree[l.front().lchild]);
                    code[tree[l.front().lchild].data] = code[l.front().data] + "0";
                }
                if(l.front().rchild != -1){
                    l.push_back(tree[l.front().rchild]);
                    code[tree[l.front().rchild].data] = code[l.front().data] + "1";
                }
                l.pop_front();
            }
        }
        void Code_Output(){
            for(auto &c : this->code)
                cout << c.first << " " << c.second << endl;
        }
        void Tree_Output(){
            for(auto &c : this->tree)
                cout << c.data << " " << c.weight << " " << c.parent << " " << c.lchild << " " << c.rchild << endl;
        }
        string GetCode(char c){
            return this->code[c];
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

        void BuildFullTree(vector<HTree_Node> &Tree, int loc, int depth){
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
            BuildFullTree(Tree, Tree[loc].lchild, depth - 1);
            BuildFullTree(Tree, Tree[loc].rchild, depth - 1);
        }

        vector<HTree_Node> Full_Level_Tree(){
            vector<HTree_Node> copy_tree = this->tree;
            BuildFullTree(copy_tree, copy_tree.size() - 1, this->Depth());
            cout << this->Depth() << endl;
            for(auto i : copy_tree)
                cout << i.data << " " << i.weight << " " << i.parent << " " << i.lchild << " " << i.rchild << endl;
            vector<HTree_Node> res;
            cout << "Here" << endl;
            res.push_back(this->tree.back());
            list<HTree_Node> l;
            l.push_back(copy_tree.back());
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
            for(auto &c : res)
                cout << c.data << " " << c.weight << " " << c.parent << " " << c.lchild << " " << c.rchild << endl;
            return res;
        }

        //void VisualPrint();
};

/*


   B:60
   / \
C:20  D:40

*/