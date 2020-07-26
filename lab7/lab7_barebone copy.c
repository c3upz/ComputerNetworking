/*
 * Link State Routing
 * Lab 7
 */

#include <unistd.h>
        // Import the required libraries
        //******STUDENT WORK******


//defines
#define	N			4
#define	INFINITE	1000



// types
typedef struct machines
{
	//******STUDENT WORK******	

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
	FILE	*fp;
	int		i, j;
	pthread_t	thr1, thr2;
	int		id, cost;
	int		packet[3];


	if (argc < x)
	{
		// check the number of arguments
        //******STUDENT WORK******
	}

	myid = //******STUDENT WORK******
	nodes = //******STUDENT WORK******

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
	

	// get costs from the costs file and populate the matrix
	if ((fp = fopen (argv[4], "r")) == NULL)
	{
		//******STUDENT WORK******
	}

	//******STUDENT WORK******
		
	
    // init address
    //******STUDENT WORK******

    // create socket
    //******STUDENT WORK******

    // bind
    //******STUDENT WORK******


	// create threads and initiate mutex
	pthread_mutex_init (&lock, NULL);
	pthread_create (&thr1, NULL, receive_info, NULL);
	pthread_create (&thr2, NULL, run_link_state, NULL);

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
