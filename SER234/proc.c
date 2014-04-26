#include	<stdio.h>
#include	<math.h>
#include	"types.h"
#include	"proc.h"



static	u64			time			= 0;


typedef	struct	{
	u64		_t;			// time to which process is allowed to run
	u32*	_code_addr;
	u32*	_code_time;
	u32*	_data_addr;
	u32*	_data_time;
}
run_time;

struct	{
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


void blocked_enq(process* p, u64 time)
{

}

process* blocked_deq()
{

}

void ready_enq(process* p, s32 priority_delta)
{

}

process* ready_deq()
{

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
	u64		time	= get_time();
	u32		i;

	u32	code_trans	= virt_to_phys(code_addr);
	u32	data_trans	= virt_to_phys(data_addr);

	if (!code_trans) {
		//page_fault code
	}

	if (!data_trans) {
		//page_fault code
	}

	while (1) {
		u32	t_t_t	= t - get_time(); //time_till_timer
		if (code_time < data_time) {
			if (code_time > t_t_t) {
				code_time -= t_t_t;
				data_time -= t_t_t;
				return t;
			}
			set_time (get_time() + code_time);
			data_time  -= code_time;
			code_addr	= new_code_addr(code_addr, proc_code_limit);
			code_time	= new_code_time();
			code_trans	= virt_to_phys_read(code_addr);
			if (!code_trans) {
				//page_fault code
				return disk_time();
			}
		}
		else if (data_time > t_t_t) {
			code_time -= t_t_t;
			data_time -= t_t_t;
			return t;
		}
		else {
			set_time (get_time() + data_time);
			code_time  -= data_time;
			data_addr	= new_data_addr(data_addr, proc_code_limit, proc_data_limit);
			data_time	= new_data_time();
			data_trans	= virt_to_phys_write(data_addr);
			if (!data_trans) {
				//page_fault code
				return disk_time();
			}
		}
	}
}




