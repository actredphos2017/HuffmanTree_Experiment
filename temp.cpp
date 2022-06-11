#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <map>

using namespace std;

struct HTreeNode                //���������������
{
	char data;                    //�ַ�
	double weight;                    //Ȩֵ
	int parent;                    //˫�׵�λ��
	int lchild;                    //���ӵ�λ��
	int rchild;                    //�Һ��ӵ�λ��
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
		for (j = 0; j < i; j++)
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

void show() {
	system("cls");
	cout << "------------- �������� -------------" << endl;
	cout << " 1. �ն������ַ�����Ƶ����Ƶ�ʣ�����" << endl;
	cout << " 2. ��ȡ�ļ���ȡ��Ƶ�����������������" << endl;
	cout << " 3. ��ȡ��������" << endl;
	cout << " 4. �ô��ڵĹ�������������ļ�����" << endl;
	cout << " 5. �Ա����ļ���������" << endl;
	cout << " Q. �˳�" << endl;
	cout << "��ѡ��: ";
}

double string_to_double(string str){
	double res = 0;
	bool flag = false;
	int digical_count = 0;
	for(auto c : str){
		if(c == '.'){
			flag = true;
			continue;
		}
		res = res*10 + (int)(c - '0');
		if(flag) count ++;
	}
	res /= pow(10, count);
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
		case '1':{
			int n;
			cout << "�����ַ�������";
			cin >> n;
			string temp;
			char _data;
			double _weight;
			for (int i = 0; i < n; i++) {
				getline(cin, temp, '\n'); // "n 23.12"
				if(temp.substr(0, 2) == "\\n"){
					_data = '\n';
					_weight = string_to_double(temp.substr(3));
				}else{
					_data = temp[0];
					_weight = string_to_double(temp,substr(2));
				}
				ht.push_back(HTreeNode{ _data, _weight, -1, -1, -1 });
			}
			CreateHTree(ht);
			ofstream oF("output/hfmTree.txt");
			for(auto it : ht)
				if(it.data == '\n')
					oF << "\\n " << it.weight << " " << it.parent << " " << it.lchild << " " << it.rchild << endl;
				else
					oF << it.data << " " << it.weight << " " << it.parent << " " << it.lchild << " " << it.rchild << endl;
			oF.close();
			hfmcode = CreateHCode(ht);
			for (auto& c : hfmcode)
				cout << c.first << " " << c.second << endl;
			cout << "The tree has been built successfully!" << endl;
		}break;
		case '2':{
			string filename;
			cout << "�������ַ�Ȩ���ļ���Ŀ¼: ";
			cin >> filename;
			ifstream iF(filename);
			if(!iF.good()){
				cout << "File open failed!" << endl;
				exit(0);
			}
			map<char, double> _char_weight;
			char* _data = new char;
			while(!iF.eof()){
				iF.read(_data, sizeof(char));
				_char_weight[_data] ++;
			}
			for(auto &it : _char_weight)
				ht.push_back(HTreeNode{it.first, it.second, -1, -1, -1});
			CreateHTree(ht);
			ofstream oF("output/hfmTree.txt");
			for(auto it : ht)
				if(it.data == '\n')
					oF << "\\n " << it.weight << " " << it.parent << " " << it.lchild << " " << it.rchild << endl;
				else
					oF << it.data << " " << it.weight << " " << it.parent << " " << it.lchild << " " << it.rchild << endl;
			oF.close();
			hfmcode = CreateHCode(ht);
			for (auto& c : hfmcode)
				cout << c.first << " " << c.second << endl;
			cout << "��������!" << endl;
			string _str = "";
			iF.seekg(0, ios::beg);
			while(!iF.eof()){
				iF.read(_data, sizeof(char));
				_str += hfmcode[_data];
			}
			ofstream oF("output/code.txt");
			oF << _str;
			oF.close();
			cout << "���������" << endl;
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
					//   a 23.12 12 -1 -1
					getline(iF, temp);
					stringstream sstemp(temp);
					double _weight;
					char _data;
					int _lchild, _rchild, _parent;
					if (temp.substr(0, 2) == "\\n") {
						string lj;
						_data = '\n';
						sstemp >> lj;
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
		case 'Q': case 'q':
			return 0;
		default:
			cout << "С���ӣ���tm������ˣ�" << endl;
		}
		system("pause");
	}
	return 0;
}