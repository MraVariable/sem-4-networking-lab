#include<iostream>
#include<string>
using namespace std;

string divide(string cur,string gen)
{
    if(cur[0]=='0'){return cur.substr(1);}
    string ans="";
    for(int i=1;i<gen.length();i++)
    {
        if(gen[i]==cur[i]){ans+='0';}
        else{ans+='1';}
    }
    return ans;
}

int main()
{
    string data,gen;
    cout<<"DATA AND GENERATOR\n";cin>>data>>gen;
    string tempdata=data;
    if(gen.length()>data.length()){cout<<"INVALID";return 0;}
    for(int i=1;i<gen.length();i++){tempdata+='0';}
    string cur=tempdata.substr(0,gen.length());
    cur=divide(cur,gen);
    for(int i=gen.length();i<tempdata.length();i++)
    {
        cur+=tempdata[i];
        cur=divide(cur,gen);
    }
    string dataword=data+cur;
    cout<<"DATAWORD:"<<dataword;
    /*dataword[1]='1';*//////////////////uncomment this line to insert one bit error and see if error is detected or not
    cur=dataword.substr(0,gen.length());
    cur=divide(dataword,gen);
    for(int i=gen.length();i<dataword.length();i++)
    {
        cur+=dataword[i];
        cur=divide(cur,gen);
    }
    for(int i=0;i<cur.length();i++)
    {
        if(cur[i]=='1'){cout<<"\nERROR DETECTED";return 0;}
    }
    cout<<"\nNO ERROR DETECTED";
}
