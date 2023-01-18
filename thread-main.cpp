// ----------------------------------------------------------- 
// NAME : Caleb Melnychenko                                                    
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//            Uses threads Threads to simulate Santa, the reindeer,
//            and the elves preparing for Christmas. When everyone
//            is ready then Santa takes off for Christmas.
// -----------------------------------------------------------

#include <iostream>
#include "thread.h"
#include <stdio.h>
#include <string.h>
using namespace std;

int main ( int argc, char* argv[] ) {
   
   if ( argc < 4 ) {
      cout << "You need to run it as './prog5 e r t'" << endl;
      cout << "e = number of elves" << endl;
      cout << "r = number of reindeer" << endl;
      cout << "t = number of toy deliveries" << endl;
      return 1;
   }

   int e,r,t;
   e = atoi(argv[1]);
   r = atoi(argv[2]);
   t = atoi(argv[3]);

   if ( e <= 0 ) {
      e = 7;
   }
   if ( r <= 0 ) {
      r = 9;
   }
   if ( t <= 0 ) {
      t = 5;
   }

   bool retire = false;
   Santa* fatman = new Santa(t, r, &retire);
   Reindeer* deer[r];
   Elves* buddies[e];

   fatman->Begin();
   //fatman->Begin();
   for ( int i = 0; i < r; i++ ) {
      deer[i+1] = new Reindeer(i+1, r, &retire);
      deer[i+1]->Begin();
   }

   for ( int i = 0; i < e; i++ ) {
      buddies[i+1] = new Elves(i+1, &retire);
      buddies[i+1]->Begin();
   }

   fatman->Join();
   
   char buf[100];
   sprintf(buf, 
         "After (%d) deliveries, Santa retires and is on vacation!\n",t);
   write(1,buf,strlen(buf));

   return 0;
}
