/**
 * $Id$
 * Copyright (C) 2008 - 2014 Nils Asmussen
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

#pragma once

#include <esc/common.h>
#include <esc/fsinterface.h>
#include <stddef.h>
#include <time.h>

/* general */
#define IPC_DEF_SIZE				256

/* == messages == */
/* default response */
#define MSG_DEF_RESPONSE			100000

/* requests to file-device */
#define MSG_FILE_OPEN				50
#define MSG_FILE_READ				51
#define MSG_FILE_WRITE				52
#define MSG_FILE_CLOSE				53
#define MSG_FILE_SHFILE				54

/* responses of file-devices */
#define MSG_FILE_OPEN_RESP			100001
#define MSG_FILE_READ_RESP			100002
#define MSG_FILE_WRITE_RESP			100003
#define MSG_FILE_CLOSE_RESP			100004
#define MSG_FILE_SHFILE_RESP		100005

/* requests to fs */
#define MSG_FS_OPEN					100
#define MSG_FS_READ					101
#define MSG_FS_WRITE				102
#define MSG_FS_CLOSE				103
#define MSG_FS_STAT					104
#define MSG_FS_SYNCFS				105
#define MSG_FS_LINK					106
#define MSG_FS_UNLINK				107
#define MSG_FS_MKDIR				108
#define MSG_FS_RMDIR				109
#define MSG_FS_ISTAT				110
#define MSG_FS_CHMOD				111
#define MSG_FS_CHOWN				112

/* responses of fs */
#define MSG_FS_OPEN_RESP			100006
#define MSG_FS_READ_RESP			100007
#define MSG_FS_WRITE_RESP			100008
#define MSG_FS_STAT_RESP			100009
#define MSG_FS_SYNCFS_RESP			100010
#define MSG_FS_LINK_RESP			100011
#define MSG_FS_UNLINK_RESP			100012
#define MSG_FS_MKDIR_RESP			100013
#define MSG_FS_RMDIR_RESP			100014
#define MSG_FS_ISTAT_RESP			100015
#define MSG_FS_CHMOD_RESP			100016
#define MSG_FS_CHOWN_RESP			100017

/* == Other messages == */
#define MSG_SPEAKER_BEEP			200	/* performs a beep */

#define MSG_WIN_CREATE				300	/* creates a window */
#define MSG_WIN_MOVE				301	/* moves a window */
#define MSG_WIN_UPDATE				302	/* requests an update of a window */
#define MSG_WIN_DESTROY				303	/* destroys a window */
#define MSG_WIN_RESIZE				304	/* resizes a window */
#define MSG_WIN_ENABLE				305	/* enables the window-manager */
#define MSG_WIN_DISABLE				306	/* disables the window-manager */
#define MSG_WIN_ADDLISTENER			307	/* announces a listener for CREATE_EV or DESTROY_EV */
#define MSG_WIN_REMLISTENER			308	/* removes a listener for CREATE_EV or DESTROY_EV */
#define MSG_WIN_SET_ACTIVE			309	/* requests that a window is set to the active one */
#define MSG_WIN_ATTACH				310 /* connect an event-channel to the request-channel */
#define MSG_WIN_SETMODE				311 /* sets the screen mode */
#define MSG_WIN_EVENT				312 /* for all events */

#define MSG_SCR_SETCURSOR			500	/* sets the cursor */
#define MSG_SCR_GETMODE				501 /* gets information about the current video-mode */
#define MSG_SCR_SETMODE				502	/* sets the video-mode */
#define MSG_SCR_GETMODES			503 /* gets all video-modes */
#define MSG_SCR_UPDATE				504 /* updates a part of the screen */

#define MSG_VT_GETFLAG				600	/* gets a flag */
#define MSG_VT_SETFLAG				601	/* sets a flag */
#define MSG_VT_BACKUP				602	/* backups the screen */
#define MSG_VT_RESTORE				603	/* restores the screen */
#define MSG_VT_SHELLPID				604	/* gives the vterm the shell-pid */
#define MSG_VT_ISVTERM				605 /* dummy message on which only vterm answers with no error */
#define MSG_VT_SETMODE				606 /* requests vterm to set the video-mode */

#define MSG_KB_EVENT				700 /* events that the keyboard-driver sends */

#define MSG_MS_EVENT				800 /* events that the mouse-driver sends */

#define MSG_UIM_GETKEYMAP			900 /* gets the current keymap path */
#define MSG_UIM_SETKEYMAP			901	/* sets a keymap, expects the keymap-path as argument */
#define MSG_UIM_EVENT				902	/* the message-id for sending events to the listeners */
#define MSG_UIM_ATTACH				903 /* is used to attach to the ctrl-session */
#define MSG_UIM_GETID				904 /* get the id to use for attach */

#define MSG_PCI_GET_BY_CLASS		1000	/* searches for a pci device with given class */
#define MSG_PCI_GET_BY_ID			1001	/* searches for a pci device with given id */
#define MSG_PCI_GET_BY_INDEX		1002
#define MSG_PCI_GET_COUNT			1003

#define MSG_INIT_REBOOT				1100 /* requests a reboot */
#define MSG_INIT_SHUTDOWN			1101 /* requests a shutdown */
#define MSG_INIT_IAMALIVE			1102 /* tells init that the shutdown-request has been received
											and that you promise to terminate as soon as possible */

#define MSG_DISK_GETSIZE			1200 /* get the size of a device */

#define MSG_NIC_GETMAC				1300

#define MSG_NET_LINK_ADD			1401 /* adds a link */
#define MSG_NET_LINK_REM			1402 /* removes a link */
#define MSG_NET_LINK_CONFIG			1403 /* sets the ip, gateway and subnet-mask of a link */
#define MSG_NET_LINK_MAC			1404 /* gets the MAC address of a link */
#define MSG_NET_ROUTE_ADD			1405 /* adds an entry to the routing table */
#define MSG_NET_ROUTE_REM			1406 /* removes an entry from the routing table */
#define MSG_NET_ROUTE_CONFIG		1407 /* sets the status of a routing table entry */
#define MSG_NET_ROUTE_GET			1408 /* gets the destination for an IP address */
#define MSG_NET_ARP_ADD				1409 /* resolves an IP address and puts it into the ARP table */
#define MSG_NET_ARP_REM				1410 /* removes an IP address from the ARP table */

#define MSG_SOCK_BIND				1500 /* bind a socket to an address */
#define MSG_SOCK_RECVFROM			1501 /* receive data from a socket */
#define MSG_SOCK_SENDTO				1502 /* send data to a socket */

#define MSG_DNS_RESOLVE				1600 /* resolve a name to an address */
#define MSG_DNS_SET_SERVER			1601 /* set the name server */


#define IS_DEVICE_MSG(id)			((id) == MSG_FILE_OPEN || \
									 (id) == MSG_FILE_READ || \
									 (id) == MSG_FILE_WRITE || \
									 (id) == MSG_FILE_CLOSE || \
									 (id) == MSG_FILE_SHFILE)
