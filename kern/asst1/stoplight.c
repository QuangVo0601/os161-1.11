/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>

/*
 *
 * Constants
 *
 */

/*
 * Number of cars created.
 */

#define NCARS 20
// Four resources: 0 is SE, 1 is NE, 2 is NW, 3 is SW
struct lock *myResource[4];
// Counter used when the parent thread of cars are terminated
int counter = 0;

/*
 *
 * Function Definitions
 *
 */
void
message(char possibleDirection, int numberOfCars, unsigned long cardirection, unsigned long carnumber);

/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
	//use for sorting and swapping
        unsigned long temp0, temp1, temp2;
	temp1 = cardirection;
	temp2 = ((cardirection + 1) % 4);

	if(temp2 < temp1){
		temp0 = temp1;
		temp1 = temp2;
		temp2 = temp0;
	}
	//first approach the intersection	
	message('S', 0, cardirection, carnumber);
	lock_acquire(myResource[temp1]);
	lock_acquire(myResource[temp2]);
	//inside the intersection
	message('S', 1, cardirection, carnumber); 
	lock_release(myResource[temp1]);
        lock_release(myResource[temp2]);
	//leave the intersection
	message('S', 2, cardirection, carnumber);
	counter++;

        //(void) cardirection;
        //(void) carnumber;
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
	//use for sorting and swapping
       unsigned long temp0, temp1, temp2, temp3;
       temp1 = cardirection;
       temp2 = (cardirection + 1) % 4;
       temp3 = (cardirection + 2) % 4;
       if (temp1 > temp2) {
               temp0 = temp1;
                temp1 = temp2;
                temp2 = temp0;
       }

       if (temp2 > temp3) {
               temp0 = temp2;
               temp2 = temp3;
               temp3 = temp0;
       }

       if (temp1 > temp2) {
               temp0 = temp1;
               temp1 = temp2;
               temp2 = temp0;
       }	
	//approach the intersection 
       message('L', 0, cardirection, carnumber);
       lock_acquire(myResource[temp1]);
       lock_acquire(myResource[temp2]);
       lock_acquire(myResource[temp3]);
	//inside the intersection
       message('L', 1, cardirection, carnumber);
       lock_release(myResource[temp1]);       
       lock_release(myResource[temp2]);
       lock_release(myResource[temp3]);
	//leave the intersection
       message('L', 2, cardirection, carnumber);
       counter++;


        //(void) cardirection;
        //(void) carnumber;
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
	//approach the intersection
       message('R', 0, cardirection, carnumber);
       lock_acquire(myResource[cardirection]);
	//inside the intersection
       message('R', 1, cardirection, carnumber);
       lock_release(myResource[cardirection]);
	//leave the intersection
       message('R', 2, cardirection, carnumber);
       counter++;


        //(void) cardirection;
        //(void) carnumber;
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */
 
static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
        int cardirection;
	int possibleDirection;//the car can go straight, right, or left, which is 0, 1, 2 respectively
        /*
         * Avoid unused variable and function warnings.
         */

        //(void) unusedpointer;
        //(void) carnumber;
	//(void) gostraight;
	//(void) turnleft;
	//(void) turnright;

        /*
         * cardirection is set randomly.
         */

        cardirection = random() % 4;
	possibleDirection = random() % 3;
	
	switch(possibleDirection){
	case 0: 
		gostraight(cardirection, carnumber);
		break;
	case 1: 
                turnright(cardirection, carnumber);
                break;
	case 2: 
                turnleft(cardirection, carnumber);
                break;
	}
}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;
	//Locks creating for the four resources
	myResource[0] = lock_create("myResource 0");
	myResource[1] = lock_create("myResource 1");
	myResource[2] = lock_create("myResource 2");
	myResource[3] = lock_create("myResource 3");
	
        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;

        /*
         * Start NCARS approachintersection() threads.
         */
	if( myResource[0] == NULL || myResource[1] == NULL || myResource[2] == NULL || myResource[3]==NULL){
               panic("Lock creation failed");
	}

	kprintf("\nDirections: S is Straight, R is Right, L is Left\n");
	kprintf("\nResources: E is East, W is West, S is South, N is North\n\n");
  
        for (index = 0; index < NCARS; index++) {

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }
	//need to wait for the child thread termination
	while(counter < NCARS); 
	
	kprintf("\nGreat job!!! There are a total of %d cars that successfully passed the intersection.\n\n", counter);	
	
	counter = 0;

	//Kill all the created locks
	lock_destroy(myResource[0]);
	lock_destroy(myResource[1]);
	lock_destroy(myResource[2]);
	lock_destroy(myResource[3]);
 
	return 0;
}

/*
 *  message()
 * 
 *  Arguments:
 *  char possibleDirection: the direction that the car can go, can be Straight, Left, or Right.
 *  int action: 0 represents the car is approaching the intersection, 1 represents the car is inside the intersection, 2 represents the car is leaving the intersection 
 *  unsigned long cardirection: the original direction where the car comes from
 *  unsigned long carnumber: id number of the car
 *  
 *  Returns: nothing
 *              */
void
message(char possibleDirection, int action, unsigned long cardirection, unsigned long carnumber){

	switch(action){
	   //Cars arrive at the intersection
	   case 0:
		if(cardirection==0) {
			kprintf("Incoming at intersection: Car %ld\t from %c\t turn/go %c\n", carnumber, 'E', possibleDirection);
		}
		if(cardirection==1) {
			kprintf("Incoming at intersection: Car %ld\t from %c\t turn/go %c\n", carnumber, 'W', possibleDirection);
		}
		if(cardirection==2) {
			kprintf("Incoming at intersection: Car %ld\t from %c\t turn/go %c\n", carnumber, 'S', possibleDirection);
		}
		if(cardirection==3) {
			kprintf("Incoming at intersection: Car %ld\t from %c\t turn/go %c\n", carnumber, 'N', possibleDirection);
		}
		break;
	   //Cars at the intersection
	   case 1: 
		if(cardirection==0){
			kprintf("At the intersection:      Car %ld\t from %c\t turn/go %c\n", carnumber, 'E', possibleDirection);
		}
		if(cardirection==1){	
			kprintf("At the intersection:      Car %ld\t from %c\t turn/go %c\n", carnumber, 'W', possibleDirection);
		}
		if(cardirection==2){
			kprintf("At the intersection:      Car %ld\t from %c\t turn/go %c\n", carnumber, 'S', possibleDirection);
		}
		if(cardirection==3){
			kprintf("At the intersection:      Car %ld\t from %c\t turn/go %c\n", carnumber, 'N', possibleDirection);
		}
		break;
	   //Cars leave the intesection
	   case 2:
		if(cardirection==0){
			kprintf("Leaving the intersection: Car %ld\t from %c\t turn/go %c\n", carnumber, 'E', possibleDirection);
		}
		if(cardirection==1){
			kprintf("Leaving the intersection: Car %ld\t from %c\t turn/go %c\n", carnumber, 'W', possibleDirection);
		}
		if(cardirection==2){
			kprintf("Leaving the intersection: Car %ld\t from %c\t turn/go %c\n", carnumber, 'S', possibleDirection);
		}
		if(cardirection==3){
			kprintf("Leaving the intersection: Car %ld\t from %c\t turn/go %c\n", carnumber, 'N', possibleDirection);
		}
		break;
	}
}
