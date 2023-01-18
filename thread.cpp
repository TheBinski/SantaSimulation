// ----------------------------------------------------------- 
// NAME : Caleb Melnychenko                                                    
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//           This class contains the implementations for 
//           the threads.
// -----------------------------------------------------------

#include "thread.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;

static SantaMonitor monitor("monitor");

Santa::Santa(int i, int r, bool *ret)
   :trips(i), deerTot(r), Retire(ret)
{

}

void Santa::ThreadFunc() {
   Thread::ThreadFunc();
   char buf[100];
   sprintf(buf, "Santa thread starts\n");
   write(1,buf,strlen(buf));

   int current = 1;
   while ( !Retire[0] ) {

      monitor.Sleep(deerTot);

      if ( monitor.deerBack(deerTot) ) {
         sprintf(buf, "Santa is preparing sleighs\n");
         write(1,buf,strlen(buf));
         monitor.RunDone(0,deerTot);
         Delay(1000);
         monitor.RunDone(1,deerTot);
         //flies off
         sprintf(buf, "The team flies off (%d)!\n",current);
         write(1,buf,strlen(buf));
         current++;
         if ( current >= (trips+1) ) {
            Retire[0] = true;
            break;
         }
         //releases deer
         monitor.RunDone(2,deerTot);
      }
      if ( monitor.elfQ() ) {
         sprintf(buf, "Santa is helping elves\n");
         write(1,buf,strlen(buf));

         Delay(1000);

         int e1 = monitor.getElf(1);
         int e2 = monitor.getElf(2);
         int e3 = monitor.getElf(3);

         sprintf(buf, "Santa answers the question posted by elves %d,%d,%d\n",
               e1,e2,e3);
         write(1,buf,strlen(buf));
         //releases elves
         monitor.releaseElves();
      }

   }
   Exit();
}

Reindeer::Reindeer(int i, int d, bool *ret)
   :ID(i), totalDeer(d), Retire(ret)
{

}

void Reindeer::ThreadFunc() {
   Thread::ThreadFunc();
   char buf[100];
   sprintf(buf, "     Reindeer %d starts\n", ID);
   write(1,buf,strlen(buf));

   while(1) {
      Delay(500);
      if ( Retire[0] ) {
         break;
      }
      monitor.ReindeerBack(ID, totalDeer);
      monitor.WaitOthers();

      monitor.WaitSleigh();
      monitor.FlyOff();

      Delay(500);
   }

   Exit();
}

Elves::Elves(int i, bool *ret)
   :ID(i), Retire(ret)
{

}

void Elves::ThreadFunc() {
   Thread::ThreadFunc();
   char buf[100];
   sprintf(buf, "          Elf %d starts\n", ID);
   write(1,buf,strlen(buf));

   while(1) {
      Delay(5000);
      if (Retire[0]) {
         break;
      }
      monitor.AskQuestion(ID);
      Delay(5000);
   }

   Exit();
}

//MONITOR CODE

SantaMonitor::SantaMonitor(char* Name): Monitor(Name,HOARE)
{
   //conditions 
   WakeMe = new Condition("WakeMe");
   Flying = new Condition("Flying");
   Waiting = new Condition("Waiting");
   Sleigh = new Condition("wSleigh");
   elfLineFull = new Condition("elfLineFull");
   elfLine[0] = new Condition("elf_1");
   elfLine[1] = new Condition("elf_2");
   elfLine[2] = new Condition("elf_3");
   //vars
   elfCount = 0;
   deerCount = 0;
   elfQueue[0] = elfQueue[1] = elfQueue[2] = 0;
}

void SantaMonitor::Sleep(int tot) {
   MonitorBegin();
      if ( !elfQ() && !deerBack(tot) ) { //if there are no questions
         WakeMe->Wait(); //and if the deer are not all back
      } //sleep
   MonitorEnd();
}

//returns if there are three elves currently asking a question
int SantaMonitor::elfQ() {
   if ( elfCount >= 3 ) {
      return 1;
   } else {
      return 0;
   }
}

int SantaMonitor::getElf(int i) {
   return elfQueue[i-1];
}

void SantaMonitor::releaseElves() {
   elfLine[0]->Signal();
   elfLine[1]->Signal();
   elfLine[2]->Signal();
   elfCount = 0;
   elfLineFull->Signal();
}

int SantaMonitor::deerBack(int total) {
   if ( deerCount >= total ) {
      return 1;
   } else {
      return 0;
   }
}

void SantaMonitor::AskQuestion(int i) {
   MonitorBegin();

      while ( elfCount >= 3 ) { //makes it recheck to avoid deadlocks
         elfLineFull->Wait();
      }

      char buf[100];
      sprintf(buf,"          Elf %d has a problem\n",i);
      write(1,buf,strlen(buf));

      elfQueue[elfCount] = i;
      elfCount++;
      
      if ( elfCount >= 3 ) {
         WakeMe->Signal();
         elfLine[elfCount%3]->Wait();
      } else {
         elfLine[elfCount%3]->Wait();
      }
      
     // elfLineFull->Signal();

   MonitorEnd();
}

void SantaMonitor::ReindeerBack(int i, int tot) {
   MonitorBegin();
      char buf[100];
      deerCount++;
      if ( deerCount < tot ) {
         sprintf(buf, "     Reindeer %d returns\n", i);
         write(1,buf,strlen(buf));
      } else {
         sprintf(buf, "     The last reindeer %d wakes up Santa\n", i);
         write(1,buf,strlen(buf));
         WakeMe->Signal();
      }
   MonitorEnd();
}

void SantaMonitor::RunDone(int s, int i) {
   MonitorBegin();
      if ( s == 0 ) { 
         for ( int j = 0; j < i; j++ ) {
            Waiting->Signal();
         }
      } else if ( s == 1 ) {
         for ( int j = 0; j < i; j++ ) {
            Sleigh->Signal();
         }
      } else if ( s == 2 ) {
         deerCount = 0;
         for ( int j = 0; j < i; j++ ) {
            Flying->Signal();
         }
         //deerCount = 0;
      }
   MonitorEnd();
}

void SantaMonitor::WaitOthers(){
   MonitorBegin();
      Waiting->Wait();   
   MonitorEnd();
}

void SantaMonitor::FlyOff() {
   MonitorBegin();
      //Sleigh->Signal();
      Flying->Wait();     
   MonitorEnd();
}

void SantaMonitor::WaitSleigh() {
   MonitorBegin();
      //Waiting->Signal();
      Sleigh->Wait();
   MonitorEnd();
}

