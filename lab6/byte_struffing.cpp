#include<iostream>
#include<string>
using namespace std;

int main()
{
    cout<<"FLAG:00111110\nESC:00101010\n";
    cout<<"INPUT:";
    char flag='>',esc='*';
    string str;cin>>str;
    string output="";
    for(int i=0;i<str.length();i++)
    {
        if(str[i]==flag || str[i]==esc){output+=esc;}
        output+=str[i];
    }
    cout<<"OUTPUT:"<<output<<endl;
    /*output[2]=flag;*///////////////////////uncomment this line to see that if byte stuffing not done then input string terminates when flag encountered
    string input="";
    for(int i=0;i<output.length();i++)
    {
        if(output[i]==flag){break;}
        if(output[i]==esc){input+=output[i+1];i++;}
        else{input+=output[i];}
    }
    cout<<"EXTRACTED INPUT:"<<input;
    return 0;
}
