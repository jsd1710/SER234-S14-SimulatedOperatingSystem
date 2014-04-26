#ifndef _PROC__H_
#define _PROC__H_



typedef 
struct process 
{
	u16				_pid;
	u32				_vas;									// Virtual Adddress Size
	dev_type		_wait_on;
	u32				_cs;									// Code Size
	u32				_ds;									// Data Size
	u32				_fs;									// File Size
	u32				_ca;									// Code Address
	u32				_da;									// Data Address
	u32				_fa;									// File Address
	u32				_ct;									// Code Time
	u32				_dt;									// Data Time
	u32				_ft;									// File Time
	u32				_nt;									// Net Time
	u32				_kt;									// kbd Time
	void			(*_cp)(u32 m, u32 t, u32* l, u32* d);	// Code Profile
	void			(*_dp)(u32 m, u32 t, u32* l, u32* d);	// Data Profile
	void			(*_fp)(u32 m, u32 t, u32* l, u32* d);	// File Profile
	void			(*_np)(u32 m, u32 t, u32* l, u32* d);	// Net Profile
	void			(*_kp)(u32 m, u32 t, u32* l, u32* d);	// kbd Profile
	u16				_fpi;									// File Pointer Index
	u64				_time;
	u32				_priority;
	struct process*	_next;
	u16				_pti;									// Page Table Index
	u16				_sbt		[1024];						// Swap Block Table
} 
process;





void		blocked_enq		(process*	p,		u64		time);
process*	blocked_deq		();
void		ready_enq		(process*	p,		s32		priority_delta);
process*	ready_deq		();
u64			time_get		();
void		time_adv		(u32		delta);




#endif
