
/* names.c - general purpose string handling. A shell sort. */

#include <ctype.h>
#include <string.h>
#include "jimk.h"
#include "memory.h"

/* Force a string to upper case */
to_upper(s)
register UBYTE *s;
{
register UBYTE c;

while ((c = *s++) != 0)
	{
	if (islower(c))
		*(s-1) = _toupper(c);
	}
}

/* Convert all occurences of in character to out character in string */
tr_string(string, in, out)
register char *string;
register char in, out;
{
register char c;

while ((c = *string)!=0)
	{
	if ( c == in )
		*string = out;
	string++;
	}
}

/* count up nodes in a (singly linked) list */
els_in_list(list)
register Name_list *list;
{
register count = 0;

while (list)
	{
	count++;
	list = list->next;
	}
return(count);
}


cmp_name_list(l1,l2)
Name_list *l1, *l2;
{
return(strcmp(l1->name, l2->name) );
}


/* sort a list (first convert it into an array and sort array */
void *
sort_list(list, cmp)
register Name_list *list;
Vector cmp;
{
register Name_list **array, *pt, **array_pt;
register int elements, i;

elements = els_in_list(list);
if (elements <= 1)
	return(list);	/* length 0 or 1 lists already sorted */

array = (Name_list **)askmem( elements * sizeof(Name_list *) );
if (array)
	{
	pt = list;
	array_pt = array;
	while ( pt )
		{
		*array_pt++ = pt;
		pt = pt->next;
		}
	sort_array(array, elements, cmp);
	array_pt = array;
	list = NULL;
	i = elements;
	while (--i >= 0)
		{
		pt = *array_pt++;
		pt->next = list;
		list = pt;
		}
	freemem( array );
	}
return(list);
}

Name_list *
sort_name_list(Name_list *list)
{
sort_list(list, cmp_name_list);
}


/* a little shell sort on an array of strings */
sort_array(array, count, cmp)
Name_list **array;
int count;
Vector cmp;
{
register Name_list **pt1, **pt2;
register Name_list *swap;
register short swaps;
register int space, ct;

if (count >= 2)  /*very short arrays are already sorted*/
	{
	space = count/2;
	--count; /* since look at two elements at once...*/
	for (;;)
		{
		swaps = 1;
		while (swaps)
			{
			pt1 = array;
			pt2 = array + space;
			ct = count - space + 1;
			swaps = 0;
			while (--ct >= 0)
				{
				if ((*cmp)(*pt1, *pt2) < 0)
					{
					swaps = 1;
					swap = *pt1;
					*pt1 = *pt2;
					*pt2 = swap;
					}
				pt1++;
				pt2++;
				}
			}
		if ( (space = space/2) == 0)
			break;
		}
	}
}




char *
clone_string(s)
char *s;
{
char *d;

if ((d = (char *)askmem(strlen(s)+1)) != NULL)
	strcpy(d, s);
return(d);
}

unsigned char bitmasks[8] =
	{
	0x80, 0x40, 0x20, 0x10,
	0x8, 0x4, 0x2, 0x1,
	};

#ifdef SLUFFED
getbit(table, ix)
unsigned char *table;
register int ix;
{
return(table[(ix>>3)]&bitmasks[ix&7]);
}

setbit(table,ix)
unsigned char *table;
register int ix;
{
table[(ix>>3)] |= bitmasks[ix&7];
}
#endif /* SLUFFED */

void
free_name_list(Name_list *lst)
{
register Name_list *next;

while (lst)
	{
	next = lst->next;
	freemem(lst->name);
	freemem(lst);
	lst = next;
	}
}

longest_string(strings, count)
char **strings;
int count;
{
int acc, len;

acc = 0;
while (--count >= 0)
	{
	len = strlen(*strings++);
	if (len > acc)
		acc = len;
	}
return(acc);
}

remove_a_suffix(f)
char *f;
{
int i, len;

len = strlen(f);
i = 4;
if (i>len)
	i = len;
f += len;
while (--i >= 0)
	{
	if (*(--f) == '.')
		{
		*f = 0;
		break;
		}
	}
}
