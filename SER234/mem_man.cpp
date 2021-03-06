#include	<stdio.h>
#include	"types.h"
#include	"proc.h"
#include	"mem_man.h"
#include	"util.h"
#include	"disk.h"

//------------Constants--------------------------
//Page
#define	PAGE_COUNT			65536
#define	PAGE_MASK			(PAGE_COUNT - 1)
//VAS
#define	VAS_VEC_SIZE		(1 << 6)
#define	VAS_VEC_SIZE_MASK	(VAS_VEC_SIZE - 1)

//------------Member variables of mem_man--------
static	page		mem[PAGE_COUNT];						//Pages in memory.
static	u16			page_avail				= 0;			//The currently available page index in mem[].
static	mem_manage	mem_man[PAGE_COUNT]		= {0};			//The page table in memory.
static	u16			mem_offset				= 1;
static	u64			vas_vec[VAS_VEC_SIZE]	= {0};			//The 64 Virtual Address Spaces in memory.
static	u32			vas_offset				= 0;
static	u32			vas_count				= VAS_VEC_SIZE;

//------------Methods----------------------------
//Reads page from data-store to memory.
void read_page(
		page* 	x, 	//Page table 's address on data-store.
		u16 	y	//Page's index in the page table mem[] to write to.
		)
{
	u32	i;
	for (i = 0; i < 512; ++i) 
	{//For the entire page of x.
		//Copy it's contents to the page of mem[]'s page table at index y.
		x->_u64[i] = mem[y]._u64[i];
	}
}

//Writes page from memory to data-store.
void write_page (
		page*	x,	//Page table's address on data-store.
		u16 	y	//Page's index in the page table mem[] to read from.
		)
{
	u32	i;
	for (i = 0; i < 512; ++i) 
	{//For the entire page of the mem[] page table at index y.
		//Copy it's contents to the page x.
		mem[y]._u64[i]	= x->_u64[i];
	}
}


u16 page_alloc()
{
	u16	t	= page_avail;
	if (page_avail)	
	{//If page_avail is not 0.
		//Assign the page_avail to the page to the current available page's
		//u16 page table at the beginning of the page table.
		page_avail	= mem[page_avail]._u16[0]; 
	} 

	return t; //Returns the current available page's address?
}


void page_free(u16 x) //Takes in the page index you want to free from the mem[] page table.
{
	mem[x]._u16[0]	= page_avail; //Commits the last page to this index of mem[] and changes the page_avail to the input x.
	page_avail		= x;
}


int vas_alloc (u16 v[], u32 size) 
{
	u32	i;
	u32	t;

	if (vas_count < size) 
	{	//If the virtual address space count(64 - loaded processes) is less 
		//than the size being allocated.
		return 0; //Returns false and does nothing.
	}

	for (i = 0; i < size; ++i) 
	{//For each virtual address space that needs to be added to vas_vec.
		if (~(vas_vec[vas_offset])) 
		{ //If the complement of the vas_vec's offset is all true (e.g. 111111)
			t = lsb64(vas_vec[vas_offset]); //Find the least significant bit of vas_vec's offset.
			vas_vec[vas_offset] |= 1ul << t; //vas_vec at index offset is OR'd with (1 unsigned long shifted by t).
			v[i] = (vas_offset << 6) | t; //Assigns the index of the input virtual address space that is input as the offset shifts by 6 bits and ors it with the size mask.
		}
		else	vas_offset = (vas_offset + 1) & VAS_VEC_SIZE_MASK; //Increment the offset by 1 and AND it with the size mask(111111).
	}
	vas_count -= size; //Removes the proper amount of virtual address spaces.
	return 1; //Returns true.
}


void vas_free (u16 v[], u32 size) 
{
	u32	i;

	for (i = 0; i < size; ++i)
	{
		vas_vec[v[i] >> 6] &= ~(1ul << (v[i] & 63));
	}
	vas_count += size; //Frees the proper amount of virtual address spaces.
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
			{//If the page is not pinned:
				if (mem_man[i]._dirty == 0)
				{//And the page is not dirty:
					return i; //Return the address of the page.
				}
				else
				{//Or write that page to disk.
					//disk_write(mem[i]._u32, i); //QUESTION: Should this be  write_page()?
				}
			}
		}
	}
	return 0;
}

u32 virt_to_phys_read(u32 code_address)
{
	return 1;
}

u32 virt_to_phys_write(u32 code_address)
{
	return 1;
}

u32 virt_to_phys(u32 code_or_data_address)
{
	return 1;
}