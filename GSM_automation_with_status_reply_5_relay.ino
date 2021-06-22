#include <EEPROM.h>
#include <SoftwareSerial.h> //Create software serial object to communicate with SIM800L
SoftwareSerial GSM(8, 9);   //Connected SIM800L Tx-to ArduinoPin#8,SIM800L Rx to Arduino Pin#9
//Note:-Do not use Digital Pin 0 and 1 in ths project to use as a digital output.because they are default RX and TX of Arduino.
String phone_no1 = "+971557407961"; //change +971 with country code and 7407961 with phone number to sms
String phone_no2 = "Enter Number2"; 

String        RxString  = ""; // Will hold the incoming String  from the GSM shield
char          RxChar    = ' ';
int           Counter   = 0;
String        GSM_Nr    = "";
String        GSM_Msg   = "";

#define Relay1 2 //Connected Relay1 to Digital Pin 2 of Arduino
#define Relay2 3 //Connected Relay2 to Digital Pin 3 of Arduino
#define Relay3 4 //Connected Relay3 to Digital Pin 4 of Arduino 
#define Relay4 5 //Connected Relay4 to Digital Pin 5 of Arduino
#define Relay5 6 //Connected Relay5 to Digital Pin 6 of Arduino
int load1, load2, load3, load4,load5;

void setup(){ // put your setup code here, to run once

pinMode(Relay1, OUTPUT); digitalWrite(Relay1, 1); 
pinMode(Relay2, OUTPUT); digitalWrite(Relay2, 1); 
pinMode(Relay3, OUTPUT); digitalWrite(Relay3, 1); 
pinMode(Relay4, OUTPUT); digitalWrite(Relay4, 1); 
pinMode(Relay5, OUTPUT); digitalWrite(Relay5, 1); 
Serial.begin(9600);//Begin serial communication with Arduino and Arduino IDE (Serial Monitor)  
GSM.begin(9600);   //Begin serial communication with Arduino and SIM800L

Serial.println("Initializing....");
initModule("AT","OK",1000);                //Scan for GSM Module
initModule("AT+CPIN?","READY",1000);       //this command is used to check whether SIM card is inserted in GSM Module or not
initModule("AT+CMGF=1","OK",1000);         //Set SMS mode to ASCII
initModule("AT+CNMI=2,2,0,0,0","OK",1000); //Set device to read SMS if available and print to serial
Serial.println("Initialized Successfully"); 
  
load1 = EEPROM.read(1);
load2 = EEPROM.read(2);
load3 = EEPROM.read(3);
load4 = EEPROM.read(4);
load5 = EEPROM.read(5);
relays();

delay(100);
}

void loop(){
  
  // scan for data from software serial port
  //-----------------------------------------------
  RxString = "";
  Counter = 0;
  while(GSM.available()){
    delay(1);  // short delay to give time for new data to be placed in buffer
    // get new character
    RxChar = char(GSM.read());
    //add first 200 character to string
    if (Counter < 200) {
      RxString.concat(RxChar);
      Counter = Counter + 1;
    }
  }
 
  // Is there a new SMS?
  //-----------------------------------------------
  if (Received(F("CMT:")) ) GetSMS();

if(GSM_Nr==phone_no1 || GSM_Nr==phone_no2){
  
if(GSM_Msg=="Sok1on") {load1=1; sendSMS(GSM_Nr,"Ok Socket 1 is On");}//send message-Sok1on-to Turn on the Socket1
if(GSM_Msg=="Sok1off"){load1=0; sendSMS(GSM_Nr,"Ok Socket1 is Off");}//send message-Sok1off-to Turn off the Socket1

if(GSM_Msg=="Sok2on") {load2=1; sendSMS(GSM_Nr,"Ok Socket 2 is On");}//send message-Sok2o- to Turn on the Socket2
if(GSM_Msg=="Sok2off"){load2=0; sendSMS(GSM_Nr,"Ok Socket 2 is Off");}//send message-Sok2off-to Turn off the Socket2

if(GSM_Msg=="Sok3on") {load3=1; sendSMS(GSM_Nr,"Ok Socket 3 is On");}//send message-Sok3on-to Turn on the Socket3
if(GSM_Msg=="Sok3off"){load3=0; sendSMS(GSM_Nr,"Ok Socket 3 is Off");}//send message-Sok3off-to Turn off the Socket3

if(GSM_Msg=="Sok4on") {load4=1; sendSMS(GSM_Nr,"Ok Socket 4 is On");}//send message -ok4on-to Turn on the Socket4
if(GSM_Msg=="Sok4off"){load4=0; sendSMS(GSM_Nr,"Ok Socket 4 is Off");}//send message-Sok4off-to Turn off the Socket4

if(GSM_Msg=="Sok5on") {load5=1; sendSMS(GSM_Nr,"Ok Socket 5 is On");}//send message-Sok5on-to Turn on the Socket5
if(GSM_Msg=="Sok5off"){load5=0; sendSMS(GSM_Nr,"Ok Socket 5 is Off");}//send message-Sok5off-to Turn off the Socket5

if(GSM_Msg=="Onallsok") {load1=1, load2=1, load3=1, load4=1,load5=1; sendSMS(GSM_Nr,"Ok All Sockets are On");}
if(GSM_Msg=="Offallsok"){load1=0, load2=0, load3=0, load4=0,load5=0; sendSMS(GSM_Nr,"Ok All Sockets are Off");}
//Send message-Onallsok-to Turn on all sockets together,Send message-Offallsok-to Turn off all sockets together.
if(GSM_Msg=="Socketstatus"){//Send a message- Socketstatus- to check status of all sockets.
String loadst = "";

if(load1==1){loadst="Socket1 On\r\n";}
        else{loadst="Socket1 Off\r\n";}
        
if(load2==1){loadst=loadst + "Socket2 On\r\n";}
        else{loadst=loadst + "Socket2 Off\r\n";}
        
if(load3==1){loadst=loadst + "Socket3 On\r\n";}
        else{loadst=loadst + "Socket3 Off\r\n";}
        
if(load4==1){loadst=loadst + "Socket4 On\r\n";}
        else{loadst=loadst + "Socket4 Off\r\n";}
        
if(load5==1){loadst=loadst + "Socket5 On";}
        else{loadst=loadst + "Socket5 Off";}
                
sendSMS(GSM_Nr,loadst);
}

eeprom_write();
relays();
}


GSM_Nr="";
GSM_Msg="";
}

void eeprom_write(){
EEPROM.write(1,load1);
EEPROM.write(2,load2);
EEPROM.write(3,load3);
EEPROM.write(4,load4);  
EEPROM.write(5,load5);
}

void relays(){  
digitalWrite(Relay1, load1); 
digitalWrite(Relay2, load2); 
digitalWrite(Relay3, load3); 
digitalWrite(Relay4, load4); 
digitalWrite(Relay5, load5);
}

// Send SMS 
void sendSMS(String number, String msg){
GSM.print("AT+CMGS=\"");GSM.print(number);GSM.println("\"\r\n"); //AT+CMGS=”Mobile Number” <ENTER> - Assigning recipient’s mobile number
delay(500);
GSM.println(msg); // Message contents
delay(500);
GSM.write(byte(26)); //Ctrl+Z  send message command (26 in decimal).
delay(5000);  
}

// Get SMS Content
void GetSMS() {
  //Get SMS number
  //================================================
  GSM_Nr  = RxString;
  //get number
  int t1 = GSM_Nr.indexOf('"');
  GSM_Nr.remove(0,t1 + 1);
  t1 = GSM_Nr.indexOf('"');
  GSM_Nr.remove(t1);
   
  // Get SMS message
  //================================================
  GSM_Msg = RxString;
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  GSM_Msg.remove(0,1);
  GSM_Msg.trim();

Serial.print("Number:"); Serial.println(GSM_Nr);
Serial.print("SMS:"); Serial.println(GSM_Msg);
}

// Search for specific characters inside RxString 
boolean Received(String S) {
  if (RxString.indexOf(S) >= 0) return true; else return false;
}


// Init GSM Module 
void initModule(String cmd, char *res, int t){
while(1){
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while(GSM.available()>0){
       if(GSM.find(res)){
        Serial.println(res);
        delay(t);
        return;
       }else{Serial.println("Error");}}
    delay(t);
  }
}
