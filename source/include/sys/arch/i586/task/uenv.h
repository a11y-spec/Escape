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

#pragma once

#include <sys/common.h>

class UEnv : public UEnvBase {
	friend class UEnvBase;

	UEnv() = delete;

	static void startSignalHandler(Thread *t,sIntrptStackFrame *stack,int sig,
	                               Signals::handler_func handler);
	static void setupRegs(sIntrptStackFrame *frame,uintptr_t entryPoint);
	static uint32_t *addArgs(Thread *t,uint32_t *esp,uintptr_t tentryPoint,bool newThread);
};

inline void UEnvBase::handleSignal(Thread *t,sIntrptStackFrame *stack) {
	int sig;
	Signals::handler_func handler;
	int res = Signals::checkAndStart(t->getTid(),&sig,&handler);
	if(res == SIG_CHECK_CUR)
		UEnv::startSignalHandler(t,stack,sig,handler);
	else if(res == SIG_CHECK_OTHER)
		Thread::switchAway();
}
