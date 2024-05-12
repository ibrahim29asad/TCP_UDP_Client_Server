#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#define _OE_SOCKETS
#include <sys/types.h>
#define _XOPEN_SOURCE_EXTENDED 1
#include <sys/socket.h>
#include <string>
#include <iostream>
#include<fstream>
#include <cstdlib>

using namespace std;

/*

Refrence:
D2L:
- Chapter 2 Textbook Chapter Slides
- Week 5 Tutorial 
- Assignment 3 Video "Socket Programming And A3 Faq"


Website Links:
https://linux.die.net/man/3/setsockopt 
https://www.cs.cmu.edu/~srini/15-441/S10/lectures/r01-sockets.pdf 
https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/ 
https://cplusplus.com/reference/string/string/clear/
https://www.techiedelight.com/find-size-of-file-c/ 
https://www.tutorialspoint.com/writing-a-binary-file-in-cplusplus   
https://courses.cs.vt.edu/cs2604/fall02/binio.html 
https://cplusplus.com/reference/fstream/fstream/ 
https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-bind-bind-name-socket   
*/

struct EFTP{

string AUTH[5] = {"01","","", "", ""};
string RRQ[4] = {"02","", "", ""}; // read
string WRQ[4] = {"03","", "", ""}; // write 
string DATA[9000][5] = {"04","","", "", ""}; // acutal segement data (1028 charecters)
string ACK[9000][4] = {"05","", "", ""};
string ERROR[3] = {"06", "", ""};
};


//./server [username:password] [port] [working directory]
//./server username:password 2007 server

/*
  Current Directory
 /home/ugb/ibrahim.asad/UDP/udp_server
*/
int main(int argc, char** argv){

char ch;
int loop_in = 0;
int upload_block = 1;
int block_ack = 1;
while(loop_in >= 0){

string user_pass = argv[1];
string ports_s = argv[2];
string pwd = argv[3];

int ports = stoi(ports_s);






  // session number
  int sesh = 1; 
  string session = to_string(sesh);
	string password = "password";
	string username = "username";
  string sourcefile = "hare.jpg"; 

//send is true 
//recv is false
bool send_recv = true; 


struct EFTP packet_struc;

packet_struc.AUTH[1] = username;
packet_struc.AUTH[3] = password;
packet_struc.RRQ[1] = session;
packet_struc.WRQ[1] = session;
packet_struc.RRQ[2] = sourcefile;
packet_struc.WRQ[2] = sourcefile;

//// << packet_struc.AUTH[0] << "\n";
//// << packet_struc.DATA[0][0] << "\n";
//// << packet_struc.AUTH[1] << "\n";
//// << packet_struc.AUTH[3] << "\n";



//string copiedfile = "examp";


//
// << ":" << ports << ":\n";
unsigned short final_port = (unsigned short) ports;
int status;
  struct sockaddr_in servaddr;
   struct sockaddr_in cliaddr;
 memset ((char*) &servaddr, 0, sizeof(servaddr));
 memset ((char*) &servaddr, 0, sizeof(cliaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_port = htons(final_port);
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

 int mySocket;
 mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
 if(mySocket == -1){
 	cout << "socket() call failed \n";
 }

unsigned int xd = sizeof(cliaddr);
int time = 5;
int timer = getsockopt(mySocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&time, &xd);
 if(timer == -1){
 cout << "Could not bind place timer \n";
 return -1;
 }


 status = bind(mySocket, (struct sockaddr*) &servaddr, sizeof(servaddr)); 
 if(status == -1){
 cout << "Could not bind to port \n";
 return -1;
 }



unsigned int x = sizeof(cliaddr);
//printf("%d \n", strlen(rcv_message));
char opcode[2];
char errormes[512];
char AUTH[1024];
// recieved the username and password
int count = recvfrom(mySocket, AUTH,1024, 0, 
            (struct sockaddr*) &servaddr, &x);
if(count <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}

string comp_user_pass ="";

for (int i  = 0; i < 1024; i++){
  ch = AUTH[i];
  if (ch != '\0')
  comp_user_pass += ch;
  
}

//// << "The server "<< user_pass << " The client " << comp_user_pass << "\n";
//// << "The server length "<< user_pass.length() << " The client lengths " << 
   //   comp_user_pass.length() << "\n";

int randomsession = rand() + 1;
randomsession = randomsession%65535;
if(randomsession == 0){
  randomsession = 1;
}



if(comp_user_pass == user_pass){
opcode[0] = '0';
opcode[1] = '5';
string opc = "05";
//// << "Sent an Ack" << "\n";
packet_struc.ACK[0][0]= opc;
packet_struc.ACK[0][1]= to_string(randomsession);
packet_struc.ACK[0][2]= "0";
packet_struc.ACK[0][3]= "0";


string s = to_string(randomsession);
packet_struc.RRQ[1] = s;
packet_struc.WRQ[1] = s;
char randsesh[5];
//// << s << "\n" << "\n";
//// << "Length is \n" << s.length() << "\n";
for(int i = 0; i < 5; i++){
  randsesh[i] = s.at(i);
}
//// << "TESTING IT HERE \n" << "\n";
count = sendto(mySocket, opcode, 2, 0, (struct sockaddr*) &servaddr, x);
if(count == -1){
  cout<< "Did not send, Termintating Side \n";
  return -1;
}
//// << count << "\n";
count = sendto(mySocket, randsesh, 5, 0, (struct sockaddr*) &servaddr, x);
if(count == -1){
  cout<< "Did not send, Termintating Side \n";
  return -1;
}
char block[2];
//// << count << "\n";
//count = sendto(mySocket, block, 1, 0, (struct sockaddr*) &servaddr, x);
char segn[4];

//count = sendto(mySocket, segn, 1, 0, (struct sockaddr*) &servaddr, x);
 //// << count << "\n";

} else{
opcode[0] = '0';
opcode[1] = '6';


char errormes[512] = "Username and Passwords do not match";
string s(errormes);
packet_struc.ERROR[2] = "1";
packet_struc.ERROR[1] = s;
count = sendto(mySocket, opcode, 2, 0, (struct sockaddr*) &servaddr, x);
if(count == -1){
  cout<< "Did not send, Termintating Side \n";
  return -1;
}
//// << count << "\n";
 ////.flush();
count = sendto(mySocket, errormes, 512, 0, (struct sockaddr*) &servaddr, x);
if(count == -1){
  cout<< "Did not send, Termintating Side \n";
  return -1;
}
//// << count << "\n";
 ////.flush();
}

//// << "TEST \n";
 char opcoderw[2];
char filenames[255];
for(int L = 0; L < 255; L++){
  filenames[L] = '\0';
}

char bytess[1];
count = recvfrom(mySocket, opcoderw, 2, 0, (struct sockaddr*) &servaddr, &x);
if(count <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}

count = recvfrom(mySocket, filenames, 255, 0, (struct sockaddr*) &servaddr, &x);
if(count <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}
puts(filenames);
//cout << "TEST1 \n";



string tempos = pwd + "/";

if(opcoderw[1] == '3'){
  packet_struc.WRQ[3] = "1";
  packet_struc.RRQ[3] = "0";
  
//// << "TEST1 \n";
  for (int i  = 0; i < count; i++){
  ch = filenames[i];
  if (ch != '\0')
  tempos += ch;  
  }
  packet_struc.WRQ[2] = tempos;
  packet_struc.RRQ[2] = tempos;
}
else{
  //RRQ
    packet_struc.WRQ[3] = "0";
  packet_struc.RRQ[3] = "1";

  for (int i  = 0; i < count; i++){
  ch = filenames[i];
  if (ch != '\0')
  tempos += ch;  
  }
  packet_struc.WRQ[2] = tempos;
  packet_struc.RRQ[2] = tempos;
  send_recv = false;
}

sourcefile = tempos;

//// << "the initial is called: " << tempos << "\n";
//// << "the after is called: " << sourcefile << "\n";

//begin upload

//cout << send_recv << "\n";
if(send_recv==true){
bool coca = true;
string inputfile;
//cout << "IM Finally here \n";
string sourcefile_upload = "";

// works for txt.
int hwm = 0;
char file_exist_op[2];
count = recvfrom(mySocket, file_exist_op, 2, 0, (struct sockaddr*) &servaddr, &x);
if(count <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}

if(file_exist_op[1] == '6'){
  char errormes_file[512];
  count = recvfrom(mySocket, errormes_file, 512, 0, (struct sockaddr*) &servaddr, &x);
  if(count <= -1){
    cout << "Not Recived exiting \n";
    return -1;
  }
  packet_struc.ERROR[2] = "1";
  string temp(errormes_file);
  packet_struc.ERROR[1] = temp;
  // << packet_struc.ERROR[1] << "\n";
}
else{
int ack_numbs = 1;
int seg_num_ACK = 0;
int ior = 0;
while(coca == true){
char sBlast[1024];

//send acks
//cout << "enter loop \n";

int ack_len = 0;
//blocks
int send_block = block_ack;
string send_block_ack = to_string(send_block);
ack_len = send_block_ack.length();
char const *send_ACK_block = send_block_ack.c_str();
count = sendto(mySocket, send_ACK_block, ack_len, 0, (struct sockaddr*) &servaddr, x);
if(count == -1){
  cout<< "Did not send, Termintating Side \n";
  return -1;
}
//segments
string temp_seg_ack = to_string(seg_num_ACK);
ack_len = temp_seg_ack.length();
char const *send_ACK_seg = temp_seg_ack.c_str();
count = sendto(mySocket, send_ACK_seg, ack_len, 0, (struct sockaddr*) &servaddr, x);
if(count == -1){
  cout<< "Did not send, Termintating Side \n";
  return -1;
}
seg_num_ACK++;
packet_struc.ACK[ior][0] = "05"; // op

packet_struc.ACK[ior][1] = "0"; // session
packet_struc.ACK[ior][2] = send_block_ack; // op
packet_struc.ACK[ior][3] = temp_seg_ack; // session

char stale[2];
stale[0]= '0';
stale[1] = '5';
int mate;
char wasted_s[2];
count = recvfrom(mySocket, sBlast, 1024, 0, (struct sockaddr*) &servaddr, &x);
if(count <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}
mate = sendto(mySocket, stale, 2, 0, (struct sockaddr*) &servaddr, x);
if(mate == -1){
  cout<< "Did not send, Termintating Side \n";
  return -1;
}


string s3(sBlast);

packet_struc.DATA[ior][0] = "04";
packet_struc.DATA[ior][1] = to_string(randomsession);
packet_struc.DATA[ior][2] = to_string(upload_block);
packet_struc.DATA[ior][3] = to_string(ior);
packet_struc.DATA[ior][4] = s3;




if(count == 0){
  coca = false;
}
// seperates
char sep[2];

int sup = sendto(mySocket, sep, 2, 0, (struct sockaddr*) &servaddr, x);
if(sup == -1){
  cout<< "Did not send, Termintating Side \n";
  return -1;
}
//// << "initial "<< count << "\n";
string source_up = packet_struc.WRQ[2];
size_t torb = source_up.find(".txt");
//file name

if(torb != string::npos){
for (int i  = 0; i < count; i++){
  ch = sBlast[i];
   if (ch != '\0')
   {
   // // << "need to be here \n";
  inputfile += ch;
   }
  else{
    //// << "need to be here \n";
    coca = false;
  }

}
}
else{
  for (int i  = 0; i < count; i++){
  ch = sBlast[i];
  inputfile += ch;
  }
  if (count !=1024){
    coca = false;
  }
}

if(count != 1024){
  coca = false;
}
//// << "later "<< hwm << "\n";
hwm++;
ior++;
}

//cout << "\n chopper feel like a opper \n";
string source_up = packet_struc.WRQ[2];
size_t torb = source_up.find(".txt");
fstream downloads;
//cout << argv[3] << "+" << sourcefile << "\n";
//cout << tempos << "\n";
//cout << inputfile << "\n"; 
if(torb != string::npos){
  downloads.open(tempos, ios::out);
  }
  else{
  downloads.open(tempos,ios::binary|ios::out);
  } 

int LEN = inputfile.length();
cout << LEN << "\n";
//// << tempos << "\n";
for(int i = 0; i < LEN; i++){
    ch = inputfile.at(i);
  if (ch != '\0'){
  downloads.put(ch);
  }
  else{
    downloads.put(ch);
  }

}

}

}






else{ //begin dowload
  int upsend;
  int leave_it = 0;
//  cout << " File " << packet_struc.WRQ[2] << "is the value \n";
//for download to server
// << tempos << "\n";
  size_t torb = tempos.find(".txt");
  fstream upload;
  string length_file;
  ifstream file_exist;
  char sent_op[2];
  string souf = packet_struc.WRQ[2];
  if(torb != string::npos){
    file_exist.open(souf);
      if (file_exist){
      upload.open(souf, ios::in);
      sent_op[1] = '9';
      upsend = sendto(mySocket, sent_op, 2, 0, (struct sockaddr*) &servaddr, x);
      if(upsend == -1){
      cout<< "Did not send, Termintating Side \n";
      return -1;
      }
      }
      else{
        //// << "test \n";
        sent_op[0] = '0';
        sent_op[1] = '6';
        upsend = sendto(mySocket, sent_op, 2, 0, (struct sockaddr*) &servaddr, x);
        if(upsend == -1){
        cout<< "Did not send, Termintating Side \n";
        return -1;
        }
        string file_nf = "File not found";
        const char* fnf = file_nf.c_str();
        upsend = sendto(mySocket, fnf, 20, 0, (struct sockaddr*) &servaddr, x);
        if(upsend == -1){
        cout<< "Did not send, Termintating Side \n";
        return -1;
        }
        leave_it = 1;
      }
    
  }
  else{
    FILE *fpts;
    //fpt = fopen (sourcefile,"r");
    // << "HEllo " << sourcefile << "\n";
    const char* fp = sourcefile.c_str();
    file_exist.open(sourcefile, ios::binary|ios::in);
    if (fpts = fopen(fp,"r")){
      upload.open(sourcefile,ios::binary|ios::in);
        sent_op[1] = '9';
        upsend = sendto(mySocket, sent_op, 2, 0, (struct sockaddr*) &servaddr, x);
        if(upsend == -1){
        cout<< "Did not send, Termintating Side \n";
        return -1;
        }
    }
    else{
   
        sent_op[0] = '0';
        sent_op[1] = '6';
        upsend = sendto(mySocket, sent_op, 2, 0, (struct sockaddr*) &servaddr, x);
        if(upsend == -1){
        cout<< "Did not send, Termintating Side \n";
        return -1;
        }
        string file_nf = "File not found";
        const char* fnf = file_nf.c_str();
        upsend = sendto(mySocket, fnf, 20, 0, (struct sockaddr*) &servaddr, x);
        if(upsend == -1){
        cout<< "Did not send, Termintating Side \n";
        return -1;
        }
        leave_it = 1;
    }
  } 
//// << "We are download now " < << "\n";
// gets the whole file in a string;

if(leave_it != 1){
  while(!upload.eof()) {
  ch=upload.get();
    if(upload.eof()){

    }
  else{
  length_file+=ch; // carries the whole file in this string
  }
  }

int a = length_file.length();

 ////// << a;
//// << length_file << "\n";
// number of segments needed
int segements = 1;
  if(a > 1024){
    segements = a/1024;
  segements++;
  
  }
  if (a==1024){
    segements++;
  }

//// << a << "\n";
// << segements << "\n";

  string segement_data[segements];
int counter_sdc = 0;
string sd; // socket data
char sdc; //socket data char
int please = segements - 1;
//// << "\n The Segmenet Data is " << please << "\n";
int bufr = a - (please*1024);
//// << "the bufr size is \n" << bufr << "\n"; 
// << "Did not Die here ? \n";
if(a >= 1024){
  //// << "length of string is: "<< a << "\n";
int yc = 0;

for(int i = 0; i < segements ; i++){

int il = i *1024;


if(i == please){
sd = length_file.substr(il, 1024);
}
else{
sd = length_file.substr(il, 1024);
}
// << "The value of i here in line 401 is " << i << "\n";
segement_data[i] = sd;

packet_struc.DATA[i][0] = "04";
int joshin = i + 1;
packet_struc.DATA[i][1] = to_string(randomsession);
packet_struc.DATA[i][0] = upload_block;
packet_struc.DATA[i][3] = to_string(joshin);
packet_struc.DATA[i][4] = sd;

}
// << "1 \n";
string fm;
for(int i = 0; i < segements ; i++){
sd = segement_data[i];
int y = sd.length();
for(int j = 0; j < y; j++){
  ch = sd.at(j);
//downloads.put(ch);
}
fm += segement_data[i];
if(y != 1024)
{
//// << "located at : " << i << " value is " << y << "\n";
}
}
}
else{
  segement_data[counter_sdc] = length_file;
} 
// << "2 value is " << segements << "\n";

int count2 = 1;
for(int i = 0; i < segements; i++){

  string sa = to_string(count2);
  packet_struc.DATA[i][0] = "04";
  packet_struc.DATA[i][1] = packet_struc.ACK[i][1];
  packet_struc.DATA[i][2] = "1";
  packet_struc.DATA[i][3] = sa;
  packet_struc.DATA[i][4] = segement_data[i];
  count2++;
}
// << "Did not Die here \n";
//// << "THE AMOUTN OF SEGMENTS IS " << segements << "\n";
int ds = 0;
int elf = 0;
for(int i = 0; i < segements; i++){
  //cout << "the value of i is " << i <<" \n";
  string sf = packet_struc.DATA[i][4];
  ////// << sf << "\n";
  int q = sf.length();
  
  ds += q;
  // for txt file
  if (q != -1 && torb != string::npos){
    char sBlast[q];
  for(int m = 0; m < q; m++)
  {
    ch = sf.at(m);
    if (ch != '\0'){ 
    sBlast[m] = ch;
    
  }
   
  }
    char sep[2];
    upsend = sendto(mySocket, sBlast, q, 0, (struct sockaddr*) &servaddr, x);
      if(upsend == -1){
      cout<< "Did not send, Termintating Side \n";
      return -1;
      }
    int sup = recvfrom(mySocket, sep, 2, 0, (struct sockaddr*) &servaddr, &x);
    if(sup <= -1){
    cout << "Not Recived exiting \n";
    return -1;
    }
    packet_struc.ACK[i][0] = "05";
    packet_struc.ACK[i][1] = to_string(randomsession);
    packet_struc.ACK[i][2] = upload_block;
    packet_struc.ACK[i][3] = to_string(i);


    //// << upsend << "\n";
  }

  if (q != -1 && torb == string::npos){
    char sBlast[q];
  for(int m = 0; m < q; m++)
  {
    ch = sf.at(m);
    sBlast[m] = ch;
   
  }
  char sep[2];
    upsend = sendto(mySocket, sBlast, q, 0, (struct sockaddr*) &servaddr, x);
      if(upsend == -1){
      cout<< "Did not send, Termintating Side \n";
      return -1;
      }
    int sup = recvfrom(mySocket, sep, 2, 0, (struct sockaddr*) &servaddr, &x);
  if(sup <= -1){
  cout << "Not Recived exiting \n";
  return -1;
  }
    packet_struc.ACK[i][0] = "05";
    packet_struc.ACK[i][1] = to_string(randomsession);
    packet_struc.ACK[i][2] = upload_block;
    packet_struc.ACK[i][3] = to_string(i); 
    elf += upsend;
  }



}
  // << "3 \n";

}
}
cout << "done \n";
block_ack++;
close(mySocket); 
} 
//close(mySocket); 
return 0;
}

//close(mySocket); 
//return 0;







