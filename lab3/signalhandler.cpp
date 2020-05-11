#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <signal.h>
using namespace std;
void signalCallback(int signum) {
   cout << "Caught signal " << signum << endl;
   exit(signum);
}
int main(){
   signal(SIGINT, signalCallback);
   while(true){
      cout << "Some work" << endl;
      sleep(1);
   }
   return EXIT_SUCCESS;
}
