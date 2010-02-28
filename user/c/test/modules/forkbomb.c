/**
 * $Id$
 * Copyright (C) 2008 - 2009 Nils Asmussen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <esc/common.h>
#include <esc/proc.h>
#include <esc/fileio.h>
#include <esc/signals.h>
#include "forkbomb.h"

#define MAX_PIDS	2048

s32 pids[MAX_PIDS];

int mod_forkbomb(int argc,char *argv[]) {
	UNUSED(argc);
	UNUSED(argv);
	u32 i = 0;
	while(1) {
		pids[i] = fork();
		/* failed? so send all created child-procs the kill-signal */
		if(pids[i] < 0) {
			printf("Fork() failed, so kill all childs...\n");
			flush();
			while(i-- > 0) {
				if(sendSignalTo(pids[i],SIG_KILL,0) < 0)
					printe("Unable to send SIG_KILL to %d\n",pids[i]);
				waitChild(NULL);
			}
			printf("Done :)\n");
			return 0;
		}
		/* the childs break here */
		if(pids[i] == 0)
			break;
		i++;
	}

	/* now stay here until we get killed ^^ */
	printf("Child %d running...\n",getpid());
	flush();
	while(1)
		sleep(1000);
	return 0;
}
