#include<fstream>
#include<iostream>
using namespace std;
int main()
{
    ifstream obj;
    obj.open("lab1packet.pcap");
    unsigned char temp;
    while(obj.good())
    {
        obj>>temp;
        printf("%X\n",temp);
    }
    cout<<"done";
}
