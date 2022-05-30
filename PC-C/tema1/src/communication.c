#include "communication.h"
#include "util_comm.h"

#include <stdio.h>


/* Task 1 - The Beginning */

void send_byte_message(void)
{
    /* TODO
     * Send the encoding of the characters: R, I, C, K
     */
    send_squanch(18);  
    send_squanch(9);
    send_squanch(3);
    send_squanch(11);
}


void recv_byte_message(void)
{
    /* TODO
     * Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     *
     * ATTENTION!: Use fprintf(stdout, ...)
     */

    char message;
    
    for (int i=0; i<5; i++){
        message = (char)(recv_squanch() + 64);  
        fprintf(stdout, "%c", message);
    }

}


void comm_byte(void)
{
    /* TODO
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */
    
    int code;
    for (int i=0; i<10; i++){
        code =recv_squanch();  
        for (int j=0; j<2; j++){
            send_squanch(code);
        }
    }
    
}


/* Task 2 - Waiting for the Message */

void send_message(void)
{
    /* TODO
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */

    send_squanch((uint8_t)(10 << 2)); 
    send_squanch(8);  
    send_squanch(5);
    send_squanch(12);
    send_squanch(12);
    send_squanch(15);
    send_squanch(20);
    send_squanch(8);
    send_squanch(5);
    send_squanch(18);
    send_squanch(5);

}


void recv_message(void)
{
    /* TODO
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     * 
     * ATTENTION!: Use fprintf(stdout, ...)
     */

    uint8_t len = ((uint8_t)(recv_squanch() << 2) >> 4); 
    uint8_t len1 = len % 10;  
    uint8_t len2 = (len / 10) % 10;  
    fprintf(stdout, "%u", len2); 
    fprintf(stdout, "%u", len1);
    for (int i=0; i<len; i++){
        char message = (char)(recv_squanch() + 64); 
        fprintf(stdout, "%c", message);
    }

}


void comm_message(void)
{
    /* TODO
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */

    uint8_t len = ((uint8_t)(recv_squanch() << 2) >> 4); 
    int check = 0;  
    for (int i=0; i<len; i++){  
        if (i == len-1){
            if (recv_squanch() == 16){
                check = 1;
            }
        }
        else{
            recv_squanch();
        }
    }

    if (check){  
        len = (uint8_t)(10 << 2);
        send_squanch(len);
        send_squanch(16);
        send_squanch(9);
        send_squanch(3);
        send_squanch(11);
        send_squanch(12);
        send_squanch(5);
        send_squanch(18);
        send_squanch(9);
        send_squanch(3);
        send_squanch(11);   
    }
    else{
        len = (uint8_t)(11 << 2);        
        send_squanch(len);
        send_squanch(22);
        send_squanch(9);
        send_squanch(14);
        send_squanch(4);
        send_squanch(9);
        send_squanch(3);
        send_squanch(1);
        send_squanch(20);
        send_squanch(15);
        send_squanch(18);   
        send_squanch(19);
    }

}


/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2)
{
    /* TODO
     * Steps:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     */

    uint8_t res = 0;
    uint8_t c1_copy = c1;  
    uint8_t c2_copy = c2;
    for (int i=0; i<8; i+=2){  
        if (c2_copy & 1){
            res = res | (1 << i);
        }
        if (c1_copy & 1){
            res = res | (1 << (i+1));
        }
        c1_copy >>= 1;  
        c2_copy >>= 1;
    }
    send_squanch(res); 

}


uint8_t decode_squanch2(uint8_t c)
{
    /*
     * Decode the given byte:
     * - split the two characters as in the image from ocw.cs.pub.ro
     */

    uint8_t res = 0;

    /* TODO */

    uint8_t c_copy = c;
    uint8_t mask_right = 1;  
    uint8_t mask_left = 1 << 4;
    for (int i=0; i<8; i+=2){     
        if (c_copy & (1 << i)){
            res = res | mask_right;
        }
        if (c_copy & (1 << (i + 1))){
            res = res | mask_left;
        }
        mask_right <<= 1;
        mask_left <<= 1;  
    }

    return res;
}
