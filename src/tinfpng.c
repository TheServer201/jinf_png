/*
 * tinfpng  -  tiny png decompressor
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

#include "tinf.h"

#define PNGS 0x0A1A0A0D474E5089

#define IHDR 0x52444849
#define IDAT 0x54414449

int tinf_png_inspect(const void *source, tinf_png_info *info)
{
	uint8_t *src = (uint8_t *)source, dep, clt, cpm, ftm, itm;
	uint32_t len, typ;
	int32_t wid, hei;
	uint64_t sgt;
	
	/* -- check format -- */
	sgt = RdQword(src);
	len = RsDword(cur);
	typ = RdDword(cur);
	wid = RsDword(cur);
	hei = RsDword(cur);
	dep = RdByte(cur);
	clt = RdByte(cur);
	cpm = RdByte(cur);
	ftm = RdByte(cur);
	itm = RdByte(cur);
	
	if (sgt != PNGS) return TINF_DATA_ERROR;
	if (len != 0x0D) return TINF_DATA_ERROR;
	if (typ != IHDR) return TINF_DATA_ERROR;
	if (wid <= 0x00 || hei <= 0x00) return TINF_DATA_ERROR;
	if (dep != 0x08 && dep != 0x10) return TINF_DATA_ERROR;
	if (clt != 0x06) return TINF_DATA_ERROR;
	if (cpm != 0x00) return TINF_DATA_ERROR;
	if (ftm != 0x00) return TINF_DATA_ERROR;
	if (itm != 0x00) return TINF_DATA_ERROR;
	
	/* -- extract infos -- */
	info->width = wid;
	info->height = hei;
	info->depth = dep;
	info->size = hei + ((wid * hei) / 8) * dep;
	indo->data = src + 4;
	
	return TINF_OK;
}

int tinf_png_uncompress(void *dest, tinf_png_info info)
{
	uint8_t *src = info.data,
			*dst = (uint8_t *)dest;
	uint32_t len, typ;
	
	/* -- foreach chunks -- */
	do {
		len = RsDword(src);
		typ = RdDword(src);
		
		switch (typ) {
			case IDAT:
				/* -- inflate -- */
				uint32_t siz;
				
				int32_t res = tinf_zlib_uncompress(dst, &siz, src, len);
				
				if (res != TINF_OK) return res;
				
				dst += siz;
				break;
			case IEND:
				goto Done;
			default:
				src += len;
				break;
		}
		
		src += 4;
	} while(0x01);
	
Done:
	return TINF_OK;
}
