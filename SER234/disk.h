#ifndef	_DISK_H_
#define	_DISK_H_



#include	"types.h"



u64		disk_read	(u32	block,	u16		addr);
u64		disk_write	(u32	block,	u16		addr);
int		in_alloc	(u16	v[],	u32		size);
void	in_free		(u16	v[],	u32		size);
int		lb_alloc	(u16	v[],	u32		size);
void	lb_free		(u16	v[],	u32		size);
int		sb_alloc	(u16	v[],	u32		size);
void	sb_free		(u16	v[],	u32		size);



#endif

