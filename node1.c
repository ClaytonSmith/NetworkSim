#include <stdio.h>


extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
};

extern int TRACE;
extern int YES;
extern int NO;

int connectcosts[4] = {1,0,1,999};

struct distance_table
{
  int costs[4][4];
} dt1;

struct rtpkt packet;
/* students to write the following two routines, and maybe some others */

void rtinit1(){

  int i, j;
  
  if( TRACE > 0 ) printf("RTinit1: Initializing... \n");

  // Init min cost
  for( i = 0; i < 4; i++ ){
    packet.mincost[i] = 999;
    
    // Also init distances to ininity 
    for( j = 0; j < 4; j++ ){
      dt1.costs[i][j]= 999;	  
    }
  }
  
  // Set the distances   
  dt1.costs[0][0]=1;
  dt1.costs[2][2]=1;
  
  // Find and save the minimum costs if they exist. 
  for( i = 0; i < 4; i++ ){
    for( j = 0; j < 4; j++ ){
      if(packet.mincost[i] > dt1.costs[i][j]){
	packet.mincost[i] = dt1.costs[i][j];
      }
    }
  }

  packet.sourceid = 1;
  packet.destid   = 0;
  tolayer2(packet);

  packet.sourceid = 1;
  packet.destid   = 2;
  tolayer2(packet);

  if( TRACE > 0 ) printf("RTinit1: Initializing complete \n");  
  if( TRACE > 1 ) printdt1(&dt1); 
}


void rtupdate1(rcvdpkt)
     struct rtpkt *rcvdpkt;
{

  int needsUpdate = 0;
  int i, j;

  if( TRACE > 0 ) printf("RTupdate0: updating... \n");

  // Init min cost
  for( i = 0; i < 4; i++ ){    
    packet.mincost[i] = 999;
  }

  // loop through each connection to check for update
  for( i = 0; i < 4; ++i){
    if( rcvdpkt->mincost[i] + dt1.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] < dt1.costs[i][rcvdpkt->sourceid]){
      // Update needed

      needsUpdate = 1;
      dt1.costs[i][rcvdpkt->sourceid] = rcvdpkt->mincost[i]  + dt1.costs[rcvdpkt->sourceid][rcvdpkt->sourceid]; 
    }
  }

  // Update table if needed
  if( needsUpdate ){
    for( i = 0; i < 4; i++ ){
      for( j = 0; j < 4; j++ ){
	if( packet.mincost[i] > dt1.costs[i][j] ) 
	  packet.mincost[i] = dt1.costs[i][j];
      }
    }
    
    packet.sourceid = 1;
    packet.destid   = 0;
    tolayer2(packet);
    
    packet.sourceid = 1;
    packet.destid   = 2;
    tolayer2(packet);
  }

  if( TRACE >  0 ) printf("RTupdate1: complete \n");  
  if( TRACE > -1 ) printdt1(&dt1);
}


printdt1(dtptr)
struct distance_table *dtptr;
  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|----------- \n");
  printf("     0|  %3d   %3d \n", dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d \n", dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d \n", dtptr->costs[3][0], dtptr->costs[3][2]);

}

linkhandler1(linkid, newcost)
int linkid, newcost;
{
  /* called when cost from 0 to linkid changes from current value to newcost*/
  /* You can leave this routine empty if you're an undergrad. If you want */
  /* to use this routine, you'll need to change the value of the LINKCHANGE */
  /* constant definition in prog3.c from 0 to 1 */


}
