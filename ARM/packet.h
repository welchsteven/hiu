#ifndef _PAKCET_H
#define _PAKCET_H

//Packet.h
// this header file contains the subroutines to create and handle packets 
//    that interact with the HIU(Hardware Interface Unit)
//
// Author: Jonathan Holden
// Date: 10/16/2008
// Compliant with Developer Code Version 0.3
//    do not use older developer code as some functionality has changed
//
//
//
// Incoming_Packet_Parser(Current_Packet[8])
//
// This subroutine will take an incoming packet from the global array (Current_Packet[8])
// and break it into the correct parts and place it in the global variables.
// it will then perform a checksum on the data to insure that the data is not corrupted.
//
//
// PACKET_BUILDER(int TeamID, int16 PlayerID, int SubID, int16 ProcessID, int ReturnID)
// 
// This routine takes the values in the global variables and forms them into a packet that is ready for communication
// The packet is stored in Packet_Buffer[8]
//
// Packet_Input()
//
// This routine will query the user for information and place it in the global variables
// The IO stream that you are printing to must be named USB
//
//
// Print_Packet()
//
// This routine will print the current packet that is in Packet_Buffer[8]
// The IO stream that you are printing to must be named USB


typedef int int16;
typedef unsigned char int8;


#define	IRCOMMID	0
#define	RFCOMMID	1
#define	RFIDCOMMID	2
#define	CANCOMMID	3

typedef struct 
{ 
	unsigned char COM;
	int8 Team;
	int16 Player;
	int8 Subroutine;
	int16 Process;
	int16 ReturnValue;
	int8 Checksum;
}Packet; 

//int8 Current_Packet[8];
int8 Packet_Buffer[8];

//subroutines
Packet Incoming_Packet_Parser(int8 Current_Packet[],int size);
int PACKET_BUILDER(int TeamID, int PlayerID, int SubID, int ProcessID, int ReturnID);
Packet Packet_Input();
void Print_Packet(int * buffer); // modify for 8 & 9 byte packets use size of array
int Integrity_Check(Packet currentpacket);

Packet Incoming_Packet_Parser(int8 Current_Packet[], int size)
{
   Packet packetinput;

   packetinput.COM = 0;
   packetinput.Team = 0;
   packetinput.Player = 0;
   packetinput.Subroutine = 0;
   packetinput.Process = 0;
   packetinput.ReturnValue = 0;
   packetinput.Checksum = 0;


   if (size==8)
   {
	   packetinput.COM = 255;
	   packetinput.Team = (Current_Packet[0]);
	   packetinput.Player = Current_Packet[1];
	   packetinput.Player = packetinput.Player*16;
	   packetinput.Player = packetinput.Player + ((Current_Packet[2]&0b11110000)/16);
	   packetinput.Subroutine = (Current_Packet[2]&0b00001111);
	   packetinput.Process = (Current_Packet[3]);
	   packetinput.Process = packetinput.Process*256 + Current_Packet[4];
	   packetinput.ReturnValue = (Current_Packet[5]*256) + (Current_Packet[6]);
	   packetinput.Checksum = Current_Packet[7];
   }
   if (size==9)
   {
	   packetinput.COM = (Current_Packet[0]);
	   packetinput.Team = (Current_Packet[1]);
	   packetinput.Player = Current_Packet[2];
	   packetinput.Player = packetinput.Player*16;
	   packetinput.Player = packetinput.Player + ((Current_Packet[3]&0b11110000)/16);
	   packetinput.Subroutine = (Current_Packet[3]&0b00001111);
	   packetinput.Process = (Current_Packet[4]);
	   packetinput.Process = packetinput.Process*256 + Current_Packet[5];
	   packetinput.ReturnValue = (Current_Packet[6]*256) + (Current_Packet[7]);
	   packetinput.Checksum = Current_Packet[8];
   }
   return packetinput;
}



int Integrity_Check(Packet currentpacket)
{
	int8 IntegrityCHK = 0;
	IntegrityCHK = currentpacket.Team+currentpacket.Player+currentpacket.Subroutine+currentpacket.Process+currentpacket.ReturnValue;
	if((currentpacket.Checksum - IntegrityCHK) != 0)
    {
      //Subroutine_ID = 0x00;
		return 0;
    }

	else
		return 1;
}

int PACKET_BUILDER(int TeamID, int16 PlayerID, int SubID, int16 ProcessID, int16 ReturnID)
{
   int chk;
   int Packet_Buffer[8];
   
   Packet_Buffer[0]=TeamID;
   Packet_Buffer[1]=((PlayerID&0b0000111111110000)/16);
   Packet_Buffer[2]=((PlayerID&0b0000000000001111)*16);
   Packet_Buffer[2]=Packet_Buffer[2] + (SubID&0b00001111);
   Packet_Buffer[3]=((ProcessID&0b1111111100000000)/256);   
   Packet_Buffer[4]=((ProcessID&0b0000000011111111));      
   Packet_Buffer[5]=((ReturnID&0b1111111100000000)/256);
   Packet_Buffer[6]=((ReturnID&0b0000000011111111));
   chk = (TeamID)+(PlayerID&0b0000111111111111)+(SubID&0b00001111)+ProcessID+ReturnID;
   Packet_Buffer[7]=chk;

   return Packet_Buffer;
}

//Packet Packet_Input()
//{
//   Packet packetinput;
//   char* temp[6];
//
//   fprintf(USB,"\n\r Please enter Team ID\n\r");
//   fgets(temp, USB);
//   if (temp[strlen(temp)-1] == '\n')
//      temp[strlen(temp)-1] = '\0';
//   packetinput.Team = (int8)strtoul(temp,null,0);
//   fprintf(USB,"  %u  ",packetinput.Team);
//
//   fprintf(USB,"\n\r Please enter Player ID\n\r");
//   fgets(temp, USB);
//   if (temp[strlen(temp)-1] == '\n')
//      temp[strlen(temp)-1] = '\0';
//   packetinput.Player = strtoul(temp,null,0);
//   fprintf(USB,"  %lu  ",packetinput.Player);
//
//   fprintf(USB,"\n\r Please enter Subroutine ID\n\r");
//   fgets(temp, USB);
//   if (temp[strlen(temp)-1] == '\n')
//      temp[strlen(temp)-1] = '\0';
//   packetinput.Subroutine = (int8)strtoul(temp,null,0);
//   fprintf(USB,"  %u  ",packetinput.Subroutine);
//
//   fprintf(USB,"\n\r Please enter Process ID\n\r");
//   fgets(temp, USB);
//   if (temp[strlen(temp)-1] == '\n')
//      temp[strlen(temp)-1] = '\0';
//   packetinput.Process = strtoul(temp,null,0);
//   fprintf(USB,"  %lu  ",packetinput.Process);
//
//   fprintf(USB,"\n\r Please enter Return Value\n\r");
//   fgets(temp, USB);
//   if (temp[strlen(temp)-1] == '\n')
//      temp[strlen(temp)-1] = '\0';
//   packetinput.ReturnValue = strtoul(temp,null,0);
//   fprintf(USB,"  %lu  ",packetinput.ReturnValue);
//
//   return packetinput;
//}

//void Print_Packet(int * buffer)
//{
//   fprintf(USB,"\n\r  current packet  \n\r");
//   fprintf(USB," %u  %u  %u  %u  %u  %u  %u  %u \n\r",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
//
//}


#endif

