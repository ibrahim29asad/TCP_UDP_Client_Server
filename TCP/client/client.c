#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>


/*
References:
To Get the Connections and Send and Recives:
Used the Tutorial as my base 
To input numbers into 
https://www.tutorialspoint.com/cprogramming/c_command_line_arguments.htm
To get the Time:
https://www.tutorialspoint.com/c_standard_library/c_function_time.htm
https://www.tutorialspoint.com/c_standard_library/c_function_ctime.htm
FILE REad and Write:
https://www.programiz.com/c-programming/c-file-input-output


*/


int main(int validnumber, char *port[])
  {


int tf = atoi(port[1]);
// printf("%d\n", tf);
// Sets time and Date 
 struct sockaddr_in address;
 memset(&address, 0, sizeof(address));
 address.sin_family = AF_INET;
 address.sin_port = htons(tf);
 address.sin_addr.s_addr = inet_addr("127.0.0.1");


 int serversocket1;
 serversocket1 = socket(AF_INET, SOCK_STREAM, 0);
 if(serversocket1 == -1){
 	printf("socket() call failed");
 }

 
 int status;
 status = connect(serversocket1, (struct sockaddr *)&address, sizeof(struct sockaddr_in));
 if(status==-1){
 	printf("connect() call failed");
 }




  int count;
  int UCID;
  char snd_message[100];
  printf("Please enter your UCID: ");
  scanf("%d", &UCID);
  

  sprintf(snd_message, "%d", UCID);

 //process 1  
 count = send(serversocket1, snd_message, 100, 0);
 if(count == -1){
 	printf("send() call failed, Process 1");
 }

 //process 2 
 time_t dt;
 char rcv_message[100];
 count = recv(serversocket1, &dt, sizeof(rcv_message), 0);
 if(count == -1){
 	printf("recv() call failed, Process 2");
 }
  printf("%s\n", ctime(&dt));


 serversocket1 = 3;
 //process 3


  int c = dt;
  int y = c/60;
  c = c - (y*60);
  int password = UCID % 10000;

  password = password + c; 

   
  printf("The Password Sent to the server is %d\n", password);
 count = send(serversocket1, &password, sizeof(int), 0);
 if(count == -1){
 	printf("send() call failed, Process 3.");
 }



 serversocket1 = 3;

int flie_there = 0;

count = recv(serversocket1, &flie_there, sizeof(int), 0);
 if(count == -1){
 	printf("recv() call failed.");
 }



if(flie_there == 0){
int cp;
count = recv(serversocket1, &cp, sizeof(int), 0);
 if(count == -1){
 	printf("recv() call failed.");
 }

char wait[cp];
 count = recv(serversocket1, &wait, sizeof(wait), 0);
 if(count == -1){
 	printf("recv() call failed.");
 }


FILE *fp = fopen("data.txt", "w+");
fwrite(wait, 1, sizeof(wait),fp);

}
else if(flie_there == 1){
    // printf("I DID NOT BYPASSED EVERYTHIN \n");
  printf("The File is not there \n");
}


 close(serversocket1);

  }


