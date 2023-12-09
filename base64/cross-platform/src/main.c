/**
 * @brief base64 test program
 *
 * Cross Platform Edition, based on atari version by Thomas Cherryhomes
 * 
 * @author Mark Fisher
 * @license gpl v. 3, see LICENSE for details.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "fujinet-io.h"

char test_in[20];
char test_out[20];

const char *test_text = "Hello World";
char encoded_text[40];
unsigned long encoded_len;

const char *test_base64 = "SGVsbG8gV29ybGQ=";
char decoded_text[40];
unsigned long decoded_len;

void main(void)
{
  printf("BASE64 TEST PROGRAM\n\n");

  printf("PERFORMING ENCODE TESTS\n\n");

  printf("ENCODE INPUT\n");
  
  if (fn_io_base64_encode_input((char *)test_text,strlen(test_text)) > 128)
    {
      printf("BASE64 ENCODE INPUT FAILED\n");
      exit(1);
    }

  printf("ENCODE COMPUTE\n");
  
  if (fn_io_base64_encode_compute() > 128)
    {
      printf("BASE64 ENCODE COMPUTE FAILED\n");
      exit(1);
    }

  printf("ENCODE LENGTH\n");
  
  if (fn_io_base64_encode_length(&encoded_len) > 128)
    {
      printf("BASE64 ENCODE LENGTH FAILED\n");
      exit(1);
    }
  else
    printf("ENCODED LENGTH: %lu\n",encoded_len);

  printf("ENCODE OUTPUT\n");
  
  if (fn_io_base64_encode_output(encoded_text,encoded_len) > 128)
    {
      printf("BASE64 ENCODE OUTPUT FAILED\n");
      exit(1);
    }
  else
    printf("ENCODED TEXT IS: %s\n",encoded_text);

  // ------------------------------------------------
  
  printf("\n\nPERFORMING DECODE TESTS\n\n");

  printf("DECODE INPUT\n");
  
  if (fn_io_base64_decode_input((char *)test_base64,strlen(test_base64)) > 128)
    {
      printf("BASE64 DECODE INPUT FAILED\n");
      exit(1);
    }

  printf("DECODE COMPUTE\n");
  
  if (fn_io_base64_decode_compute() > 128)
    {
      printf("BASE64 DECODE COMPUTE FAILED\n");
      exit(1);
    }

  printf("DECODE LENGTH\n");
  
  if (fn_io_base64_decode_length(&decoded_len) > 128)
    {
      printf("BASE64 DECODE LENGTH FAILED\n");
      exit(1);
    }
  else
    printf("DECODED LENGTH: %lu\n",decoded_len);

  printf("DECODE OUTPUT\n");
  
  if (fn_io_base64_decode_output(decoded_text,decoded_len) > 128)
    {
      printf("BASE64 ENCODE OUTPUT FAILED\n");
      exit(1);
    }
  else
    printf("DECODED TEXT IS: %s\n",decoded_text);

  printf("\nDONE.\n");

  cgetc();
}
