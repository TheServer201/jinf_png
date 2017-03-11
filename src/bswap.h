/*
 * Byte swap
 *
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#include <stdint.h>
#include <string.h>

#define BsDword(g) ({				\
	uint32_t h = (g); 				\
	h = ((h & 0x000000FF) << 24) |	\
		((h & 0x0000FF00) <<  8) |	\
		((h & 0x00FF0000) >>  8) |	\
		((h & 0xFF000000) >> 24);	\
	h;								\
})

/* Why memcpy ? http://stackoverflow.com/a/32095106 */

#define ToByte(e) ({			\
	uint8_t f;					\
	memcpy(&f, e, sizeof(f));	\
	f;							\
})

#define ToDword(e) ({			\
	uint32_t f;					\
	memcpy(&f, e, sizeof(f));	\
	f;							\
})

#define ToQword(e) ({			\
	uint64_t f;					\
	memcpy(&f, e, sizeof(f));	\
	f;							\
})

#define RdByte(c) ({		\
	uint8_t d = ToByte(c);	\
	c += sizeof(uint8_t);	\
	d;						\
})

#define RdDword(c) ({			\
	uint32_t d = ToDword(c);	\
	c += sizeof(uint32_t);		\
	d;							\
})

#define RdQword(c) ({			\
	uint64_t d = ToQword(c);	\
	c += sizeof(uint64_t);		\
	d;							\
})

/*  On x86 (gcc 7)
	RsDword:
	mov     eax, DWORD PTR [esp+4]
	mov     eax, DWORD PTR [eax]
	bswap   eax
	ret
*/
#define RsDword(a) ({			\
	uint32_t b = RdDword(a);	\
	b = BsDword(b);				\
	b;							\
})
