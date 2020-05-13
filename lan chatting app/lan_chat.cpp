#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
using namespace std;

int startListening();
int listeningServer = -1;
class User
{
    public:
    User(string ip,string port,string name,int socket)
    {
        this->port = port;
         this->ip = ip;
         this->name = name;
         this->socket = socket;
         this->groupNum = rand()%7;
    }
    User(){}
    string port;
    string ip;
    string name = "";
    int socket = -1;
    int groupNum = 0;
};


class Users
{
public:
 User users[10];
    int insertPos = 0;
    User addNewUser(string ip,string port,string name,int socket){
        User newUser = User(ip, port, name,socket);
        users[insertPos] = newUser;
        insertPos += 1;
        return newUser;
    }
    string getNameOfAllUsers(){
        string names = "";
        for(int i = 0;i<10;++i){
            if(users[i].name == ""){
                continue;
            }
            names += users[i].name + "Group Num : " + to_string(users[i].groupNum);
            names += "\n";
        }
        return names;
    }
    User* getUserByName(string name){
           for(int i = 0;i<10;++i){
               if(users[i].name == name){
                   return &users[i];
               }
           }
           return NULL;
       }
}AllUsers;
void *handleSingleClient(void *threadid){
           sockaddr_in client;
            socklen_t clientSize = sizeof(client);
       cout<<"Waiting for connection"<<endl;
       int clientSocket = accept(listeningServer, (sockaddr*)&client, &clientSize);
       cout<<"Connection established";
       char host[NI_MAXHOST];
       char service[NI_MAXSERV];
       memset(host, 0, NI_MAXHOST);
       memset(service, 0, NI_MAXSERV);
       if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
       {
           cout << host << " \nconnected on port " << service << endl;
       }
       else
       {
           inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
           cout << host << " connected on port " << ntohs(client.sin_port) << endl;
       }
    bool accountCreated = false;
    string nameOfClient = "";
    string IntroMessage = "See the list of all people \n" + AllUsers.getNameOfAllUsers()+"\n";
    string IntroMessage2 = "Please enter your username - create new if you are new\n\n";
    string clientHandle =  IntroMessage + IntroMessage2;
     char *mess = &clientHandle[0];
      send(clientSocket, mess,clientHandle.length(), 0);
        char buffer[1024];
       char quit[] = {'q','u','i','t'};
        while (true)
        {
            memset(buffer, 0, 1024);
            int bytesRecFromClient = recv(clientSocket, buffer, 1024, 0);
            if (bytesRecFromClient == -1)
            {
                cout << "Error in recv() function" << endl;
                break;
            }
            if (bytesRecFromClient == 0)
            {
                cout << "Client disconnected - bye bye" << endl;
                break;
            }
            User senderUser = User();
            if (!accountCreated){
          senderUser  =  AllUsers.addNewUser(string(host), string(service), string(buffer, 0, bytesRecFromClient),clientSocket);
                accountCreated = true;
                cout<<string(buffer, 0, bytesRecFromClient)<<endl;
                nameOfClient = senderUser.name;
                cout<<senderUser.name;
                 string notif = "New User - " +nameOfClient+ " Press 1 to see the user's list\n";
                for(int i = 0;i<10;++i){
                    int notifSocket = AllUsers.users[i].socket;
                    if (notifSocket == -1){
                        continue;
                    }
                    send(AllUsers.users[i].socket,&notif[0],notif.length(),0);
                }
            }else{
                string clientRespone = string(buffer, 0, bytesRecFromClient);
                string message = "";
                int counter = 0;
                    for(int i = 0;i<clientRespone.length();++i){
                        if(clientRespone[i] != '_'){
                            message += clientRespone[i];
                        }else{
                            break;
                        }
                        counter += 1;
                    }
                counter += 1;
                if (counter<bytesRecFromClient){
                string recName = string(buffer, counter, bytesRecFromClient);
                    if (bytesRecFromClient-counter  == 2 ){
                         string recMessage = message + " - Group Message By " + nameOfClient;
                        for(int i = 0;i<10;++i){
                                          int notifSocket = AllUsers.users[i].socket;
                            int groupCode = AllUsers.users[i].groupNum;
                            string groupNumStr = to_string(groupCode) + "\n";
                                          if (notifSocket == -1 || groupNumStr != recName){
                                              continue;
                                          }
                                          send(AllUsers.users[i].socket,&recMessage[0],recMessage.length(),0);
                                      }
                    }else{
               User* recUser =  AllUsers.getUserByName(recName);
                cout<<"Sender's Name"<<nameOfClient<<endl;
                string recMessage = message + " -  Sent By " + nameOfClient;
                  send(recUser->socket, &recMessage[0],recMessage.length(), 0);
                }
                }
            }
            string clientReq = string(buffer,0,bytesRecFromClient);
            string quitStr = string(quit,0,4);
           quitStr += "\n";
            if (clientReq.compare(quitStr)==0){
                string closingMess = "Closing the connection as requested by client";
                char *mess = &closingMess[0];
                send(clientSocket, mess,closingMess.length(), 0);
                cout<<closingMess<<endl;
                break;
            }

            else if (clientReq.compare("1\n")==0){
                string newList = AllUsers.getNameOfAllUsers();
                send(clientSocket,&newList[0],newList.length(),0);
            }
            else{}
        }
       close(clientSocket);
    return 0;
}
int main()
{
     startListening();
    return 0;
}
int startListening()
{
     listeningServer = socket(AF_INET, SOCK_STREAM, 0);
    if (listeningServer == -1)
    {
        cout << "Error in creating a socket" << endl;
        return -1;
    }
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    bind(listeningServer, (sockaddr*)&hint, sizeof(hint));
    listen(listeningServer, SOMAXCONN);
    cout<<"Currently listnening"<<endl;
    int AllowedClients = 10;
    pthread_t threads[AllowedClients];
      int error;
      int i;
      for( i = 0; i < AllowedClients; i++ )
      {
         error = pthread_create(&threads[i], NULL,  handleSingleClient, (void *)i);
         if (error)
         {
            cout << "Error:unable to create thread," << error << endl;
            exit(-1);
         }
      }
      pthread_exit(NULL);
    return 0;
}




