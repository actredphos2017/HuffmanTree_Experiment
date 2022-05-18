#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "functions/CodingHuff.h"
#include "functions/DeCodingHuff.h"

using namespace std;

int main(){
	Huffman hf = coding("input/ToBeTrean.txt");
	vector<HTree_Node> ad = hf.Full_Level_Tree();
	system("pause");
}