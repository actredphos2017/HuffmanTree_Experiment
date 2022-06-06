#pragma once
#define SEP ":"

struct HTree_Node{
        char data;
        int weight;
        int parent;
        int lchild;
        int rchild;
};

typedef std::vector<HTree_Node> HTreeType;

struct NodeType{
        int no;
        char data;
        int weight;
};

std::list<NodeType> Get(std::map<char, int> char_weight){
    std::list<NodeType> ht;
    for(auto &c : char_weight){
        NodeType node;
        node.no = ht.size();
        node.data = c.first;
        node.weight = c.second;
        ht.push_back(node);
    }
    return ht;
}

NodeType push_min(std::list<NodeType>& hl){
    NodeType min;
    std::list<NodeType>::iterator min_loc = hl.begin();
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

bool ifRepeat(char c, std::vector<char> v){
    for(auto i : v)
        if(i == c)
            return true;
    return false;
}

void filful_str(std::string &str, int n){
    while(str.size() < n)
        str += " ";
}

std::string ctos(char c){
    std::string str = "";
    str += c;
    return str;
}

std::string data_put(HTree_Node htn, std::vector<char> mcode){
    if(htn.data == '\0')
        return " ";
    if(htn.data == '\n')
        return std::string("\\n") + SEP + std::to_string(htn.weight);
    if(std::find(mcode.begin(), mcode.end(), htn.data) != mcode.end())
        return ctos(htn.data) + SEP + std::to_string(htn.weight);
    return ctos('^') + SEP + std::to_string(htn.weight);
}

class Huffman{
    public:
        HTreeType tree;
        std::map<char, std::string> code;
        std::vector<char> cmap;
    public:
        void BuildTree(std::map<char, int> &char_weight){
            srand(time(NULL));
            for(auto &c : char_weight)
                tree.push_back(HTree_Node{c.first, c.second, -1, -1, -1});
            std::list<NodeType> l = Get(char_weight);
            std::vector<char> v;
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

        void BuildCode(std::map<char, int> &char_weight){
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

        void Hfmtree_Output(std::ofstream &oF, std::map<char, int> &char_weight){
            for(auto &c : this->code)
                if(char_weight.find(c.first) != char_weight.end())
                    oF << c.second << " " << c.first << std::endl;
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

        std::vector<HTree_Node> Full_Level_Tree(){
            HTreeType copy_tree = this->tree;
            BuildFullTree(copy_tree, copy_tree.size() - 1, this->Depth());
            std::vector<char> mainc;
            std::vector<HTree_Node> res;
            res.push_back(this->tree.back());
            std::list<HTree_Node> l;
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
            std::vector<HTree_Node> full_tree = this->Full_Level_Tree();
            auto it = full_tree.rbegin();
            std::vector<std::string> lines;
            std::list<int> l;
            std::list<bool> has_value;
            for(int k = this->Depth() - 1; k >= 0; k --){
                if(k == this->Depth() - 1){
                    std::string temp = "";
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
                    std::string temp = "";
                    int flag = 0;
                    std::list<int> _l = l;
                    l.clear();
                    while(!_l.empty()){
                        has_value.push_back(it->data != '\0');
                        filful_str(temp, _l.front());
                        _l.pop_front();
                        std::string str = data_put(*it, this->cmap);
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
                    std::string temp = "";
                    std::list<int> _l = l;
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
            std::cout << std::endl;
            std::cout << "==========================================" << std::endl;
            std::cout << "         Visual Huffman Tree" << std::endl << std::endl;
            for(auto rit = lines.rbegin(); rit != lines.rend(); rit ++)
                std::cout << *rit << std::endl;
            std::cout << std::endl;
            std::cout << "==========================================" << std::endl;
        }
};