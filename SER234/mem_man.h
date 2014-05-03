#ifndef	_MEM_MAN__H_
#define	_MEM_MAN__H_


#include "types.h"
#include "proc.h"


typedef
struct 
{
	u64		_used	:  1; 	//Read-only - Is being used by a process if true, 
							//is not being used if false.
							
	u64		_dirty	:  1; 	//Present - If not present, a swap needs to occur, transferring 
							//the data from a data store to the physical memory.
							
	u64		_pinned	:  1; 	//Valid - Page numbers are allocated to current executing process
							//If not valid, page is either not in the page table, or are in 
							//the page table but belong to another process. 
							
	u64				: 13;
	u64		_l1i	: 10;	
	u64				:  6;
	u64		_l2i	: 10;	
	u64				:  6;
	u64		_proc	: 16;
}
mem_manage;



void	read_page		(page*		x, 		u16		y);
void	write_page		(page*		x, 		u16		y);
void	build_mem_avail ();
u16		page_alloc		();
void	page_free		(u16		x);

//Allocates the references to physical memory from the page table into a contiguous 
//virtual address space that allows the process to execute. 
int		vas_alloc		(u16		v[],	u32		size); 	
void	vas_free		(u16		v[],	u32		size);

u32 virt_to_phys_read(u32 code_address);
u32 virt_to_phys_write(u32 code_address);
u32 virt_to_phys(u32 code_or_data_address);

#endif
