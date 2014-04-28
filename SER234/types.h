#ifndef _TYPES__H_
#define _TYPES__H_



#define	CASE	break; case




typedef				char	s8;
typedef	unsigned	char	u8;
typedef				short	s16;
typedef	unsigned	short	u16;
typedef				int		s32;
typedef	unsigned	int		u32;
typedef				long	s64;
typedef	unsigned	long	u64;



typedef 
union {
	u8		_u8		[4096];
	s8		_s8		[4096];
	u16		_u16	[2048];
	s16		_s16	[2048];
	u32		_u32	[1024];
	s32		_s32	[1024];
	u64		_u64	[ 512];
	s64		_s64	[ 512];
} 
page; //Page Table with different units of division.




typedef	
enum {
	Code, Data, File, Network, Keyboard, Timer
}	
dev_type;



#endif
