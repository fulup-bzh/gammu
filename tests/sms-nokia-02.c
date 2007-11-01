/* Test for decoding SMS on Nokia 6110 driver */

#include <gammu.h>
#include <stdlib.h>
#include <stdio.h>
#include "../common/protocol/protocol.h" /* Needed for GSM_Protocol_Message */
#include "../common/gsmstate.h" /* Needed for state machine internals */

#include "sms-printing.h"

unsigned char data[] = {
	0x01, 0x08, 0x00, 0x08, 0x01, 0x02, 0x01, 0x00, 0x05, 0x91, 0x26, 0x18, 0x16, 0x42, 0x00, 0xF0, 
	0x10, 0x00, 0x05, 0x91, 0x00, 0x00, 0x00, 0xA0, 0x0E, 0xD0, 0x49, 0x27, 0xF1, 0x39, 0x0D, 0x52, 
	0x41, 0x0B, 0x0C, 0x91, 0x70, 0x50, 0x03, 0x11, 0x61, 0x13, 0x00, 0xD2, 0x70, 0x1A, 0x0D, 0xA2, 
	0xC9, 0x40, 0xD6, 0x77, 0x7D, 0x8C, 0x2E, 0xCB, 0x41, 0xC2, 0x32, 0x3B, 0xEC, 0x56, 0x87, 0x41, 
	0x52, 0xB8, 0xAC, 0x06, 0x93, 0x8B, 0x5B, 0x52, 0xB8, 0x4C, 0x49, 0x47, 0x05, 0xE1, 0xF2, 0x96, 
	0xB2, 0xEE, 0x86, 0xDD, 0x52, 0x2E, 0x6A, 0xDA, 0x7D, 0x5E, 0x87, 0xE9, 0xEB, 0xB0, 0x1B, 0x04, 
	0x2F, 0xB7, 0xC3, 0xEB, 0x70, 0x3A, 0xEC, 0x06, 0x05, 0xDD, 0xE4, 0x30, 0xC8, 0x04, 0x4A, 0xB7, 
	0xE4, 0x69, 0xF7, 0x19, 0x14, 0x5E, 0xD3, 0xD3, 0x66, 0xD7, 0xB2, 0x4C, 0x4F, 0xAF, 0x75, 0x52, 
	0x5A, 0x95, 0x37, 0x87, 0x87, 0xE7, 0x69, 0x9F, 0x33, 0xDC, 0x0E, 0x8F, 0x90, 0x6F, 0x71, 0x1A, 
	0xB4, 0x2E, 0xDF, 0x6E, 0xB7, 0x1B, 0x4A, 0x0A, 0x9F, 0xD5, 0x60, 0x2B, 0x28, 0xD4, 0x99, 0x72, 
	0x4D, 0xF3, 0x61, 0x79, 0x98, 0x0E, 0x32, 0x81, 0xD6, 0x65, 0x7A, 0xD9, 0x4D, 0xAF, 0x87, 0xDD, 
	0x20, 0x71, 0x59, 0xCE, 0x0E, 0xAF, 0xEB,  
	};

/* This is not part of API! */
extern GSM_Error N6110_ReplyGetSMSMessage(GSM_Protocol_Message msg, GSM_StateMachine *s);

int main(int argc UNUSED, char **argv UNUSED)
{
	GSM_Debug_Info *debug_info;
	GSM_StateMachine *s;
	GSM_Protocol_Message msg;
	GSM_Error error;
	GSM_MultiSMSMessage sms;

	debug_info = GSM_GetGlobalDebug();
	GSM_SetDebugFileDescriptor(stderr, debug_info);
	GSM_SetDebugLevel("textall", debug_info);

	/* Allocates state machine */
	s = GSM_AllocStateMachine();
	if (s == NULL) {
		printf("Could not allocate state machine!\n");
		return 1;
	}

	/* Init message */
	msg.Type = 0x14;
	msg.Length = 0xb7;
	msg.Buffer = data;

	s->Phone.Data.GetSMSMessage = &sms;

	/* Parse it */
	error = N6110_ReplyGetSMSMessage(msg, s);

	/* Display message */
	DisplayTestSMS(sms);

	/* Free state machine */
	GSM_FreeStateMachine(s);

	printf("%s\n", GSM_ErrorString(error));

	return (error == ERR_NONE) ? 0 : 1;
}

/* Editor configuration
 * vim: noexpandtab sw=8 ts=8 sts=8 tw=72:
 */
