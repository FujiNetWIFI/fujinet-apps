#include "netread.h"
#include "fujinet-network.h"

// See README.md for information on running the REST API that this code needs
char abUrl[80]; // = "http://localhost:8080/alphabet/";
char cabUrl[80]; // = "http://localhost:8080/cab/";

uint8_t buffer[1024];

uint8_t err = 0;

char *version = "1.0.3";
char *url;

uint16_t nw_bw = 0;
uint8_t nw_conn = 0;
uint8_t nw_err = 0;

int main(void) {

  new_screen();
  printf("Normal Read 27 bytes\n");
  do_alpha(27, 0x01);

  // on real FN, this doesn't work as network read isn't looking at bw yet
  new_screen();
  printf("Chunked 27 bytes\n"); 
  do_chunked(27, 0x02);

  // new_screen();
  // printf("Normal Read 521 bytes\n");
  // do_alpha(521, 0x03);

  // new_screen();
  // printf("Chunked 521 bytes\n");
  // do_chunked(521, 0x03);

  new_screen();
  // 3 lots of A-A, B-B, C-C, then an extra D at the end.
  printf("multi 82/27\n");
  do_multi(82, 27, 0x03);

  return 0;
}

void do_read(int num) {
  err = network_read(url, buffer, num);
  handle_err("read");

  printf("Fetched %" PRIu16 " bytes:\n", fn_bytes_read);
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
  cgetc();
}

void do_alpha(int num, uint8_t clear_char) {
  clear_buffer(clear_char);
  url = createAbUrl(num);
  do_common(num);
  do_close();
}

void do_chunked(int num, uint8_t clear_char) {
  clear_buffer(clear_char);
  url = createCabUrl(num);
  do_common(num);
  do_close();
}

void do_multi(int total, int size, uint8_t clear_char) {
  int i = 0;
  int num_chunks = total / size;
  int remainder = total - (size * num_chunks);
  printf("chunks: %d, rem: %d\n", num_chunks, remainder);

  clear_buffer(clear_char);
  url = createAbUrl(total);
  do_open();

  for (i = 0; i < num_chunks; i++) {
    do_read(size);
  }
  if (remainder > 0) {
    do_read(remainder);
  }

  do_close();
}

void clear_buffer(uint8_t v) {
  memset(buffer, v, 1024);
}

void new_screen() {
  clrscr();
  gotoxy(0, 0);
  printf("netread %s\n", version);
}

// Create Network URL to REST API we are using to test network library functionality
char * createAbUrl(int num) {
  sprintf(abUrl, "N:http://%s:%s/alphabet/%d", REST_SERVER_ADDRESS, REST_SERVER_PORT, num);
  return (char *)abUrl;
}

char * createCabUrl(int num) {
  sprintf(cabUrl, "N:http://%s:%s/cab/%d", REST_SERVER_ADDRESS, REST_SERVER_PORT, num);
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