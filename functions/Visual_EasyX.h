#pragma once

#include <graphics.h>
#include <iostream>
#include "head/Huffman.h"
#include <vector>
#include <queue>
#define DESTENCE 50
#define HEIGTH 50
#define MAX_DES 1920
#define MAX_HEI 1080
#define SAVE_DES 100

using namespace std;

struct Circle{
    int x, y;
    HTree_Node Target_Node;
};

struct douC{
    Circle c[2];
};

typedef vector<Circle> circles;

struct Line{
    int x1, y1;
    int x2, y2;
    int weight;
};

typedef vector<Line> lines;

int _pow(int n, int m){
    int r = 1;
    while(m --)
        r *= n;
    return r;
}

void Visual_Print_EasyX(vector<HTree_Node> Full_Tree){
    int depth = 1;
    while(_pow(2, depth)-1 < Full_Tree.size())
        depth ++;
    if(_pow(2, depth)-1 != Full_Tree.size())
        exit(-1);
    if(2*SAVE_DES + DESTENCE*(_pow(2,depth-1)-1) > MAX_DES || 2*SAVE_DES + HEIGTH*(depth-1) > MAX_HEI){
        std::cout << "This graph is too big!" << endl;
        return;
    }
    auto it = Full_Tree.begin();
    circles cs;
    lines ls;
    queue<Circle> q;
    douC dc;
    for(int i = 0; i < depth; i ++){
        int y = i * HEIGTH;
        int x = (_pow(2, depth-i-1)-1) * DESTENCE / 2;
        for(int j = 0; j < _pow(2, i); j ++, x += _pow(2, depth-i-2)*DESTENCE){
            Circle c = {x, y, *(it ++)};
            cs.push_back(c);
            q.push(c);
            dc.c[j%2] = c;
            if(j%2 == 1){
                Circle headc = q.front();
                q.pop();
                ls.push_back(Line{headc.x, headc.y, dc.c[0].x, dc.c[0].y, dc.c[0].Target_Node.weight});
                ls.push_back(Line{headc.x, headc.y, dc.c[1].x, dc.c[1].y, dc.c[1].Target_Node.weight});
            }
        }
    }
    initgraph(2*SAVE_DES + DESTENCE*(_pow(2,depth-1)-1), 2*SAVE_DES + HEIGTH*(depth-1));
    setbkcolor(WHITE);
    setlinecolor(BLACK);
    setfillcolor(WHITE);
    for(auto it : ls)
        line(it.x1,it.y1,it.x2,it.y2);
    for(auto it : cs)
        fillcircle(it.x, it.y, 25);
}