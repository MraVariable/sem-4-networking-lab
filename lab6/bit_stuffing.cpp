#include<iostream>
#include<string>
using namespace std;

int main()
{
    cout<<"STRING:";
    string str;
    cin>>str;
    if(str.length()<8){cout<<"Invalid Input";return 0;}
    for(int i=0;i<str.length();i++)
    {
        if(str[i]!='0' && str[i]!='1'){cout<<"Invalid Input";return 0;}
    }
    string output=str.substr(0,6);
    string prev=output;
    for(int i=6;i<str.length();i++)
    {
        if(prev=="011111")
        {
            output+='0';
        }
        output+=str[i];
        prev=prev.substr(1)+str[i];
    }
    cout<<"OUTPUT:"<<output;
    string input=output.substr(0,6);
    prev=input;
    for(int i=6;i<output.length();i++)
    {
        if(prev!="011111"){input+=output[i];}
        prev=prev.substr(1)+output[i];
    }
    cout<<"\nEXTRACTED INPUT STRING:"<<input;
}
