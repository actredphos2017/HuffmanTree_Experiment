#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <map>

using namespace std;

struct HTreeNode                //哈夫曼树结点类型
{
	char data;                    
	double weight;                   
	int parent;                    
	int lchild;                    
	int rchild;                    
};

void CreateHTree(vector<HTreeNode>& ht)
{
	int n = ht.size();
	ht.resize(2 * n);
	int m = 2 * n - 1;
	int i, j;
	int s1, s2, min;
	for (i = n;i < m; i++)
	{
		for (j = 0;j < i; j++)
			if (ht[j].parent == -1)
			{
				min = j;
				break;
			}
		for (j = min + 1;j < i; j++)
			if (ht[j].parent == -1 && ht[j].weight < ht[min].weight)
				min = j;
		s1 = min;
		for (j = 0;j < i; j++)
			if (ht[j].parent == -1 && j != s1)
			{
				min = j;
				break;
			}
		for (j = min + 1;j < i; j++)
			if (ht[j].parent == -1 && ht[j].weight < ht[min].weight && j != s1)
				min = j;
		s2 = min;
		ht[i].weight = ht[s1].weight + ht[s2].weight;
		ht[s1].parent = ht[s2].parent = i;
		ht[i].parent = -1;
		ht[i].lchild = s1;
		ht[i].rchild = s2;
	}

}

map<char, string> CreateHCode(vector<HTreeNode> ht) {
	map<char, string> htcode;
	int n = ht.size() / 2;
	for (int i = 0; i < n; i++) {
		string temp = "";
		int p = i;
		while (ht[p].parent != -1) {
			if (ht[ht[p].parent].lchild == p) {
				temp = "0" + temp;
			}
			else {
				temp = "1" + temp;
			}
			p = ht[p].parent;
		}
		htcode[ht[i].data] = temp;
	}
	return htcode;
}

map<string, char> Inverse_HCode(map<char, string> HCode){
	map<string, char> res;
	for(auto &c : HCode)
		res[c.second] = c.first;
	return res;
}

void show() {
	system("cls");
	cout << "------------- 哈夫曼树 -------------" << endl;
	cout << " 1. 终端输入字符及其频数（频率）后建树" << endl;
	cout << " 2. 读取文件获取字频构造哈夫曼树并编码" << endl;
	cout << " 3. 读取哈夫曼树" << endl;
	cout << " 4. 用存在的哈夫曼编码进行文件编码" << endl;
	cout << " 5. 对编码文件进行译码" << endl;
	cout << " Q. 退出" << endl;
	cout << "请选择: ";
}

double string_to_double(string str) {
	double res = 0;
	bool flag = false;
	int digical_count = 0;
	for (auto c : str) {
		if (c == '.') {
			flag = true;
			continue;
		}
		res = res * 10 + (int)(c - '0');
		if (flag) digical_count++;
	}
	res /= pow(10, digical_count);
	return res;
}

int main()
{
	char choice;
	map<char, string> hfmcode;vector<HTreeNode> ht;
	while (1) {
		show();
		cin >> choice;
		switch (choice) {
		case '1': {
			int n;
			cout << "输入字符个数：";
			cin >> n;
			string temp;
			char _data;
			double _weight;
			for (int i = 0; i < n; i++) {
				getline(cin, temp, '\n');
				if (temp.substr(0, 2) == "\\n") {
					_data = '\n';
					_weight = string_to_double(temp.substr(3));
				}
				else {
					_data = temp[0];
					_weight = string_to_double(temp.substr(2));
				}
				HTreeNode temp = { _data, _weight, -1, -1, -1 };
				ht.push_back(temp);
			}
			CreateHTree(ht);
			ofstream oF("output/hfmTree.txt");
			for (auto it : ht)
				if (it.data == '\n')
					oF << "\\n " << it.weight << " " << it.parent << " " << it.lchild << " " << it.rchild << endl;
				else
					oF << it.data << " " << it.weight << " " << it.parent << " " << it.lchild << " " << it.rchild << endl;
			oF.close();
			hfmcode = CreateHCode(ht);
			for (auto& c : hfmcode)
				cout << c.first << " " << c.second << endl;
			cout << "哈夫曼树已成功建立!" << endl;
		}break;

		case '2': {
			string filename;
			cout << "请输入字符权重文件的目录: ";
			cin >> filename;
			ifstream iF(filename);
			if (!iF.good()) {
				cout << "文件打开失败!" << endl;
				exit(0);
			}
			map<char, double> _char_weight;
			char* __data = new char;
			while (!iF.eof()) {
				iF.read(__data, sizeof(char));
				_char_weight[*__data] ++;
			}
			for (auto& it : _char_weight)
				ht.push_back(HTreeNode{ it.first, it.second, -1, -1, -1 });
			CreateHTree(ht);
			ofstream oF("output/hfmTree.txt");
			for (auto it : ht)
				if (it.data == '\n')
					oF << "\\n " << it.weight << " " << it.parent << " " << it.lchild << " " << it.rchild << endl;
				else
					oF << it.data << " " << it.weight << " " << it.parent << " " << it.lchild << " " << it.rchild << endl;
			oF.close();
			hfmcode = CreateHCode(ht);
			for (auto& c : hfmcode)
				cout << c.first << " " << c.second << endl;
			cout << "树建完啦!" << endl;
			iF.close();
			iF.open(filename);
			oF.open("output/codefile.txt");
			while (!iF.eof()) {
				iF.read(__data, sizeof(char));
				oF << hfmcode[*__data];
			}
			oF.close();
			cout << "编码已完成" << endl;
			system("pause");
			
		}break;

		case '3':{
			string filename;
			cout << "Please enter the dir of the char-weight file: ";
			cin >> filename;
			ifstream iF(filename);
			if (!iF.good()) {
				cout << "File open failed!" << endl;
				exit(0);
			}
			else
			{
				string temp;
				while (!iF.eof()) {
					getline(iF, temp);
					stringstream sstemp(temp);
					double _weight;
					char _data;
					int _lchild, _rchild, _parent;
					if (temp.substr(0, 2) == "\\n") {
						string l;
						_data = '\n';
						sstemp >> l;
						sstemp >> _weight >> _parent >> _lchild >> _rchild;
						ht.push_back(HTreeNode{ _data, _weight, _parent, _lchild, _rchild });
					}
					else if (temp[0] == ' ') {
						_data = ' ';
						sstemp >> _weight >> _parent >> _lchild >> _rchild;
						ht.push_back(HTreeNode{ _data, _weight, _parent, _lchild, _rchild });
					}
					else {
						sstemp >> _data >> _weight >> _parent >> _lchild >> _rchild;
						ht.push_back(HTreeNode{ _data, _weight, _parent, _lchild, _rchild });
					}
				}
				CreateHTree(ht);
				hfmcode = CreateHCode(ht);
				iF.close();
			}
		}break;

		case '4':{
			string filename;
			cout << "Please enter the dir of the file: ";
			cin >> filename;
			ifstream iF(filename);
			ofstream oF("output/codefile.txt");
			if (!iF.good()) {
				cout << "File open failed!" << endl;
				exit(0);
			}
			char *ch = new char;
			while(!iF.eof()){
				iF.read(ch, sizeof(char));
				oF << hfmcode[*ch];
			}
			oF.close();
			iF.close();
		}break;

		case '5':{ //5. 对编码文件进行译码
			string filename;
			cout << "Please enter the dir of the file: ";
			cin >> filename;
			ifstream iF(filename);
			ofstream oF("output/recodefile.txt");
			if (!iF.good()) {
				cout << "File open failed!" << endl;
				exit(0);
			}
			map<string, char> ihc = Inverse_HCode(hfmcode);
			string _str = "";
			char* ch = new char;
			while(!iF.eof()){
				iF.read(ch, sizeof(char));
				_str += *ch;
				if(ihc.find(_str) != ihc.end()){
					oF << ihc[_str];
					_str = "";
				}
			}
			oF.close();
			iF.close();
		}break;

		case 'Q': case 'q':
			return 0;
		default:
			cout << "输错了！" << endl;
		}
		system("pause");
	}
	return 0;
}