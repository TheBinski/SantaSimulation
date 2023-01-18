// ----------------------------------------------------------- 
// NAME : Caleb Melnychenko                                                  
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//           This class has the framework for the threads
//           and the monitor.
// -----------------------------------------------------------

#ifndef _THREAD_H
#define _THREAD_H

#include "ThreadClass.h"

class Santa : public Thread {
   public:
      Santa(int i, int r, bool *ret); //constructor

   private:
      int trips;
      int deerTot;
      void ThreadFunc();
      bool *Retire;
      
};

class Elves : public Thread {
   public:
      Elves(int i, bool *ret);

   private:
      int ID;
      void ThreadFunc();
      bool *Retire;

};

class Reindeer : public Thread {
   public:
      Reindeer(int i, int d, bool *ret); //constructor
   private:
      int ID;
      int totalDeer;
      void ThreadFunc();
      bool *Retire;

};

class SantaMonitor : public Monitor {
   public:
      SantaMonitor(char* Name); //constructor
      //Santa methods
      void Sleep(int tot);
      int deerBack(int total);
      int elfQ();
      int getElf(int i);
      void releaseElves();
      void waitDone();
      void sleighDone();
      void RunDone(int s, int i);
      //Elves methods
      void AskQuestion(int i);
      //Reindeer methods
      void ReindeerBack(int i, int tot);
      void WaitOthers();
      void FlyOff();
      void WaitSleigh();
   private:
      Condition *WakeMe;
      Condition *elfLineFull;
      int elfCount;
      Condition *elfLine[3];
      int elfQueue[3];
      int deerCount;
      Condition *Flying;
      Condition *Waiting;
      Condition *Sleigh;

};

#endif
