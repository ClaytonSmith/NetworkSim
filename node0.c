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

struct distance_table
{
  int costs[4][4];
} dt0;


struct rtpkt packet;
/* students to write the following two routines, and maybe some others */

void rtinit0(){

  int i, j;
  
  if( TRACE > 0 ) printf("RTinit0: Initializing... \n");

  // Init min cost
  for( i = 0; i < 4; i++ ){
    packet.mincost[i] = 999;
    
    // Also init distances to ininity 
    for( j = 0; j < 4; j++ ){
      dt0.costs[i][j]= 999;	  
    }
  }
  
  // 1, 3, 7 are the distances   
  dt0.costs[1][1]=1;
  dt0.costs[2][2]=3;
  dt0.costs[3][3]=7;

  // Find and save the minimum costs if they exist. 
  for( i = 0; i < 4; i++ ){
    for( j = 0; j < 4; j++ ){
      if(packet.mincost[i] > dt0.costs[i][j]){
	packet.mincost[i] = dt0.costs[i][j];
      }
    }
  }
  

  packet.sourceid = 0;
  packet.destid   = 1;
  tolayer2(packet);
  
  packet.sourceid = 0;
  packet.destid   = 2;
  tolayer2(packet);
  
  packet.sourceid = 0;
  packet.destid   = 3;
  tolayer2(packet);
  

  if( TRACE > 0 ) printf("RTinit0: Initializing complete \n");  
  if( TRACE > 1 ) printdt0(&dt0); 
}


void rtupdate0(rcvdpkt)
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
    if( rcvdpkt->mincost[i] + dt0.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] < dt0.costs[i][rcvdpkt->sourceid]){
      // Update needed
      needsUpdate = 1;
      dt0.costs[i][rcvdpkt->sourceid] = rcvdpkt->mincost[i]  + dt0.costs[rcvdpkt->sourceid][rcvdpkt->sourceid]; 
    }
  }

  // Update table if needed
  if( needsUpdate ){
    for( i = 0; i < 4; i++ ){
      for( j = 0; j < 4; j++ ){
	if( packet.mincost[i] > dt0.costs[i][j] ) 
	  packet.mincost[i] = dt0.costs[i][j];
      }
    }
    
    packet.sourceid = 0;
    packet.destid   = 1;
    tolayer2(packet);
    
    packet.sourceid = 0;
    packet.destid   = 2;
    tolayer2(packet);
    
    packet.sourceid = 0;
    packet.destid   = 3;
    tolayer2(packet);
  }

  if( TRACE >  0 ) printf("RTupdate0: complete \n");  
  if( TRACE > -1 ) printdt0(&dt0);
}


printdt0(dtptr)
struct distance_table *dtptr;
{
  
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n", dtptr->costs[1][1], dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n", dtptr->costs[2][1], dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n", dtptr->costs[3][1], dtptr->costs[3][2],dtptr->costs[3][3]);
}

linkhandler0(linkid, newcost)
int linkid, newcost;
{
  /* called when cost from 0 to linkid changes from current value to newcost*/
  /* You can leave this routine empty if you're an undergrad. If you want */
  /* to use this routine, you'll need to change the value of the LINKCHANGE */
  /* constant definition in prog3.c from 0 to 1 */
  
  
}
