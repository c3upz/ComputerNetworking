/*
 * Link State Routing
 * Lab 7
 * Chris Gerbino
 */

        // Import the required libraries
        //******STUDENT WORK******
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>


//defines
#define	N			4
#define	INFINITE	1000



// types
typedef struct machines
{
	char name[0];
	char ip [50];
	int port;	
} MACHINES;


// global variables
MACHINES	machines[N];
int			costs[N][N];
int			distances[N];
int			myid, nodes;
int			sock;
struct sockaddr_in addr;
struct sockaddr_in otheraddr;
socklen_t addr_size;
pthread_mutex_t	lock;



// functions
void *run_link_state (void *);
void *receive_info (void *);
void print_costs (void);


/**********************
 * main
 **********************/
int
main (int argc, char *argv[])
{
	FILE	*fpA;
	FILE	*fpB;
	int		i, j;
	pthread_t	thr1, thr2;
	int		id, cost;
	int		packet[3];

// 3 = machine, 4 = cost

	if (argc < 5)
	{
		printf("missing args");
		return 1;
	}

	myid = atio(argv[1]);//******STUDENT WORK******
	nodes = N;//******STUDENT WORK******

	if (id >= N)
	{
		printf ("wrong id\n");
		return 1;
	}	

	if (nodes != N)
	{
		printf ("wrong number of nodes\n");
		return 1;
	}	
	
	//get info on machines from the file using fopen and fscanf and populate the array of machines-struct
	//******STUDENT WORK******
	if ((fpA = fopen (argv[3], "r")) == NULL){
		printf("error opening machine");
		return 0;
	{
	for( i = 0; i < 4; i++){
		fscanf(fpA, "%s %s %d", machines[i].name, machines[i].ip, &machines[i].port );
	}

	// get costs from the costs file and populate the matrix
	if ((fpB = fopen (argv[4], "r")) == NULL){
		printf("error opening costs");
		return 0;	
	}
	i = 0;
	for( i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			fscanf(fpB, "%d", &cost[i][j]);
		}
	}

	addr.sin_family = AF_INET;
	addr.sin_port = machines[id].port;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset((char*)addr.sin_zero, '\0', sizeof(addr.sin_zero));
	addr_size = sizeof(addr);

	//******STUDENT WORK******
		
	
    // init address
    //******STUDENT WORK******
	struct sockaddr_in dest_address;
	dest_address.sin_family  = AF_INET;
    
		// create socket
    //******STUDENT WORK******
	sock = socket (AF_INET, SOCK_DGRAM, 0);
	if (sock < 0){
		printf ("socket error\n");
		return 1;
	}
	
    // bind
    //******STUDENT WORK******
	if (bind (sock, (struct sockaddr *)&addr, sizeof (addr)) != 0)
	{
		printf ("bind error\n");
		return 1;
	}

//here
	// create threads and initiate mutex
	pthread_create (&thr1, NULL, receive_info, NULL);
	pthread_create (&thr2, NULL, run_link_state, NULL);
	pthread_mutex_init (&lock, NULL);

	// read changes from the keyboard, update the costs matrix inform other nodes about the changes
	{ // This section can be inside the main or can be a thread by itself
		for (i = 0; i < 3; i++)
		{


			//******STUDENT WORK******
			printf ("sent\n");
		}
	}	

	sleep (5);

}


/**********************
 * receive info
 **********************/
//If there is a cost update on other machine, they will let us know through a UDP packet. We need to receive that packet and update our costs table
void *receive_info (void *arg)
{
	int	new[3];
	int	nbytes;
	int	n0, n1, n2;

	
}


/**********************
 * run_link_state
 **********************/
// This thread runs the Dijkastra's algorithm on the cost table and calculates thte shortest path
void *
run_link_state (void *arg)
{
	int	taken[N];
	int	min, spot;
	int	i, j;
	int	r;

	//******STUDENT WORK******
	


		printf ("new-shortest distances:\n");
		for (i = 0; i < N; i++)
			printf ("%d ", distances[i]);
		printf ("\n");
}


/**********************
 * print costs
 **********************/
void
print_costs (void)
{
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			pthread_mutex_lock (&lock);
			printf ("%d ", costs[i][j]);
			pthread_mutex_unlock (&lock);
		}
		printf ("\n");
	}

}
