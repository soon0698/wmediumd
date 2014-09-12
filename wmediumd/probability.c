/*
 *	wmediumd, wireless medium simulator for mac80211_hwsim kernel module
 *	Copyright (c) 2011 cozybit Inc.
 *
 *	Author:	Javier Lopez	<jlopex@cozybit.com>
 *		Javier Cardona	<javier@cozybit.com>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version 2
 *	of the License, or (at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 *	02110-1301, USA.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "probability.h"
#include "ieee80211.h"

static int array_size = 0;
static struct mac_address *indexer;


void put_mac_address(struct mac_address addr, int pos)
{
	int i;
	void *ptr = indexer;

 	for (i=0; i < pos ; i++) {
 		ptr = ptr + sizeof(struct mac_address);
 	}
 	memcpy(ptr, &addr, sizeof(struct mac_address));
}


/*
 * returns the a mac_address ptr for a given index
 */

struct mac_address * get_mac_address(int pos) {

	void * ptr = indexer;
	ptr = ptr + (sizeof(struct mac_address)*pos);

	return ((pos >= array_size) ?  NULL : (struct mac_address*)ptr);
}

/*
 * 	Returns the position of the address in the array.
 * 	If the mac_address is not found returns -1
 */

int find_pos_by_mac_address(struct mac_address *addr) {

	int i=0;

	void * ptr = indexer;
	while(memcmp(ptr,addr,sizeof(struct mac_address)) && i < array_size)
	{
		i++;
		ptr = ptr + sizeof(struct mac_address);
	}

	return ((i >= array_size) ?  -1 :  i);
}

/*
 * 	Prints the values of the Mac Adress Array
 */

void print_mac_address_array() {

	int i=0;
	void * ptr = indexer;

	while (i < array_size) {
		struct mac_address *a = malloc(sizeof(struct mac_address));
		memcpy(a,ptr,sizeof(struct mac_address));
		printf("A[%d]:%02X:%02X:%02X:%02X:%02X:%02X\n",
		       i,a->addr[0], a->addr[1], a->addr[2],
		       a->addr[3], a->addr[4], a->addr[5]);
		i++;
		ptr = ptr + sizeof(struct mac_address);
	}
}

/*
 *	Init all the probability data
 *	Returns a pointer to the probability matrix
 */
double * init_probability(int size) {

	array_size = size;
	indexer = malloc(sizeof(struct mac_address)*array_size);

	if (indexer==NULL) {
		printf("Problem allocating vector");
		exit(1);
	}
}
