#ifdef USE_SERIAL
#ifdef SERIAL_USE_MULTI_TRANSACTION
/* --- USE flexible API (using multi-type transaction function) --- */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <split_scomm.h>
#include "serial.h"
#ifdef SERIAL_DEBUG_MODE
#include <avr/io.h>
#endif
#ifdef CONSOLE_ENABLE
  #include <print.h>
#endif

uint8_t volatile serial_follower_buffer[SERIAL_FOLLOWER_BUFFER_LENGTH] = {0};
uint8_t volatile serial_leader_buffer[SERIAL_LEADER_BUFFER_LENGTH] = {0};
uint8_t volatile status_com = 0;
uint8_t volatile status1 = 0;
uint8_t follower_buffer_change_count = 0;
uint8_t s_change_old = 0xff;
uint8_t s_change_new = 0xff;

SSTD_t transactions[] = {
#define GET_FOLLOWER_STATUS 0
    /* leader buffer not changed, only recive follower_buffer_change_count */
    { (uint8_t *)&status_com,
      0, NULL,
      sizeof(follower_buffer_change_count), &follower_buffer_change_count,
    },
#define PUT_LEADER_GET_FOLLOWER_STATUS 1
    /* leader buffer changed need send, and recive follower_buffer_change_count  */
    { (uint8_t *)&status_com,
      sizeof(serial_leader_buffer), (uint8_t *)serial_leader_buffer,
      sizeof(follower_buffer_change_count), &follower_buffer_change_count,
    },
#define GET_FOLLOWER_BUFFER 2
    /* recive serial_follower_buffer */
    { (uint8_t *)&status1,
      0, NULL,
      sizeof(serial_follower_buffer), (uint8_t *)serial_follower_buffer
    }
};

void serial_leader_init(void)
{
    soft_serial_initiator_init(transactions, TID_LIMIT(transactions));
}

void serial_follower_init(void)
{
    soft_serial_target_init(transactions, TID_LIMIT(transactions));
}

// 0 => no error
// 1 => follower did not respond
// 2 => checksum error
int serial_update_buffers(int leader_update)
{
    int status, smatstatus;
    static int need_retry = 0;

    if( s_change_old != s_change_new ) {
        smatstatus = soft_serial_transaction(GET_FOLLOWER_BUFFER);
        if( smatstatus == TRANSACTION_END ) {
            s_change_old = s_change_new;
#ifdef CONSOLE_ENABLE
            uprintf("follower matrix = %b %b %b %b %b\n",
                    serial_follower_buffer[0], serial_follower_buffer[1],
                    serial_follower_buffer[2], serial_follower_buffer[3],
                    serial_follower_buffer[4] );
#endif
        }
    } else {
        // serial_follower_buffer dosen't change
        smatstatus = TRANSACTION_END; // dummy status
    }

    if( !leader_update && !need_retry) {
        status = soft_serial_transaction(GET_FOLLOWER_STATUS);
    } else {
        status = soft_serial_transaction(PUT_LEADER_GET_FOLLOWER_STATUS);
    }
    if( status == TRANSACTION_END ) {
        s_change_new = follower_buffer_change_count;
        need_retry = 0;
    } else {
        need_retry = 1;
    }
    return smatstatus;
}

#endif // SERIAL_USE_MULTI_TRANSACTION
#endif /* USE_SERIAL */
