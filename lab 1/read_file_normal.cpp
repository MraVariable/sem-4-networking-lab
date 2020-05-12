#include<fstream>
#include<iostream>
#include<vector>
using namespace std;
int main()
{
    ifstream obj;
    obj.open("lab1packet.pcap");
    unsigned char temp;
    int c=1;
    vector<unsigned char>macd,macs,ipcs,ips,ipd,tcps,tcpd,sqn,ack,syn;
    while(obj.good())
    {
        obj>>temp;
        //printf("%X",temp);
        //cout<<" "<<c<<endl;
        if(c>=41 &&c<=46){macd.push_back(temp);}
        else if(c>=47 && c<=52){macs.push_back(temp);}
        else if(c==65 || c==66){ipcs.push_back(temp);}
        else if(c>=67 && c<=70){ips.push_back(temp);}
        else if(c>=71 && c<=74){ipd.push_back(temp);}
        else if(c==75 || c==76){tcps.push_back(temp);}
        else if(c==77 || c==78){tcpd.push_back(temp);}
        else if(c>=79 && c<=82){sqn.push_back(temp);}
        else if(c>=83 && c<=85){ack.push_back(temp);}
        else if(c==87){syn.push_back(temp);}
        c++;
    }
    cout<<"\nDestination mac:";
    for(int i=0;i<macd.size();i++){printf("%X ",macd[i]);}
    cout<<"\nSource mac:";
    for(int i=0;i<macs.size();i++){printf("%X ",macs[i]);}
    cout<<"\nIP Checksum:";
    for(int i=0;i<ipcs.size();i++){printf("%X ",ipcs[i]);}
    cout<<"\nIP Source:";
    for(int i=0;i<ips.size();i++){printf("%X ",ips[i]);}
    cout<<"\nDestination IP:";
    for(int i=0;i<ipd.size();i++){printf("%X ",ipd[i]);}
    cout<<"\nSource TCP port:";
    for(int i=0;i<tcps.size();i++){printf("%X ",tcps[i]);}
    cout<<"\nDestination tcp port:";
    for(int i=0;i<tcpd.size();i++){printf("%X ",tcpd[i]);}
    cout<<"\nSequence Number:";
    for(int i=0;i<sqn.size();i++){printf("%X ",sqn[i]);}
    cout<<"\nAcknowledgment Number:";
    for(int i=0;i<ack.size();i++){printf("%X ",ack[i]);}
    cout<<"\nSYN Bit:";
    for(int i=0;i<macs.size();i++){printf("%X ",syn[i]);}
}
