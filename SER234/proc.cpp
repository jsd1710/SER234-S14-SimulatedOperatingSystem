#include	<stdio.h>
#include	<stdlib.h> //For rand().

#include	<math.h>
#include	"types.h"
#include	"proc.h"
#include	"mem_man.h"
#include	"disk.h"


#define MAX_PRIORITY 10
#define MAX_BLOCKED 50

static	u64			_time			= 0;

static	process*	priorityLowQueue[MAX_PRIORITY];
static	int			priorityLowHead = 0;
static	int			priorityLowTail = 0;

static	process*	priorityMediumQueue[MAX_PRIORITY];
static	int			priorityMediumHead = 0;
static	int			priorityMediumTail = 0;

static	process*	priorityHighQueue[MAX_PRIORITY];
static	int			priorityHighHead = 0;
static	int			priorityHighTail = 0;

static	process*	blockedQueue[MAX_BLOCKED];
static	int			blockedHead = 0;
static	int			blockedTail = 0;

typedef	struct	
{
	u64		_t;			// time to which process is allowed to run
	u32*	_code_addr;
	u32*	_code_time;
	u32*	_data_addr;
	u32*	_data_time;
}
run_time;

struct	
{
	u64			_start_time;
	u32			_run_time;
	u32			_code_size;
	u32			_data_size;
	u32			_priority;
} proc_init[20]	= {
					{	10ul,	500000,		20000000,		30000000,		1	},
					{	11ul,	600000,		40000000,		20000000,		2	},
					{	12ul,	700000,		60000000,		10000000,		3	},
					{	13ul,	800000,		80000000,		90000000,		1	},
					{	14ul,	900000,		10000000,		80000000,		2	},
					{	15ul,	500000,		20000000,		70000000,		3	},
					{	16ul,	600000,		30000000,		60000000,		1	},
					{	17ul,	700000,		40000000,		50000000,		2	},
					{	18ul,	800000,		50000000,		40000000,		3	},
					{	19ul,	900000,		60000000,		30000000,		1	},
					{	20ul,	500000,		70000000,		20000000,		2	},
					{	21ul,	600000,		80000000,		10000000,		3	},
					{	22ul,	700000,		90000000,		50000000,		1	},
					{	23ul,	800000,		10000000,		40000000,		2	},
					{	24ul,	900000,		20000000,		30000000,		3	},
					{	25ul,	500000,		30000000,		20000000,		1	},
					{	26ul,	600000,		40000000,		10000000,		2	},
					{	27ul,	700000,		50000000,		60000000,		3	},
					{	28ul,	800000,		60000000,		30000000,		1	},
					{	29ul,	900000,		70000000,		10000000,		2	}
				};

void set_time(u64 new_time)
{
	_time = new_time;
}
u64 get_time()
{
	return _time;
}

void blocked_enq(process* p)
{
	if (blockedTail >= blockedHead)
	{//If the tail is greater or equal to the head (it has not wrapped around the end of the array yet).
		if (blockedTail == MAX_BLOCKED - 1)
		{//If the tail is located at the end of the queue.
			if (blockedHead != 0)
			{//If the head is not zero, then wrap tail to 0.
				blockedTail = 0;
				blockedQueue[blockedTail] = p;
			}
		}
		else
		{//Standard procedure:
			blockedTail++; //Increment the tail location and add it to the blocked queue.
			blockedQueue[blockedTail] = p;
		}
	}
	else
	{//If the tail is less than the head (it wrapped around the end of the array).
		if ((blockedTail + 1) != blockedHead)
		{//If the head is not next in line:
			blockedTail++; //Increment the tail location and add it to the blocked queue.
			blockedQueue[blockedTail] = p;
		}

	}
}

process* blocked_deq()
{
	if (blockedHead < blockedTail)
	{//If the head is less than the tail (implying head != tail).
		process* temp = blockedQueue[blockedHead];
		blockedQueue[blockedHead] = NULL;
		blockedHead++;
		return temp;
	}
	else if (blockedHead > blockedTail)
	{
		if (blockedHead == (MAX_BLOCKED - 1))
		{
			process* temp = blockedQueue[blockedHead];
			blockedQueue[blockedHead] = NULL;
			blockedHead = 0;
			return temp;
		}
		else if (blockedHead != (MAX_BLOCKED - 1))
		{
			process* temp = blockedQueue[blockedHead];
			blockedQueue[blockedHead] = NULL;
			blockedHead++;
			return temp;
		}
	}
	else// if (blockedHead == blockedTail)
	{
		process* temp = blockedQueue[blockedHead];
		blockedQueue[blockedHead] = NULL;
		return temp;
	}
}

void ready_enq(process* p, s32 priority_delta)
{
	int* priority_head;
	int* priority_tail;
	process** priority_queue;

	switch (priority_delta)
	{
	case 3:
		priority_head = &priorityLowHead;
		priority_tail = &priorityLowTail;
		priority_queue = priorityLowQueue;
		break;
	case 2:
		priority_head = &priorityMediumHead;
		priority_tail = &priorityMediumTail;
		priority_queue = priorityMediumQueue;
		break;
	case 1:
		priority_head = &priorityHighHead;
		priority_tail = &priorityHighTail;
		priority_queue = priorityHighQueue;
		break;
	}

	if (*priority_tail >= *priority_head)
	{//If the tail is greater or equal to the head (it has not wrapped around the end of the array yet).
		if (*priority_tail == MAX_BLOCKED - 1)
		{//If the tail is located at the end of the queue.
			if (*priority_head != 0)
			{//If the head is not zero, then wrap tail to 0.
				*priority_tail = 0;
				priority_queue[*priority_tail] = p;
			}
		}
		else
		{//Standard procedure:
			*priority_tail++; //Increment the tail location and add it to the blocked queue.
			priority_queue[*priority_tail] = p;
		}
	}
	else
	{//If the tail is less than the head (it wrapped around the end of the array).
		if ((*priority_tail + 1) != *priority_head)
		{//If the head is not next in line:
			*priority_tail++; //Increment the tail location and add it to the blocked queue.
			priority_queue[*priority_tail] = p;
		}
	}
}

process* ready_deq(s32 priority_delta)
{
	int* priority_head;
	int* priority_tail;
	process** priority_queue;

	switch (priority_delta)
	{
	case 3:
		priority_head = &priorityLowHead;
		priority_tail = &priorityLowTail;
		priority_queue = priorityLowQueue;
		break;
	case 2:
		priority_head = &priorityMediumHead;
		priority_tail = &priorityMediumTail;
		priority_queue = priorityMediumQueue;
		break;
	case 1:
		priority_head = &priorityHighHead;
		priority_tail = &priorityHighTail;
		priority_queue = priorityHighQueue;
		break;
	}

	if (*priority_head < *priority_tail)
	{//If the head is less than the tail (implying head != tail).
		process* temp = priority_queue[*priority_head];
		priority_queue[*priority_head] = NULL;
		*priority_head++;
		return temp;
	}
	else if (*priority_head > *priority_tail)
	{
		if (*priority_head == (MAX_BLOCKED - 1))
		{
			process* temp = priority_queue[*priority_head];
			priority_queue[*priority_head] = NULL;
			*priority_head = 0;
			return temp;
		}
		else if (*priority_head != (MAX_BLOCKED - 1))
		{
			process* temp = blockedQueue[blockedHead];
			priority_queue[*priority_head] = NULL;
			*priority_head++;
			return temp;
		}
	}
	else// if (blockedHead == blockedTail)
	{
		process* temp = priority_queue[*priority_head];
		priority_queue[*priority_head] = NULL;
		return temp;
	}
}


u32 new_code_addr (u32 addr, u32 limit)
{
	static u32	x[32]	= {	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1, 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1, 
							 2,	 2,	 2,	 2,	 2,	 2,	 2,	 2, 3,	 3,	 3,	 3,	 4,	 4,	 8,	16};

	u32	r = rand();
	addr	=  (r & 64) ? addr + x[r & 31] : addr - x[r & 31];
	return (addr > limit) ? addr = r % limit : addr;
}




u64 new_code_time ()
{
	return	50 + (rand() & 0xfff);
}




u32 new_data_addr (u32 addr, u32 base, u32 limit)
{
	static u32	x[32]	= {	 1,	 1,	 1,	 1,	 2,	 2,	 2,	 2,	 3,	 3,	 3,	 3,	 4,	 4,	 4,	 4, 
							 5,	 5,	 6,	 6,	 7,	 7,	 8,	 8,	 9,	10,	11,	12,	16,	20,	28,	40};

	u32	r = rand();
	addr	=  (r & 64) ? addr + x[r & 31] : addr - x[r & 31];
	return ((base < addr) || (addr > limit)) ? addr = base + (r % (limit - base)) : addr;
}




u64 new_data_time ()
{
	return	100 + (rand() & 0x1fff);
}




u64 process_exec (
	u64		t,	// time to which process is allowed to run
	u32		code_addr,
	u32		code_time,
	u32		code_limit,
	u32		data_addr,
	u32		data_time,
	u32		data_limit
)
{
	u32	code_trans;// = virt_to_phys(code_addr); //Should this be walk_page_ring? //AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
	u32	data_trans;// = virt_to_phys(data_addr); //Should this be vas_alloc? //AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH

	while (1) 
	{
		u32	t_t_t	= t - get_time(); //time_till_timer
		if (code_time < data_time) 
		{
			if (code_time > t_t_t) 
			{
				code_time -= t_t_t;
				data_time -= t_t_t;
				return t;
			}
			set_time (get_time() + code_time);
			data_time  -= code_time;
			code_addr	= new_code_addr(code_addr, code_limit);
			code_time	= new_code_time();
			code_trans	= virt_to_phys_read(code_addr);
			if (!code_trans) 
			{
				//page_fault code
				return get_disk_time();
			}
		}
		else if (data_time > t_t_t) 
		{
			code_time -= t_t_t;
			data_time -= t_t_t;
			return t;
		}
		else 
		{
			set_time (get_time() + data_time);
			code_time  -= data_time;
			data_addr	= new_data_addr(data_addr, code_limit, data_limit);
			data_time	= new_data_time();
			data_trans = virt_to_phys_write(data_addr); //AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
			if (!data_trans) 
			{
				//page_fault code
				return get_disk_time();
			}
		}
	}
}

void scheduler()
{
	//ready_deq(3);
	//ready_deq(3);
	//ready_deq(3);
	//ready_deq(3);

	//ready_deq(2);
	//ready_deq(2);

	//ready_deq(1);

	//blocked_deq();
}

int main(void)
{
	printf("Hello World!");
	getchar();
	return 0;
}



