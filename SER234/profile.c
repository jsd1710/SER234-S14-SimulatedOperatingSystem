#include	<math.h>
#include	"types.h"





u32 profile_a(u32 max, u32* location) 
{
	static u32	a[32]	= {	 1,		 1,		 1,		 1,		 1,		 1,		 1,		 1, 
							 1,		 1,		 1,		 1,		 1,		 1,		 1,		 1, 
							 2,		 2,		 2,		 2,		 2,		 2,		 2,		 2,
							 3,		 3,		 3,		 3,		 4,		 4,		 8,		16};
	static u32	b[32]	= {	 1,		 1,		 1,		 1,		 1,		 1,		 1,		 1, 
							 1,		 1,		 1,		 1,		 2,		 2,		 2,		 2, 
							 2,		 2,		 3,		 3,		 3,		 3,		 4,		 4,
							 4,		 5,		 5,		 6,		 7,		 8,		16,		32};
	u32	r = rand();
	*location	=  *location + (a[(r >> 5) & 31] * ((r & 64) ? 1 : -1));
	if (*location > max) *location	= (r >> 10) % max;
	return b[r & 31];
}




u32 profile_b(u32 max, u32* location) 
{
	static u32	a[32]	= {	 1,		 1,		 1,		 1,		  1,	  1,	  1,	  1, 
							 1,		 1,		 1,		 1,		  2,	  2,	  2,	  2, 
							 2,		 2,		 3,		 3,		  3,	  3,	  4,	  4,
							 4,		 5,		 5,		 6,		  7,	  8,	 16,	 32};
	static u32	b[32]	= {	 1,		 1,		 1,		 1,		  2,	  2,	  2,	  2, 
							 3,		 3,		 3,		 4,		  4,	  4,	  5,	  5, 
							 6,		 7,		 8,		 9,		 10,	 12,	 15,	 19,
							25,		35,		55,		89,		123,	193,	331,	513};
	u32	r = rand();
	*location	=  *location + (a[(r >> 5) & 31] * ((r & 64) ? 1 : -1));
	if (*location > max) *location	= (r >> 10) % max;
	return b[r & 31];
}

u32 profile_c(u32 max, u32* location) 
{
	static u32	a[32]	= {	 1,		 1,		 1,		 1,		  2,	  2,	   2,	   2, 
							 3,		 3,		 3,		 4,		  4,	  4,	   5,	   5, 
							 6,		 7,		 8,		 9,		 10,	 12,	  15,	  19,
							25,		35,		55,		89,		123,	193,	 331,	 513};
	static u32	b[32]	= {	 1,		  1,	  2,	  2,	  3,	  3,	   4,	   4, 
							 5,		  5,	  6,	  6,	  7,	  7,	   8,	   8, 
							 9,		 10,	 12,	 15,	 18,	 23,	  39,	  51,
							79,		112,	191,	311,	517,	811,	1025,   1492};
	u32	r = rand();
	*location	=  *location + (a[(r >> 5) & 31] * ((r & 64) ? 1 : -1));
	if (*location > max) *location	= (r >> 10) % max;
	return b[r & 31];
}

u32 profile_d(u32 max, u32* location) 
{
	static u32	a[32]	= {	 1,		  1,	  2,	  2,	  3,	  3,	   4,	   4, 
							 5,		  5,	  6,	  6,	  7,	  7,	   8,	   8, 
							 9,		 10,	 12,	 15,	 18,	 23,	  39,	  51,
							79,		112,	191,	311,	517,	811,	1025,   1492};
	static u32	b[32]	= {	 1,		  1,	  1,	  1,	  1,	  1,	   1,	   1, 
							 1,		  1,	  1,	  1,	  2,	  2,	   2,	   2, 
							 2,		  2,	  3,	  3,	  3,	  3,	   4,	   4,
							 4,		  5,	  5,	  6,	  7,	  8,	  16,	  32};
	u32	r = rand();
	*location	=  *location + (a[(r >> 5) & 31] * ((r & 64) ? 1 : -1));
	if (*location > max) *location	= (r >> 10) % max;
	return b[r & 31];
}

u32 (*profiles [4])(u32 a, u32* b) = {profile_a, profile_b, profile_c, profile_d};



