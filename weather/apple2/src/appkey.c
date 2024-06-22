#include	<stdlib.h>
#include	<string.h>
#include	<stdbool.h>
#include	"fujinet-fuji.h"
#include	"weatherui.h"
#include	"appkey.h"

extern char appkey[];
//extern char default_key[];

void get_appkey() {
	
	uint16_t	len;
	bool		status;

	fuji_set_appkey_details(APPKEY_CREATOR, APPKEY_APPL, 32); 
	status = fuji_read_appkey(APPKEY_ID, &len, (unsigned char *)appkey);
	if ((!status) || (len == 0)) {
		//strcpy(appkey, default_key);
		disp_nokey();
		exit(1);
	}
}
