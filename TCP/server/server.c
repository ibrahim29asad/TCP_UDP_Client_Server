#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


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

 str
    ct sockaddr_in address;
 memset(&address, 0, sizeof(address));
 address.sin_family = AF_INET;
 address.sin_port = htons(tf);
 address.sin_addr.s_addr = htonl(INADDR_ANY);
 int serversocket1;
 serversocket1 = socket(AF_INET, SOCK_STREAM, 0);
 if(serversocket1 == -1){
 	printf("socket() call failed");
 }


 int status;
 status = bind(serversocket1, (struct sockaddr *)
 &address, sizeof(struct sockaddr_in));
 if(status==-1){
 	printf("bind() call failed");
}


while(tf == tf){

 status = listen(serversocket1,5);
 if(status==-1){
 	printf("listen() call failed");
 }


  int serversockets2_server;
 serversockets2_server = accept(serversocket1, NULL, NULL);
 if(serversockets2_server == -1){
 	printf("accept() call failed");
 }

  //  printf("Socket is %d\n", serversockets2_server);



 time_t datetime;
 time(&datetime);
// printf("%s\n", ctime(&datetime));
 

 int c = datetime;
 int y = c/60;
 c = c - (y*60);

  //process 1 
 int count;
 char rcv_message[100];
 count = recv(serversockets2_server, rcv_message, 100, 0);
 if(count == -1){
 	printf("recv() call failed.");
 }
// printf("%s\n", rcv_message);


 //process 2
  char send_time[100]= {*ctime(&datetime)};
  count = send(serversockets2_server, &datetime, sizeof(send_time), 0);
 if(count == -1){
 	printf("recv() call failed.");
 }




 //process 3
  int password = atoi(&rcv_message[4]) + c; 
  // printf("%d\n", password);

  
int cp;
 count = recv(serversockets2_server, &cp, sizeof(int), 0);
 if(count == -1){
 	printf("recv() call failed.");
 }
// printf("%d\n", cp);


  
    // printf("Socket is %d\n", serversockets2_server);
  if (cp!=password){
       printf("Passwords do not match");
  }
  else{
    FILE *fp = fopen("data.txt", "r+");
    
    int flie_there;

  if(fp == NULL){
    flie_there = 1;
  }
  else{
    flie_there = 0;
  }

    count = send(serversockets2_server, &flie_there, sizeof(int), 0);
    if(count == -1){
 	  printf("send() call failed, Process 3.");
    }



    if(fp != NULL){
    // printf("FILE is there");
   //char file_test[9999][9999];
   char mb [999999]= {""};
    int s = getc(fp);
    int a;

    for(a = 0; s !=EOF; a++){
      char c = s;
      strncat(mb, &c, 1);
      s =fgetc(fp);
    }
    
    
    count = send(serversockets2_server, &a, sizeof(int), 0);
    if(count == -1){
 	  printf("send() call failed, Process 3.");
    }

    // printf("%s", mb);
    count = send(serversockets2_server, mb, 999999, 0);
    if(count == -1){
 	  printf("send() call failed, Process 3.");
    }


    }

    
  }

  close(serversockets2_server);

}


 close(serversocket1);
  }
