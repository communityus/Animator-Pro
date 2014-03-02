#include "errcodes.h"
#include "idriver.h"
#include "input.h"

static LONG key_min[2], key_max[2] = {64,64}, 
	key_clip[2] = {64,64},
	key_aspect[2] = {1,1}, key_pos[2] = {25,25};
static UBYTE key_flags[2] = {RELATIVE,RELATIVE};

static Errcode key_detect(Idriver *idr)
{
	(void)idr;
	return(Success);
}

static Errcode key_inquire(Idriver *idr)
{
idr->does_keys = TRUE;
idr->button_count = 2;
idr->channel_count = 2;
idr->min = key_min;
idr->max = key_max;
key_clip[0] = key_max[0];
key_clip[1] = key_max[1];
idr->clipmax = key_clip;
idr->aspect = key_aspect;
idr->pos = key_pos;
idr->flags = key_flags;
return(Success);
}

static Errcode key_open(Idriver *idr)
{
key_inquire(idr);
return(Success);
}

static Errcode key_close(Idriver *idr)
{
(void)idr;
return(Success);
}

static Errcode key_input(Idriver *idr)
{
int temp;

temp = dos_key_shift();
idr->buttons = 0;
if (temp & 0x2)	/* pendown on alt */
	idr->buttons |= 0x1;
if (temp & 0x1)	/* right button on control */
	idr->buttons |= 0x2;
temp = idr->key_code = 0;
if (pj_key_is() != 0)
	temp = pj_key_in();
switch (temp)
	{
	case LARROW:
		key_pos[0] -= 1;
		break;
	case RARROW:
		key_pos[0] += 1;
		break;
	case UARROW:
		key_pos[1] -= 1;
		break;
	case DARROW:
		key_pos[1] += 1;
		break;
	default:
		idr->key_code = temp;
		break;
	}
return(Success);
}

static Errcode key_setclip(Idriver *idr,SHORT channel,long clipmax)
{
	if((USHORT)channel > idr->channel_count)
		return(Err_bad_input);

	if ((ULONG)clipmax > (ULONG)key_max[channel])
		clipmax = key_max[channel];
	key_clip[channel] = clipmax;
	return(0);
}

static Idr_library key_lib = {
	key_detect,
	key_inquire,
	key_input,
	key_setclip,
	};


Errcode init_key_idriver(Idriver *idr)
{
	idr->lib = &key_lib;
	idr->options = NULL;
	idr->hdr.init = key_open;
	idr->hdr.cleanup = key_close;
	return(Success);
}

