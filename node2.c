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
} dt2;


struct rtpkt packet;
/* students to write the following two routines, and maybe some others */

/*similar in function to rtinit0, and rtinit1.  
  Calculates shortest costs from node 1 to nodes 0,2,3 */
void rtinit2()
{
  int i, j;
  
  if( TRACE > 0 ) printf("RTinit2: Initializing... \n");

  // Init min cost
  for( i = 0; i < 4; i++ ){
    packet.mincost[i] = 999;
    
    // Also init distances to ininity 
    for( j = 0; j < 4; j++ ){
      dt2.costs[i][j]= 999;	  
    }
  }
  
  // Set the distances   
  dt2.costs[0][0]=3;
  dt2.costs[1][1]=1;
  dt2.costs[3][3]=2;

  // Find and save the minimum costs if they exist. 
  for( i = 0; i < 4; i++ ){
    for( j = 0; j < 4; j++ ){
      if(packet.mincost[i] > dt2.costs[i][j]){
	packet.mincost[i] = dt2.costs[i][j];
      }
    }
  }
  
  packet.sourceid = 2;
  packet.destid   = 0;
  tolayer2(packet);
  
  packet.sourceid = 2;
  packet.destid   = 1;
  tolayer2(packet);
  
  packet.sourceid = 2;
  packet.destid   = 3;
  tolayer2(packet);
  

  if( TRACE > 0 ) printf("RTinit2: Initializing complete \n");  
  if( TRACE > 1 ) printdt2(&dt2); 
}

/*updates data in distance table in case of changes*/
void rtupdate2(rcvdpkt)
     struct rtpkt *rcvdpkt;
{

  int needsUpdate = 0;
  int i, j;

  if( TRACE > 0 ) printf("RTupdate2: updating... \n");

  // Init min cost
  for( i = 0; i < 4; i++ ){    
    packet.mincost[i] = 999;
  }

  // loop through each connection to check for update
  for( i = 0; i < 4; ++i){
    if( rcvdpkt->mincost[i] + dt2.costs[rcvdpkt->sourceid][rcvdpkt->sourceid] < dt2.costs[i][rcvdpkt->sourceid]){
      
      // Update needed
      needsUpdate = 1;
      dt2.costs[i][rcvdpkt->sourceid] = rcvdpkt->mincost[i]  + dt2.costs[rcvdpkt->sourceid][rcvdpkt->sourceid]; 
    }
  }

  // Update table if needed
  if( needsUpdate ){
    for( i = 0; i < 4; i++ ){
      for( j = 0; j < 4; j++ ){
	if( packet.mincost[i] > dt2.costs[i][j] ) 
	  packet.mincost[i] = dt2.costs[i][j];
      }
    }
    
    packet.sourceid = 2;
    packet.destid   = 0;
    tolayer2(packet);
    
    packet.sourceid = 2;
    packet.destid   = 1;
    tolayer2(packet);
    
    packet.sourceid = 2;
    packet.destid   = 3;
    tolayer2(packet);
  }
  
  if( TRACE >  0 ) printf("RTupdate2: complete \n");  
  if( TRACE > -1 ) printdt2(&dt2);
}


printdt2(dtptr)
struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n", dtptr->costs[0][0], dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n", dtptr->costs[1][0], dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n", dtptr->costs[3][0], dtptr->costs[3][1],dtptr->costs[3][3]);
}


linkhandler2(linkid, newcost)
int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */

{
}
