#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <peekpoke.h>
#include <stdint.h>
#include "sp.h"


uint8_t sp_check_slot(uint8_t s)
{
    uint16_t a = 0xc000 + (s * 0x100);
    if ((PEEK(a+1) == 0x20) &&
        (PEEK(a+3) == 0x00) &&
        (PEEK(a+5) == 0x03) &&
        (PEEK(a+7) == 0x00))
	{
        return s;
	}

    // Did not find.
    return 0;
}


void main()
{
    uint16_t i;
	uint8_t s,j,unit,len;
	uint8_t found = 0;
	uint32_t block;
	char type,c;
	uint32_t high, mid, low;
	
	struct card{
        uint8_t sp;           //sp found
        uint16_t spentry;     //sp entry address
		uint8_t devices;      //total sp devices found
		uint16_t vendor;
		uint16_t interface_version;
		uint8_t smartportid;
		
    };
	
	struct card slot[8];

    uint32_t devblocks[12];

    clrscr();    
    printf("\n         Smartport Test Utility\n");
		
    //check slots for SP 
	for(s=7;s>0;s--)
	{
		if(sp_check_slot(s))
		{
			found = 1;
			slot[s].sp = 1;
			slot[s].spentry = sp_dispatch_address(s);
			if (!sp_status(slot[s].spentry, 0, 0))
			{
			    slot[s].devices = sp_payload[0];
			    slot[s].vendor = (sp_payload[3] * 256) + sp_payload[2];
			    slot[s].interface_version = (sp_payload[5] * 256) + sp_payload[4];
				slot[s].smartportid = PEEK(0xc000 + s*0x100 + 0xfb);
			}
		}
		else
		{
			slot[s].sp = 0;
		}
	}
	

    if (found)
	{
		printf("\nSP Cards found:\n");
		printf("\nSlot Entry Devs SPid Vendor Version\n");
		
		for(s=7;s>0;s--)
	    {
            if (slot[s].sp)
			{
		        printf("%01d    %04X  %d    %02X   %04X   %04X\n", s, slot[s].spentry, slot[s].devices, slot[s].smartportid, slot[s].vendor, slot[s].interface_version );
			}
		}
        
		printf("\nEnter Slot for testing (1-7)?");
        do
    	{
    		scanf("%d",&s);
    	} while (s > 7 || s < 1);
		
		clrscr();
        printf("\nSP Units found in Slot:%d\n",s);
        printf("\n# ID               Blks   Volume\n");
		for (unit = 1; unit <= slot[s].devices; unit++)
		{
			//get units dib
			sp_status(slot[s].spentry, unit, 3);
			printf("%d ",unit);
			
			for(j=0;j<16;j++) //output the ID
			{
				cputc(sp_payload[5+j]);
			}
		
			if (sp_payload[0]&0x80) //block or char?
			{
				high = ((uint32_t)(sp_payload[3]) << 16);
			    mid = ((uint32_t)(sp_payload[2]) << 8);
			    low = sp_payload[1];
			
			    devblocks[unit] = high + mid + low;
			
		        printf(" %06lX ", devblocks[unit]);
			
			    //lets get the volume name from the key block
			    if (devblocks[unit])  //only if numblocks > 0
			    {
			    	sp_readblock(slot[s].spentry, unit, 2);
			        len = sp_payload[4]&0x0f;
			        for(j=0;j<len;j++) //output the vol name
			        {
			        	cputc(sp_payload[5+j]);
			        }
			    }
			    printf("\n");
			}
			else
			{
				printf(" -char-\n");
			}
		}

		printf("\nEnter Unit# for Testing?");
        do
    	{
    		scanf("%d",&unit);
    	} while (unit < 1 || unit > slot[s].devices);

		printf("\nRead/Write/write with Verify (R/W/V)?");
		do
    	{
    		scanf(" %1c",&type);
    	} while (type != 'R' && type != 'r' && type != 'W' && type != 'w' && type != 'V' && type != 'v');
		
		
		if (type =='R' || type == 'r')
		{
			printf("\nReading from Unit:%d Block:",unit);
			for (block=0;block < devblocks[unit];block++)
			{
				gotox(26);
				printf("%06lX",block);
				if (sp_readblock(slot[s].spentry, unit, block))
				{
					printf("\n Error reading block: %06lX\n", block);
					cgetc();
					exit(0);
				}
			}
			printf("\n\nCompleted successfully!");
			cgetc();
		}
		else if (type == 'W' || type == 'w')
		{
		    printf("\nThis will overwrite the whole disk\n  Y to confirm?");
		    do
    	    {
    	    	scanf(" %1c",&c);
    	    } while (c != 'Y' && c != 'y');


			for (i=0;i<512;i++)
			{
				sp_payload[i] = 0xaa;
			}
			
			printf("\nWriting to unit:%d Block:",unit);
			for (block = 0; block < devblocks[unit]; block++)
			{
				gotox(24);
				printf("%06lX",block);
				if (sp_writeblock(slot[s].spentry, unit, block))
				{
					printf("\n Error writing block: %06lX\n", block);
					cgetc();
					exit(0);
				}
			}
			printf("\n\nCompleted successfully!");
			cgetc();
		}
		else
		{
			printf("\nThis will overwrite the whole disk\n  Y to confirm?");
		    do
    	    {
    	    	scanf(" %1c",&c);
    	    } while (c != 'Y' && c != 'y');


			for (i=0;i<512;i++)
			{
				sp_payload[i] = 0xaa;
			}
			
			printf("\nWriting/Verify to unit:%d Block:",unit);
			for (block = 0; block < devblocks[unit]; block++)
			{
				gotox(31);
				printf("%06lX",block);
				if (sp_writeblock(slot[s].spentry, unit, block))
				{
					printf("\n Error writing block: %06lX\n", block);
					cgetc();
					exit(0);
				}
				if (sp_readblock(slot[s].spentry, unit, block))
				{
					printf("\n Error reading block: %06lX\n", block);
					cgetc();
					exit(0);
				}
			    for (i=0;i<512;i++)
			    {
			    	if (sp_payload[i] != 0xaa)
					{
						printf("\n Error verifying block: %06lX\n", block);
					    cgetc();
					    exit(0);
					}
			    }
			}
			printf("\n\nCompleted successfully!");
			cgetc();
		}
	} 
	else
	{
		printf("\n  No SP Cards found!\n");
		printf("\n  Hit a key to exit\n");
		cgetc();
	}


    exit(0);
}

