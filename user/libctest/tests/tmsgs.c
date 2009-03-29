/**
 * @version		$Id$
 * @author		Nils Asmussen <nils@script-solution.de>
 * @copyright	2008 Nils Asmussen
 */

#include <common.h>
#include <messages.h>
#include <test.h>
#include <string.h>
#include <stdarg.h>
#include "tmsgs.h"

/* forward declarations */
static void test_msgs(void);
static void test_dataMsg(void);
static void test_binMsg(void);
static void test_binDataMsg(void);

/* our test-module */
sTestModule tModMsgs = {
	"Messages",
	&test_msgs
};

static void test_msgs(void) {
	test_dataMsg();
	test_binMsg();
	test_binDataMsg();
}

static void test_dataMsg(void) {
	const char *data = "Test";
	sMsgHeader *msg;

	test_caseStart("Testing asmDataMsg()");

	msg = asmDataMsg(MSG_ENV_SET,strlen(data) + 1,data);
	test_assertTrue(msg != NULL);
	test_assertUInt(msg->id,MSG_ENV_SET);
	test_assertUInt(msg->length,strlen(data) + 1);
	test_assertStr((const char*)(msg + 1),data);
	freeMsg(msg);

	test_caseSucceded();
}

static void test_binMsg(void) {
	sMsgHeader *msg;
	u8 b1,b2;
	u16 w1,w2;
	u32 dw1,dw2;

	test_caseStart("Testing asmDataMsg()");

	msg = asmBinMsg(MSG_ENV_SET,"124421",0xab,0xaffe,0xff00ff00,0x00ff00ff,0xdead,0xde);
	test_assertTrue(msg != NULL);
	test_assertTrue(disasmBinMsg(msg + 1,"124421",&b1,&w1,&dw1,&dw2,&w2,&b2));
	test_assertUInt(b1,0xab);
	test_assertUInt(w1,0xaffe);
	test_assertUInt(dw1,0xff00ff00);
	test_assertUInt(dw2,0x00ff00ff);
	test_assertUInt(w2,0xdead);
	test_assertUInt(b2,0xde);
	freeMsg(msg);

	test_caseSucceded();
}

static void test_binDataMsg(void) {
	sMsgHeader *msg;
	const char *str = "mystring";
	char buffer[20];
	u8 b1,b2;
	u16 w1,w2;
	u32 dw1,dw2;

	test_caseStart("Testing asmBinDataMsg()");

	msg = asmBinDataMsg(MSG_ENV_SET,str,strlen(str) + 1,"124421",0xab,0xaffe,0xff00ff00,0x00ff00ff,
			0xdead,0xde);
	test_assertTrue(msg != NULL);
	test_assertTrue(disasmBinDataMsg(msg->length,msg + 1,(u8**)&buffer,"124421",&b1,&w1,&dw1,&dw2,&w2,&b2));
	test_assertUInt(b1,0xab);
	test_assertUInt(w1,0xaffe);
	test_assertUInt(dw1,0xff00ff00);
	test_assertUInt(dw2,0x00ff00ff);
	test_assertUInt(w2,0xdead);
	test_assertUInt(b2,0xde);
	freeMsg(msg);

	test_caseSucceded();
}
