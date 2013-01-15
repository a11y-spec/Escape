/**
 * $Id$
 * Copyright (C) 2008 - 2011 Nils Asmussen
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
#include <esc/driver/video.h>
#include <esc/messages.h>
#include <esc/io.h>
#include <string.h>

int video_setCursor(int fd,const sVTPos *pos) {
	sDataMsg msg;
	memcpy(&msg.d,pos,sizeof(sVTPos));
	return send(fd,MSG_VID_SETCURSOR,&msg,sizeof(msg));
}

int video_getSize(int fd,sVTSize *size) {
	sDataMsg msg;
	int res = send(fd,MSG_VID_GETSIZE,NULL,0);
	if(res < 0)
		return res;
	res = IGNSIGS(receive(fd,NULL,&msg,sizeof(msg)));
	if(res < 0)
		return res;
	if(msg.arg1 != sizeof(sVTSize))
		return msg.arg1;
	memcpy(size,&msg.d,sizeof(sVTSize));
	return 0;
}

int video_getMode(int fd) {
	sArgsMsg msg;
	int res = send(fd,MSG_VID_GETMODE,NULL,0);
	if(res < 0)
		return res;
	res = IGNSIGS(receive(fd,NULL,&msg,sizeof(msg)));
	if(res < 0)
		return res;
	return msg.arg1;
}

int video_setMode(int fd,int mode) {
	sArgsMsg msg;
	msg.arg1 = mode;
	int res = send(fd,MSG_VID_SETMODE,&msg,sizeof(msg));
	if(res < 0)
		return res;
	res = IGNSIGS(receive(fd,NULL,&msg,sizeof(msg)));
	if(res < 0)
		return res;
	return msg.arg1;
}

ssize_t video_getModeCount(int fd) {
	sMsg msg;
	msg.args.arg1 = 0;
	ssize_t res = send(fd,MSG_VID_GETMODES,&msg,sizeof(msg.args));
	if(res < 0)
		return res;
	res = IGNSIGS(receive(fd,NULL,&msg,sizeof(msg.args)));
	if(res < 0)
		return res;
	return msg.args.arg1;
}

ssize_t video_getModes(int fd,sVTMode *modes,size_t count) {
	sArgsMsg msg;
	ssize_t err;
	msg.arg1 = count;
	err = send(fd,MSG_VID_GETMODES,&msg,sizeof(msg));
	if(err < 0)
		return err;
	return IGNSIGS(receive(fd,NULL,modes,sizeof(sVTMode) * count));
}
