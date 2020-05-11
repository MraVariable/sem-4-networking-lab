#include<fstream>
#include<iostream>
#include<vector>
using namespace std;
int main()
{
    ifstream obj;
    obj.open("lab1packet.pcap");
    unsigned char temp;
    int counter=1;
    vector<unsigned char> store;
    vector<unsigned char> checksum;
    while(obj.good())
    {
        obj>>temp;
        //printf("%X ",temp);
        if(counter==65 || counter ==66)
        {
            checksum.push_back(temp);
        }
        else if(counter>=55 && counter<=74)
        {
            store.push_back(temp);
        }
        //cout<<counter<<endl;
        counter++;
    }
    cout<<"checksum bytes\n";
    printf("%X ",checksum[0]);printf("%X ",checksum[1]);
    cout<<"\nheader bytes\n";
    for(int i=0;i<store.size();i++)
    {
        printf("%X ",store[i]);
    }
    cout<<endl;
    unsigned long sum=0;
    for(int i=0;i<=16;i+=2)
    {
        unsigned long test=0x0000;
        test=test|store[i];
        test=test<<8;
        test=test|store[i+1];
        printf("%X added\n",test);
        sum+=test;
        if(sum & 0xffff0000)
        {
            sum=sum&0xffff;////////////////adding in one's complement that is if any carry then add that carry again in the sum
            sum++;
        }
    }
    unsigned long sumtemp=sum;
    sum=~(sum)&0x0000ffff;
    unsigned long checksumbit=0x0000;
    checksumbit=checksumbit|checksum[0];
    checksumbit=checksumbit<<8;
    checksumbit=checksumbit|checksum[1];
    cout<<"RESULT:";
    if(checksumbit==sum)
    {
        cout<<"CORRECT\n";
        cout<<"sum is:"<<hex<<sum;
        cout<<" and checksum bytes are:"<<hex<<checksumbit;
        cout<<endl<<"adding sum before complement and checksum bytes:";
        sumtemp+=checksumbit;
        sumtemp=(~sumtemp)&0x0000ffff;
        printf("%X thus no error detected",sumtemp);
    }
    else
    {
        cout<<"ERROR";
        cout<<"sum is:"<<hex<<sum;
        cout<<" and checksum bits are:"<<hex<<checksumbit;
    }
}
