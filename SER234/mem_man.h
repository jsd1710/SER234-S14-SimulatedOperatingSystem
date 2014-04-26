#ifndef	_MEM_MAN__H_
#define	_MEM_MAN__H_


#include "types.h"
#include "proc.h"


typedef
struct {
	u64		_used	:  1;
	u64		_dirty	:  1;
	u64		_pinned	:  1;
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
int		vas_alloc		(u16		v[],	u32		size);
void	vas_free		(u16		v[],	u32		size);


#endif
