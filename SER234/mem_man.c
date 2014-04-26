#include	<stdio.h>
#include	"types.h"
#include	"proc.h"
#include	"mem_man.h"
#include	"util.h"



#define	PAGE_COUNT			65536
#define	PAGE_MASK			(PAGE_COUNT - 1)
#define	VAS_VEC_SIZE		(1 << 6)
#define	VAS_VEC_SIZE_MASK	(VAS_VEC_SIZE - 1)



static	page		mem[PAGE_COUNT];
static	u16			page_avail				= 0;
static	mem_manage	mem_man[PAGE_COUNT]		= {0};
static	u16			mem_offset				= 1;
static	u64			vas_vec[VAS_VEC_SIZE]	= {0};
static	u32			vas_offset				= 0;
static	u32			vas_count				= VAS_VEC_SIZE;




void
read_page (
	page*	x,
	u16		y
)
{
	u32	i;
	for (i = 0; i < 512; ++i) x->_u64[i]		= mem[y]._u64[i];
}




void
write_page (
	page*	x,
	u16		y
)
{
	u32	i;
	for (i = 0; i < 512; ++i) mem[y]._u64[i]	= x->_u64[i];
}




u16
page_alloc(
)
{
	u16	t	= page_avail;
	if (page_avail)	page_avail	= mem[page_avail]._u16[0];
	return t;
}




void
page_free(
	u16	x
)
{
	mem[x]._u16[0]	= page_avail;
	page_avail		= x;
}




int vas_alloc (u16 v[], u32 size) 
{
	u32	i;
	u32	t;

	if (vas_count < size)
	{
		return 0;
	}

	for (i = 0; i < size; ++i) 
	{
		if (~(vas_vec[vas_offset])) 
		{
			t = lsb64(vas_vec[vas_offset]);
			vas_vec[vas_offset] |= 1ul << t;
			v[i] = (vas_offset << 6) | t;
		}
		else	vas_offset = (vas_offset + 1) & VAS_VEC_SIZE_MASK;
	}
	vas_count -= size;
	return 1;
}




void vas_free (u16 v[], u32 size) 
{
	u32	i;

	for (i = 0; i < size; ++i)
	{
		vas_vec[v[i] >> 6] &= ~(1ul << (v[i] & 63));
	}
	vas_count += size;
}




u16 walk_page_ring ()
{
	u16 i; //Index for walking through mem_man[PAGE_COUNT].

	for (i = 0; i < PAGE_COUNT; i++)
	{//For all indexs of mem_man:
		if (mem_man[i]._used == 1)
		{//If the the page is used:
			mem_man[i]._used = 0; //Set page to unused.
		}
		else if (mem_man[i]._used == 0)
		{//If the page is unused:
			if (mem_man[i]._pinned == 0)
			{//And the page is not pinned:
				if (mem_man[i]._dirty == 0)
				{//And the page is not dirty:
					return i; //Return the address of the page.
				}
				else
				{//Or write that page to disk.
					disk_write(mem[i], i);
				}
			}
		}
	}
	return 0;
}





