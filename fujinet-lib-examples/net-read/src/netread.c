#include "netread.h"
#include "fujinet-network.h"

// See README.md for information on running the REST API that this code needs
char abUrl[80]; // = "http://localhost:8080/alphabet/";
char cabUrl[80]; // = "http://localhost:8080/cab/";

uint8_t buffer[1024];

uint8_t err = 0;

char *version = "1.1.9";
char *url;

uint16_t nw_bw = 0;
uint8_t nw_conn = 0;
uint8_t nw_err = 0;

bool ask() {
  char sure = 0;
  printf("Run this test? ");
  sure = cgetc();
  if (sure == 'y' || sure == 'Y') {
    return true;
  } else {
    return false;
  }
}

int main(void) {
  bool sure = false;

  new_screen();
  setup();

  // new_screen();
  // printf("read google page\n");
  // sure = ask();
  // if (sure) {
  //   // show all of the page by using 0 as the count
  //   do_long("n:https://www.google.com/");
  // }

  // simple read of 27 bytes
  new_screen();
  printf("Normal Read 27 bytes\n");
  do_alpha(27);

  // ask and take exactly 27 (chunks are 10 bytes every 100ms)
  new_screen();
  printf("Chunked 27/27 bytes\n"); 
  do_chunked(27, 27);

  // ask for less than available, should be 10 in BW at end
  new_screen();
  printf("Chunked 27/17 bytes\n"); 
  do_chunked(27, 17);

  // ask for more than available
  new_screen();
  printf("Chunked 27/500 bytes\n"); 
  do_chunked(27, 500);

  // ask for lots, only read a few, BW will be 13 at the end (not sure why it's not 3)
  new_screen();
  printf("Chunked 550/27 bytes\n"); 
  do_chunked(550, 27);

  // over 512
  new_screen();
  printf("Normal Read 521 bytes\n");
  do_alpha(521);

  // over 512 chunked
  new_screen();
  printf("Chunked 521/521 bytes\n");
  do_chunked(521, 521);

  // multiple reads in application
  new_screen();
  // 3 lots of A-A, B-B, C-C, then an extra D at the end.
  printf("multi 82/27\n");
  do_multi(82, 27);

  new_screen();
  printf("read 500K\n");
  sure = ask();
  if (sure) {
    // this takes about 2 minutes to fully display only printing 1st byte of each 1024 block. Takes 20 if you display every byte.
    sprintf(abUrl, "n:http://%s:%s/alphabet/500000", REST_SERVER_ADDRESS, REST_SERVER_PORT);
    do_long_first_only(abUrl);
  }

  printf("\n");
  return 0;
}

bool setup() {
  uint8_t init_r = 0;
  printf("running network_init\n");
  init_r = network_init();
  printf("init: %d, derr: %d\n", init_r, fn_device_error);
#ifdef __APPLE2__
  printf("nw: %d\n", sp_network);
#endif

  if (init_r != 0) {
    err = init_r;
    handle_err("network_init failed");
  }
  printf("Press a key to continue.");
  cgetc();
  return true;
}

void do_read(int num) {
  int count = 0;
  count = network_read(url, buffer, num);
  if (count < 0) {
    err = -count;
    handle_err("read");
  }

  printf("Fetched %" PRIu16 " bytes, count: %" PRIu16 "\n", fn_bytes_read, count);
  hex_dump(buffer, fn_bytes_read);

  // check network status
  network_status(url, &nw_bw, &nw_conn, &nw_err);
  printf("bw: %u, c: %d, err: %d\n", nw_bw, nw_conn, nw_err);
}

void do_open() {
  printf("Url: %s\n", url);
  err = network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  handle_err("open");
}

void do_close() {
  network_close(url);
}

void do_common(int num) {
  do_open();
  do_read(num);
}

void do_alpha(int num) {
  clear_buffer();
  url = createAbUrl(num);
  do_common(num);
  do_close();
  cgetc();
}

void do_chunked(int total, int size) {
  clear_buffer();
  url = createCabUrl(total);
  do_common(size);
  do_close();
  cgetc();
}

void do_multi(int total, int size) {
  int i = 0;
  int num_chunks = total / size;
  int remainder = total - (size * num_chunks);

  clear_buffer();
  url = createAbUrl(total);
  do_open();

  for (i = 0; i < num_chunks; i++) {
    do_read(size);
  }
  if (remainder > 0) {
    do_read(remainder);
  }

  do_close();
  cgetc();
}

void do_long(char *u) {
  int i = 0;

  url = u;
  do_open();
  do {
    network_read(url, buffer, sizeof(buffer));
    for (i = 0; i < fn_bytes_read; ++i) {
      putchar(buffer[i]);
    }
  } while (fn_network_error != 136);
  do_close();
  cgetc();
}

void do_long_first_only(char *u) {
  int i = 0;

  url = u;
  do_open();
  do {
    network_read(url, buffer, sizeof(buffer));
    // only display first byte of block to save time
    putchar(buffer[0]);
  } while (fn_network_error != 136);
  do_close();
  cgetc();
}

void clear_buffer() {
  memset(buffer, 0, 1024);
}

void new_screen() {
  clrscr();
  gotoxy(0, 0);
  printf("netread %s\n", version);
}

// Create Network URL to REST API we are using to test network library functionality
char * createAbUrl(int num) {
  sprintf(abUrl, "n:http://%s:%s/alphabet/%d", REST_SERVER_ADDRESS, REST_SERVER_PORT, num);
  return (char *)abUrl;
}

char * createCabUrl(int num) {
  sprintf(cabUrl, "n:http://%s:%s/cab/%d", REST_SERVER_ADDRESS, REST_SERVER_PORT, num);
  return (char *)cabUrl;
}

void handle_err(char *reason) {
    if (err) {
        printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
        cgetc();
        exit(1);
    }
}

void hex_dump(uint8_t* data, int size) {
    int i = 0;
    int j = 0;
    int p = 0;
    int start = 0;
    int padding = 0;
    char c;

    for (i = 0; i < size; i++) {
        if (i % 8 == 0) {
            printf("%04x ", i);
        }
        printf("%02x ", (unsigned char)data[i]);

        if ((i + 1) % 8 == 0 || i == size - 1) {
            padding = ((i + 1) % 8) ? (8 - (i + 1) % 8) : 0;
            for (p = 0; p < padding; p++) {
                printf("   "); // for alignment
            }
            printf("| ");
            start = i - (i % 8);
            for (j = start; j <= i; j++) {
                c = data[j];
                if (isprint((unsigned char)c)) {
                    printf("%c", c);
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
    }
}