#include<fstream>
#include<iostream>
using namespace std;

int main()
{
    ifstream f;
    f.open("sample.txt");
    string temp;
    getline(f,temp);
    f.close();
    fstream a;
    a.open("binary.dat",ios::out|ios::binary);
    a.write((char*)&temp,sizeof(temp));
    a.close();
    a.open("binary.dat",ios::binary|ios::in);
    while(a.read((char*)&temp,sizeof(temp)))
    {
        cout<<temp;
    }
    f.close();
}
