#include <iostream>
#include <fstream>

using namespace std;

#if 1
//文件处理
int main(){
    string instr, str;
    getline(cin, instr, '\n');
    ofstream oF("text.txt");
    oF << instr << endl;
    oF.close();
    ifstream iF("text.txt");
    getline(iF, str, '\n');
    iF.close();
    cout << str << endl;
    return 0;
}

#else
//形与参(逆指针)
void swap(int &c, int &d){
    int t = c;
    c = d;
    d = t;
}
int main(){
    int a, b;
    cin >> a >> b;
    cout << a << "\t" << b << endl;
    swap(a,b);
    cout << a << "\t" << b << endl;
    return 0;
}
#endif