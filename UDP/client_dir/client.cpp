#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sstream> 
#include <fstream>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#define _OE_SOCKETS
#include <sys/types.h>
#define _XOPEN_SOURCE_EXTENDED 1
#include <sys/socket.h>
#include <string> 
#include <iostream>
#include <cstdlib>
#include<fstream>
#include <sys/time.h>

#include <cstring>
#include<bits/stdc++.h>
#include <cctype>

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
string DATA[9000][5] = {"04","","", "", ""}; // acutal segement data (1024 charecters)
string ACK[9000][4] = {"05","", "", ""};
string ERROR[3] = {"06", "", ""};
};





//./eftpclient [username:password@ip:port] [upload/download] [filename]
//./eftpclient username:password@127.0.0.1:2007 upload test.txt
int main(int argc, char** argv){
  char ch; 
  char opcode[2];
  int block_ack = 1;
  //true, false, upload, recive
  string tfur = argv[2];
  transform(tfur.begin(), tfur.end(), tfur.begin(), ::toupper);

  // credentials VS Connections
  string cred_con = argv[1];

  //serches for the last case of '@'
  
  int cred_con_sep = 0;
  for(int loc= 0; loc < cred_con.length(); loc++){
    char atc = cred_con[loc];
    if(atc == '@'){
      cred_con_sep = loc;
    }
  }
  ////cout << "Location is at \n" << cred_con_sep <<"\n";

string user_pass;
  for(int loc= 0; loc < cred_con_sep; loc++){
    char atc = cred_con[loc];
    user_pass+=atc;
  }

	string password = "";
	string username = "";

int trig = 0;
  for(int loc= 0; loc < user_pass.length(); loc++){

    char atc = cred_con[loc];
    if(atc == ':'){
      trig = 1;
    }
    if(trig==1){
      password+=atc;
    }
    else{
      username+=atc;
    }


  }

/*   //cout << "username is \n" << username <<"\n";
  //cout << "password is \n" << password <<"\n"; */
string recvr = "DOWNLOAD";
string sendr = "UPLOAD";

struct EFTP packet_struc;
bool send_recv; 
string ts = argv[3]; 


if (tfur==sendr){
  send_recv = true; 
  
  packet_struc.WRQ[2] = ts;
  packet_struc.RRQ[2] = ts;
  packet_struc.WRQ[3] = "1";
  packet_struc.RRQ[3] = "0";
}
if (tfur==recvr){
  send_recv = false; 

  packet_struc.WRQ[2] = ts;
  packet_struc.RRQ[2] = ts;
  packet_struc.WRQ[3] = "0";
  packet_struc.RRQ[3] = "1";

}


  string ip_port;
  for(int loc= cred_con_sep + 1; loc < cred_con.length(); loc++){
    char atc = cred_con[loc];
    ip_port+=atc;
  }
  ////cout << ip_port <<"\n";


  string portn ="";
  string ips = "";

  trig = 0;
  for(int loc= 0; loc < ip_port.length(); loc++){

    char atc = ip_port[loc];
    if(atc == ':'){
      trig = 1;
      loc++;
      atc = ip_port[loc];
    }
    if(trig==1){
      portn+=atc;
    }
    else{
      ips+=atc;
    }
  }

//  cout << "ip is " << ips <<"\n";
//  cout << "port is " << portn <<"\n"; 


string sourcefile = argv[3]; 
packet_struc.AUTH[1] = username;
packet_struc.AUTH[3] = password;

  //** ^ COMMAND LINE OPTIONS ^ **//



 struct sockaddr_in servaddr;
  
  int port_s;
  port_s = stoi(portn);
  //port = 2007;
//cout << ":" << port_s << ":\n";
  
  char user_ip[ips.length()];

  for(int n = 0; n < ips.length(); n++){
    user_ip[n]+=ips.at(n);
  }
const char* final_ips = ips.c_str();
unsigned short final_port = (unsigned short) port_s;
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
  unsigned int x8 = sizeof(servaddr);
 servaddr.sin_port = htons(final_port);
 servaddr.sin_addr.s_addr = inet_addr(final_ips);
// servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
 int mySocket;
 mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
 if(mySocket <= 0){
 cout << "Could not setup a socket \n" ;
 return -1;
 }


string pass_user= username + password;
int x = sizeof(servaddr);
int pass_ser_len = pass_user.length();
//cout<< pass_ser_len << "\n";
char upass_pass[pass_ser_len] = "";

for(int m = 0; m < pass_ser_len; m++){
  ch = pass_user.at(m);
  upass_pass[m] += ch;
  //cout << upass_pass[m] << "\n";
}

char errormes[512];
socklen_t xy = sizeof(servaddr);


//send username password
int trial_q = sendto(mySocket, upass_pass, pass_ser_len, 
                   0, (struct sockaddr*) &servaddr, x);
if(trial_q == -1){
  cout<< "Did not send, Termintating Client Side \n";
  return -1;
}

int upsend = recvfrom(mySocket, opcode, 2, 0, (struct sockaddr*) &servaddr, &xy);
if(upsend <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}


//cout << "the valie is " <<opcode[1] << "\n";

//cout << "TEST" << "\n";
    if(opcode[1] == '6'){
    upsend = recvfrom(mySocket, errormes, 512, 0, (struct sockaddr*) &servaddr, &xy);
    if(upsend <= -1){
    cout << "Not Recived exiting \n";
    return -1;
    }
    string s(errormes);
  packet_struc.ERROR[2] = "1";
  packet_struc.ERROR[1] = s;
    puts(errormes);
   return -1;
    }

  string opcs(opcode);    
  char randsesh[5];
  //cout << "Sent an Ack \n";
  upsend = recvfrom(mySocket, randsesh, 5, 0, (struct sockaddr*) &servaddr, &xy);
  if(upsend <= -1){
  cout << "Not Recived exiting \n";
  return -1;
  } 
  //cout << upsend << "\n";
  string randsh(randsesh);  
  char block[2];
  //upsend = recvfrom(mySocket, block, 2, 0, (struct sockaddr*) &servaddr, &xy);
  //cout << upsend << "\n";
  string ublock(block);
  char segn[4];
  
  //upsend = recvfrom(mySocket, segn, 4, 0, (struct sockaddr*) &servaddr, &xy);
  string usegn(segn);
  //cout << upsend << "\n";

  //cout <<"Broke out \n";
          
/*   packet_struc.ACK[0][0] = opcs;
  packet_struc.ACK[0][1] = randsesh;
  packet_struc.ACK[0][2] = ublock;
  packet_struc.ACK[0][3] = usegn;  */
      
  //cout << "TEST4" << "\n";
  opcode[1] = '3';
   opcode[0] = '0';
  if (send_recv == true)
  {
    opcode[1] = '3';
  }
  else{
    opcode[1] = '2';
  }
    int op = sendto(mySocket, opcode, 2, 
                    0, (struct sockaddr*) &servaddr, x);
    if(op == -1){
  cout<< "Did not send, Termintating Client Side \n";
  return -1;
}
    //cout << "TEST3" << "\n";
  //  puts(opcode);
  

//cout << "TEST4" << "\n";
//cout << "\n there " << sourcefile << " is it \n";
    int irw = sourcefile.length();
    char sourcefile_ch[irw];
    for(int i = 0;  i < irw; i++){
      sourcefile_ch[i] = sourcefile.at(i);
    }
//sending the file name
  upsend = sendto(mySocket, sourcefile_ch, irw, 0, 
		   (struct sockaddr*) &servaddr, x);
  if(upsend == -1){
  cout<< "Did not send, Termintating Client Side \n";
  return -1;
}
 

    //cout << "TEST1" << "\n";

    
//cout << "TEST4" << "\n";



//cout << "\n enterning the loop \n";

//upload
 if (send_recv==true){
  size_t torb = sourcefile.find(".txt");
  fstream upload;
  string length_file;
  ifstream file_exist;
  char sent_op[2];
//  cout << sourcefile;
  if(torb != string::npos){
    file_exist.open(sourcefile);
      if (file_exist){
      upload.open(sourcefile, ios::in);
      sent_op[1] = '9';
      upsend = sendto(mySocket, sent_op, 2, 0, (struct sockaddr*) &servaddr, x);
      if(upsend == -1){
  cout<< "Did not send, Termintating Client Side \n";
  return -1;
}
      }
      else{
//        cout << "test \n";
        sent_op[0] = '0';
        sent_op[1] = '6';
        upsend = sendto(mySocket, sent_op, 2, 0, (struct sockaddr*) &servaddr, x);
        if(upsend == -1){
        cout<< "Did not send, Termintating Client Side \n";
        return -1;
        }
        string file_nf = "File not found";
        const char* fnf = file_nf.c_str();
        upsend = sendto(mySocket, fnf, 20, 0, (struct sockaddr*) &servaddr, x);
        if(upsend == -1){
        cout<< "Did not send, Termintating Client Side \n";
        return -1;
        }
        return -1;
      }
    
    
  }
  else{
    FILE *fpt;
    //fpt = fopen (sourcefile,"r");
    const char* fp = sourcefile.c_str();
    file_exist.open(sourcefile, ios::binary|ios::in);
    if (fpt = fopen(fp,"r")){
      upload.open(sourcefile,ios::binary|ios::in);
        sent_op[1] = '9';
        upsend = sendto(mySocket, sent_op, 2, 0, (struct sockaddr*) &servaddr, x);
        if(upsend == -1){
        cout<< "Did not send, Termintating Client Side \n";
        return -1;
        }
    }
    else{
      //cout << "test \n";
        sent_op[0] = '0';
        sent_op[1] = '6';
        upsend = sendto(mySocket, sent_op, 2, 0, (struct sockaddr*) &servaddr, x);
        if(upsend == -1){
        cout<< "Did not send, Termintating Client Side \n";
        return -1;
        }
        string file_nf = "File not found";
        const char* fnf = file_nf.c_str();
        upsend = sendto(mySocket, fnf, 20, 0, (struct sockaddr*) &servaddr, x);
        if(upsend == -1){
        cout<< "Did not send, Termintating Client Side \n";
        return -1;
        }
        return -1;
    }
  } 


if(sent_op[1] == '9'){
//  cout << sourcefile << " I am in \n";
  while(!upload.eof()) {
  ch=upload.get();

    if(upload.eof()){
//cout << " I am in \n";
    }
  else{
  length_file+=ch; // carries the whole file in this string
  }
  }

 int a = length_file.length();
 cout << a << " is the length \n";

// number of segments needed
int segements = 1;
  if(a > 1024){
    segements = a/1024;
  // if ((a%1024)!=0){ 
  segements+=1;
  //} 
  } 
  if(a == 1024){
    segements++;
  }
  
//  cout << segements << " is the amount of times I send \n";


// added 
string segement_data[segements];
int counter_sdc = 0;
string sd; // socket data
char sdc; //socket data char
int please = segements - 1;
//cout << "\n The Segmenet Data is " << please << "\n";
int bufr = a - (please*1024);
//cout << "the bufr size is \n" << bufr << "\n";
 

if(a >= 1024){
  //cout << "length of string is: "<< a << "\n";
int yc = 0;

for(int i = 0; i < segements ; i++){

int il = i *1024;


if(i == please){
sd = length_file.substr(il, 1024);
}
else{
sd = length_file.substr(il, 1024);
}

segement_data[i] = sd;
}
string fm;
for(int i = 0; i <segements ; i++){
sd = segement_data[i];
int y = sd.length();
for(int j = 0; j < y; j++){
  ch = sd.at(j);
//downloads.put(ch);
}
fm += segement_data[i];
if(y != 1024)
{
//cout << "located at : " << i << " value is " << y << "\n";
}
}
}
else{
  segement_data[counter_sdc] = length_file;
} 


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

//cout << "THE AMOUTN OF SEGMENTS IS " << segements << "\n";
int ds = 0;
int elf = 0;
int seg_data = 1;
for(int i = 0; i < segements; i++){
  
  string sf = packet_struc.DATA[i][4];
  ////cout << sf << "\n";
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
    cout<< "Did not send, Termintating Client Side \n";
    return -1;
    }
    //upsend = sendto(mySocket, seq, 2, 0, (struct sockaddr*) &servaddr, x);
    //cout << upsend << "\n";
  }
  
  if (q != -1 && torb == string::npos){
    char sBlast[q];
  for(int m = 0; m < q; m++)
  {
    ch = sf.at(m);
    sBlast[m] = ch;
   
  }

// recv ack
packet_struc.ACK[i][0] = "05"; // op

packet_struc.ACK[i][1] = "0"; // session
packet_struc.ACK[i][3]; // segments
 char tone[2];
 int send1 = recvfrom(mySocket, tone, 2, 0, (struct sockaddr*) &servaddr, &xy);
 if(send1 <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}
char ttwo[2];
 int send2 = recvfrom(mySocket, ttwo, 2, 0, (struct sockaddr*) &servaddr, &xy);
if(send2 <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}
string u(tone);
packet_struc.ACK[i][2] = "1";
string v(ttwo);
packet_struc.ACK[i][3] = v;


  char sep[2];
  int sup;

  seg_data++; 
  //sending data 
    upsend = sendto(mySocket, sBlast, q, 0, (struct sockaddr*) &servaddr, x);
    if(upsend == -1){
    cout<< "Did not send, Termintating Client Side \n";
    return -1;
    }
    sup = recvfrom(mySocket, sep, 2, 0, (struct sockaddr*) &servaddr, &xy);
    if(sup <= -1){
   cout << "Not Recived exiting \n";
   return -1;
    }
    elf += upsend;
  }
}
cout << "The value of a is: " << a << " The value of ds is: " << ds << "\n";
cout << "The value of a is: " << a << " The value of A is: " << elf << "\n";
 }

} 






else{
  bool coca = true;
string inputfile;
//cout << "IM Finally in download here \n";
string sourcefile_upload = "";

char file_exist_op[2];
int tcount = recvfrom(mySocket, file_exist_op, 2, 0, (struct sockaddr*) &servaddr, &xy);
if(tcount <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}

if(file_exist_op[1] == '6'){
  char errormes_file[512];
  tcount = recvfrom(mySocket, errormes_file, 512, 0, (struct sockaddr*) &servaddr, &xy);
  if(tcount <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}
  packet_struc.ERROR[2] = "1";
  string temp(errormes_file);
  packet_struc.ERROR[1] = temp;
//  cout << packet_struc.ERROR[1] << "\n";
  return -1;
}
else{
// works for txt.
int hwm = 0;
while(coca == true){
char sBlast[1024];

int count = recvfrom(mySocket, sBlast, 1024, 0, (struct sockaddr*) &servaddr, &xy);
if(count <= -1){
  cout << "Not Recived exiting \n";
  return -1;
}

if(count == 0){
  coca = false;
}
// seperates
char sep[2];
sep[0] = '0';
sep[1] = '5';
int sup = sendto(mySocket, sep, 2, 0, (struct sockaddr*) &servaddr, x);
if(sup == -1){
  cout<< "Did not send, Termintating Client Side \n";
  return -1;
}
//cout << "initial "<< count << "\n";
string source_up = packet_struc.WRQ[2];
size_t torb = source_up.find(".txt");
//file name
if(torb != string::npos){
for (int i  = 0; i < count; i++){
  ch = sBlast[i];
   if (ch != '\0')
   {
   // cout << "need to be here \n";
  inputfile += ch;
   }
  else{
    //cout << "need to be here \n";
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
//cout << "later "<< hwm << "\n";
hwm++;

}
//cout << inputfile << "\n";
//cout << "\n chopper feel like a opper \n";

fstream downloads;
string source_up = packet_struc.WRQ[2];
size_t torb = source_up.find(".txt");
if(torb != string::npos){
  downloads.open(ts, ios::out);
  }
  else{
  downloads.open(ts,ios::binary|ios::out);
  } 

//cout << ":" << ts <<  ": \n";

int LEN = inputfile.length();
cout << LEN << "\n";
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

//cout << send_recv << "\n";  

close(mySocket);


}

