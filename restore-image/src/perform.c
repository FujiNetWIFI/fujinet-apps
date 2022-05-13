/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <string.h>
#include "perform.h"
#include "typedefs.h"
#include "system.h"

extern char response[1024];
extern State state;
extern char path[128];

unsigned char src[16384], dst[16384];
PFState pfState, pfStateRetry;

unsigned long readBlockNum, writeBlockNum, verifyBlockNum;
unsigned short readOffset, writeOffset, verifyOffset;
unsigned char passBlocks, writtenBlocks;

bool eom=false;

void perform_init(void)
{
  io_perform_open(path);
  pfState=PF_CONFIRM;
  readBlockNum=writeBlockNum=verifyBlockNum=0;
  readOffset=writeOffset=verifyOffset=0;
  eom=false;
}

void perform_confirm(void)
{
  screen_perform_confirm();
  
  if (input_perform_confirm())
    pfState=PF_RESTORE;
  else
    pfState=PF_ABORT;
}

void perform_read(void)
{
  readOffset=0;
  passBlocks=0;
  
  while (readOffset < sizeof(src))
    {
      screen_perform_read(readBlockNum);

      if (!io_perform_read_block(readBlockNum,&src[readOffset]))
	{
	  pfState=PF_ERROR;
	  pfStateRetry=PF_RESTORE;
	  return;
	}

      if (io_perform_eom())
	{
	  eom=true;
	  return;
	}

      readOffset += 1024;
      readBlockNum++;
      passBlocks++;
    }
}

void perform_write(void)
{
  writeOffset=0;
  writtenBlocks=0;

  if (passBlocks==0)
    return;
  
  while (writtenBlocks < passBlocks)
    {
      screen_perform_write(writeBlockNum);

      if (!io_perform_write_block(selected_tape,writeBlockNum,&src[writeOffset]))
	{
	  pfState=PF_ERROR;
	  pfStateRetry=PF_RESTORE;
	  return;
	}

      writeOffset += 1024;
      writeBlockNum++;
      writtenBlocks++;
    }
}

void perform_verify(void)
{
  unsigned short i;
  
  verifyOffset=0;

  if (passBlocks==0)
    {
      eom=true;
      return;
    }
  
  for (i=0;i<passBlocks;i++)
    {
      screen_perform_verify_read(verifyBlockNum);
      
      if (!io_perform_verify_block(selected_tape,verifyBlockNum,&dst[verifyOffset]))
	{
	  pfState=PF_ERROR;
	  pfStateRetry=PF_RESTORE;
	  return;
	}
      
      verifyOffset += 1024;
      verifyBlockNum++;
    }

  verifyOffset=0;

  for (i=0;i<sizeof(dst);i++)
    {
      if (src[i] != dst[i])
	{
	  pfState=PF_ERROR;
	  return;
	}
    }
}

void perform_restore(void)
{
  screen_perform_restore();
  
  while (eom==false)
    {
      perform_read();
      perform_write();
      perform_verify();
    }

  pfState=PF_DONE;
}

void perform_abort(void)
{
  screen_perform_abort();
  state=SELECT_HOST;
}

void perform_done(void)
{
  screen_perform_done();
  io_perform_rewind();
  pfState=input_perform_done();
}

void perform_error(void)
{
  screen_perform_error();
  input_perform_error();
}

State perform(void)
{
  pfState = PF_INIT;
  
  while(1)
    {
      switch(pfState)
	{
	case PF_INIT:
	  perform_init();
	  break;
	case PF_CONFIRM:
	  perform_confirm();
	  break;
	case PF_RESTORE:
	  perform_restore();
	  break;
	case PF_ABORT:
	  perform_abort();
	  break;
	case PF_DONE:
	  perform_done();
	  break;
	case PF_ERROR:
	  perform_error();
	  break;
	case PF_RESTART:
	  return SELECT_HOST;
	}
    }
  return DONE;
}
