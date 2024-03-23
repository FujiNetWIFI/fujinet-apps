#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_fnlib.h"

char *version = "v1.0.4";

char *sHostName = "hostname:";
char *sIP = "ip:";
char *sGateway = "gateway:";
char *sNetmask = "netmask:";
char *sDnsIP = "dnsIP:";
char *sMacAddress = "mac:";
char *sBssid = "bssid:";

AdapterConfig ac;
AdapterConfigExtended acx;

void debug() {}

int main(void) {
    printf("test-fnlib %s\n", version);
    test_ac();
    return 0;
}

void test_ac(void) {
    bool is_success;
    is_success = fuji_get_adapter_config(&ac);
    printf("ac result: %s\n", is_success ? "true" : "false");
    is_success = fuji_get_adapter_config_extended(&acx);
    printf("acx result: %s\n", is_success ? "true" : "false");

    printf("ac\n");
    printf("%10s%s\n", sHostName, ac.hostname);
    printf("%10s%u.%u.%u.%u\n", sIP, ac.localIP[0], ac.localIP[1], ac.localIP[2], ac.localIP[3]);
    printf("%10s%u.%u.%u.%u\n", sGateway, ac.gateway[0], ac.gateway[1], ac.gateway[2], ac.gateway[3]);
    printf("%10s%u.%u.%u.%u\n", sNetmask, ac.netmask[0], ac.netmask[1], ac.netmask[2], ac.netmask[3]);
    printf("%10s%u.%u.%u.%u\n", sDnsIP, ac.dnsIP[0], ac.dnsIP[1], ac.dnsIP[2], ac.dnsIP[3]);
    printf("%10s%02x:%02x:%02x:%02x:%02x:%02x\n", sMacAddress, ac.macAddress[0], ac.macAddress[1], ac.macAddress[2], ac.macAddress[3], ac.macAddress[4], ac.macAddress[5]);
    printf("%10s%02x:%02x:%02x:%02x:%02x:%02x\n", sBssid, ac.bssid[0], ac.bssid[1], ac.bssid[2], ac.bssid[3], ac.bssid[4], ac.bssid[5]);
    printf("acx\n");
    printf("%10s%s\n", sHostName, acx.hostname);
    printf("%10s%s\n", sIP, acx.sLocalIP);
    printf("%10s%s\n", sGateway, acx.sGateway);
    printf("%10s%s\n", sNetmask, acx.sNetmask);
    printf("%10s%s\n", sDnsIP, acx.sDnsIP);
    printf("%10s%s\n", sMacAddress, acx.sMacAddress);
    printf("%10s%s\n", sBssid, acx.sBssid);
    cgetc();
    printf("\n");
}
