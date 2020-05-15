#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include<fstream>

using namespace std;
int attendance = 0;

void userDataToFile(string userCred,string attendance)
{
    fstream stream;
    stream.open(userCred+".txt",ios::out);
    string data = userCred +" : " + " Attendance : "+attendance+"\n";
    stream>>data;
    stream.close();
}


string serverRepsone(char*buffer,int sock);

int attachClientToServer(string accountCredentials)
{
       bool loggedIn = false;
       int sock = socket(AF_INET, SOCK_STREAM, 0);
       if (sock == -1)
       {
           return 1;
       }
       int port = 54000;
       string ipAddress = "0.0.0.0";
       sockaddr_in hint;
       hint.sin_family = AF_INET;
       hint.sin_port = htons(port);
       inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
       int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
       if (connectRes == -1)
       {
           return 1;
       }
       char buffer[4096];
       string serverRequest;
       cout<<serverRepsone(buffer,sock);
       do
        {
            if(loggedIn)
            {
                cout<<"\nSee your information saved on server - 1"<<endl;
                cout<<"Attend a class of your department-2"<<endl;
                cout << "YOU-> ";
                cin>>serverRequest;
                if(serverRequest == "1")
                    {}
                else if(serverRequest == "2")
                {
                   attendance += 1;
                   string atten = to_string(attendance);
                   userDataToFile(accountCredentials,atten);
               }
               else if(serverRequest == "3")
                {
                   cout<<"DISCONNECTED FROM SERVER"<<endl;
                   break;
               }
               else
               {
                   cout<<"Wrong input\n";
                   continue;
               }
            }
            else
            {
                serverRequest = accountCredentials;
                loggedIn = true;
            }
            int sendRes = send(sock, serverRequest.c_str(), serverRequest.size() + 1, 0);
            if (sendRes == -1)
            {
               cout << "Could not send to server\n";
               continue;
            }
            cout<<serverRepsone(buffer,sock);
       }while(true);
       close(sock);
       return 0;
}

int main()
{
    cout<<"First Register yourself. Follow below steps "<<endl;
    string name,code;
    cout<<"Enter your name"<<endl;
    cin>>name;
    cout<<"Enter your code"<<endl;
    cin>>code;
    string accountCredentials = name+"_"+code;
    attachClientToServer(accountCredentials);
}

string serverRepsone(char*buffer,int sock){
              memset(buffer, 0, 4096);
              int bytesReceived = recv(sock, buffer, 4096, 0);
              if (bytesReceived == -1)
              {
                  return "Error in getting response";
              }
              else
              {
                  return string(buffer, bytesReceived);
              }
}


