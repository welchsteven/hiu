#ifndef _SUBROUTINE_H
#define _SUBROUTINE_H

#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "packet.h"

struct Player
{
	int PlayerStr;
	int PlayerDex;
	int PlayerCon;
	int PlayerInt;
	int PlayerWis;
	int PlayerCha;
	int PlayerFort;
	int PlayerRef;
	int PlayerWill;
	int PlayerMaxHP;
	int PlayerAC;
	int PlayerMeleeHit;
	int PlayerRangedHit;
	int PlayerID;
	char *PlayerName;
	int PlayerTeam;
};

/*callback seems to return each line one by one*/

struct Player Hero;

/*static int callbackM(void *NotUsed, int argc, char **argv, char **azColName){



//	int i;



//  for(i=0; i<argc; i++)
//  {
//    printf("%s = %s, argument %d\n", azColName[i], argv[i] ? argv[i] : "NULL", i);
//  }
  printf("\n");
  Hero.PlayerStr=atoi(argv[0]);
  Hero.PlayerDex=atoi(argv[1]);
  Hero.PlayerCon=atoi(argv[2]);
  Hero.PlayerInt=atoi(argv[3]);
  Hero.PlayerWis=atoi(argv[4]);
  Hero.PlayerCha=atoi(argv[5]);
  Hero.PlayerFort=atoi(argv[6]);
  Hero.PlayerRef=atoi(argv[7]);
  Hero.PlayerWill=atoi(argv[8]);
  Hero.PlayerMaxHP=atoi(argv[9]);
  Hero.PlayerAC=atoi(argv[10]);
  Hero.PlayerMeleeHit=atoi(argv[11]);
  Hero.PlayerRangedHit=atoi(argv[12]);
  Hero.PlayerID=atoi(argv[13]);
  Hero.PlayerName=argv[14];
  Hero.PlayerTeam=atoi(argv[15]);




  return 0;
}

*/

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;

  int MyD20;
  int EnemyAC;
  int LoopCounter;
  int CritMin;
  int CritType;
  int DamageConstant;
  int DamageTotal;
  int DiceCount;
  int DiceMax;
  int DamageMultiplier;

  EnemyAC=12;
  DamageTotal=0;
  DamageMultiplier=1;
  CritMin=atoi(argv[0]);
  CritType=atoi(argv[1]);
  DamageConstant=atoi(argv[2]);
  DiceCount=atoi(argv[3]);
  DiceMax=atoi(argv[4]);
  printf("%d, I have captured this value\n", CritMin);
  for(i=0; i<argc; i++)
  {
    printf("%s = %s, argument %d\n", azColName[i], argv[i] ? argv[i] : "NULL", i);
  }
  printf("\n");
  srand((unsigned)time(0));
  //printf("\n\n%d\n\n",(unsigned)time(0));
  MyD20=rand()%20+1;
  printf("%d was the value of the d20.\n", MyD20);
  if (MyD20==1) {
	  printf("Critical Failure!\n");
  }
  if (MyD20 >= EnemyAC) {
       printf("The enemy was struck!\n");
       if (MyD20>=CritMin) {
    	   printf("Critical Threat!\n");
    	   MyD20=rand()%20+1;
    	   printf("Second Roll: %d\n", MyD20);
    	   if (MyD20>=EnemyAC) {
    		   printf("Critical Damage!\n");
    		   DamageMultiplier=2;
    	   }

       }
       LoopCounter=0;
       DamageTotal=0;
       printf("Damage Rolls:");
       while (LoopCounter < DiceCount) {
    	   LoopCounter=LoopCounter+1;
    	   MyD20=rand()%DiceMax+1;
    	   printf("%d ",MyD20);
    	   DamageTotal+=MyD20;

       }
       DamageTotal+=DamageConstant;
       printf("\n");
       //printf("%d was the value of LoopCounter\n", LoopCounter);
  }
  DamageTotal*=DamageMultiplier;
  printf("Damage Total was %d\n", DamageTotal);
  printf("DiceCount was %d\n",DiceCount);
  return 0;

}


int MeleeSwing(Packet NewPacket){
  /*Packet NewPacket;
  NewPacket.Player=1;
  NewPacket.Process=123;
  NewPacket.Team=1;
  NewPacket.ReturnValue=0;
  NewPacket.Subroutine=1;*/

  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *DatabaseAddress = "/home/frostwing/Desktop/test";
  //char *SelectSQL[180];
  char SelectSQL[180];

  if(NewPacket.Subroutine==1)
  {

	  int ret;
	  ret=sprintf(SelectSQL, "Select CritMin, CritType, DamageConstant, DiceCount, DiceMax, MagicEnhancement, ProcessID, ProcessName from Melee WHERE ProcessID=%d",NewPacket.Process);

	  printf ("(%s) is the result of our sprintf, which is %d characters long",SelectSQL,ret);
  }


  printf("clear\n");
  rc = sqlite3_open(DatabaseAddress, &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));

    sqlite3_close(db);
    exit(1);
  }
  //rc = sqlite3_exec(db, SelectSQL, callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK )
  {

    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }


  sqlite3_close(db);
  //char *SelectSQL;
  //SelectSQL="Select PlayerStr, PlayerDex, PlayerCon, PlayerInt, PlayerWis, PlayerCha, PlayerFort, PlayerRef, PlayerWill, PlayerMaxHP, PlayerAC, PlayerMeleeHit, PlayerRangedHit, PlayerID, PlayerName, PlayerTeam from Players";
  rc = sqlite3_open(DatabaseAddress, &db);
  printf("Working!!!!\n");
  rc = sqlite3_exec(db, SelectSQL, callback, 0, &zErrMsg);
  sqlite3_close(db);



  //printf("Final Value test is %d\n",Hero.PlayerStr);

  return 0;
}


#endif
