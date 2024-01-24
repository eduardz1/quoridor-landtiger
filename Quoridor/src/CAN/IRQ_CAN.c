#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6000000)
// no previous prototype for function 'CAN_IRQHandler'
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif

/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "../GLCD/GLCD.h"
#include "../common.h"
#include "CAN.h"
#include <LPC17xx.h>

extern uint8_t icr; // icr and result must be global in order to work with both
                    // real and simulated landtiger.
extern uint32_t result;
extern CAN_msg CAN_TxMsg; /* CAN message for sending */
extern CAN_msg CAN_RxMsg; /* CAN message for receiving */

union Move fetched_move;

bool handshake_successful = false;

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler(void)
{

    /* check CAN controller 1 */
    icr = 0;
    icr = (LPC_CAN1->ICR | icr) & 0xFF; /* clear interrupts */

    if (icr & (1 << 0))
    {                             /* CAN Controller #1 meassage is received */
        CAN_rdMsg(1, &CAN_RxMsg); /* Read the message */
        LPC_CAN1->CMR = (1 << 2); /* Release receive buffer */

        fetched_move.as_uint32_t = CAN_RxMsg.data[0] << 24 |
                                   CAN_RxMsg.data[1] << 16 |
                                   CAN_RxMsg.data[2] << 8 | CAN_RxMsg.data[3];

        if (!handshake_successful && fetched_move.player_id == 0xFF)
        {
            handshake_successful = true;
            return;
        }

        // move(fetched_move); // TODO: implement
    }
}
