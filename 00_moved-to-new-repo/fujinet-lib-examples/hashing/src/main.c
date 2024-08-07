/**
 * @brief hashing test program
 *
 * @author Mark Fisher
 * @license gpl v. 3, see LICENSE for details.
 */

#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include "fujinet-fuji.h"

#include "main.h"

#ifdef __CBM__
#include <ascii_charmap.h>
#endif

char *input_text_a = "a";

// results from hashing string "a" with given algorithms
// char *hash_md5_text_a = "0cc175b9c0f1b6a831c399e269772661";
char *hash_sha1_text_a = "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8";
char *hash_sha256_text_a = "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb";
char *hash_sha512_text_a = "1f40fc92da241694750979ee6cf582f2d5d7d28e18335de05abc54d0560e0f5302860c652bf08d560252aa5e74210546f369fbbbce8c12cfc7957b2652fe9a75";

// results from hashing string "aa" with given algorithms
// char *hash_md5_text_aa = "4124bc0a9335c27f086f24ba207a4912";
char *hash_sha1_text_aa = "e0c9035898dd52fc65c41454cec9c4d2611bfb37";
char *hash_sha256_text_aa = "961b6dd3ede3cb8ecbaacbd68de040cd78eb2ed5889130cceb4c49268ea4d506";
char *hash_sha512_text_aa = "f6c5600ed1dbdcfdf829081f5417dccbbd2b9288e0b427e65c8cf67e274b69009cd142475e15304f599f429f260a661b5df4de26746459a3cef7f32006e5d1c1";

#ifdef __CBM__
#include <cbm_petscii_charmap.h>
#endif


char *ok = "OK";
char *bad = "BAD";

char buffer[129]; // max hash is 128 with hex, allow 1 byte for string terminator

int main(void)
{
	clrscr();

	test_simple_hash(SHA1, hash_sha1_text_a);
	test_simple_hash(SHA256, hash_sha256_text_a);
	test_simple_hash(SHA512, hash_sha512_text_a);

	test_multi_add(SHA1, hash_sha1_text_aa);
	test_multi_add(SHA256, hash_sha256_text_aa);
	test_multi_add(SHA512, hash_sha512_text_aa);

	cprintf("press a key");
	cgetc();
	return 0;
}

char *hash_to_name(hash_alg_t hash)
{
	switch (hash)
	{
	case SHA1:
		return "SHA1";
	case SHA256:
		return "SHA256";
	case SHA512:
		return "SHA512";
	}
}

// Illustrating the simple "fuji_hash_data" API method for calculating the hash of the given input bytes.
// We must allocate the memory for the output, in this simple case, we just use a large enough buffer, but could call "fuji_hash_length(algorithm, is_hex)" and then malloc a buffer.
void test_simple_hash(hash_alg_t hash, char *expected)
{
	bool is_success;
	bool matched;
	char *msg;
	char *hash_name = hash_to_name(hash);

	memset(buffer, 0, sizeof(buffer));
	is_success = fuji_hash_data(hash, (uint8_t *)input_text_a, strlen(input_text_a), true, (uint8_t *)&buffer[0]);
	if (!is_success)
	{
		printf("Error: %s\n", hash_name);
	}
	else
	{
		matched = (strcmp(buffer, expected) == 0);
		msg = matched ? ok : bad;
		if (matched)
		{
			printf("%3s: %s\nACT[%d]: %s\n\n", msg, hash_name, strlen(buffer), buffer);
		}
		else
		{
			printf("%3s: %s\nEXP[%d]: %s\nACT[%d]: %s\n\n", msg, hash_name, strlen(expected), expected, strlen(buffer), buffer);
		}
	}
	cgetc();
}

// It's much easier to use the above "fuji_hash_data" for a single string,
// but if you want to do multiple algorithms, or by building up more data
// before computing, here's the way:
void test_multi_add(hash_alg_t hash, char *expected)
{
	bool is_success;
	bool matched;
	char *hash_name = hash_to_name(hash);

	memset(buffer, 0, sizeof(buffer));

	// 1. CLEAR
	fuji_hash_clear();

	// 2. ADD THE DATA (as much as you need to), here the FujiNet will hash the string "aa"
	fuji_hash_add((uint8_t *)input_text_a, 1);
	fuji_hash_add((uint8_t *)input_text_a, 1);
	// 3. CALCULATE, and reset the FN buffer. We could also (if we set discard_data to false) call the calculate multiple times, which is
	is_success = fuji_hash_calculate(hash, true, true, (uint8_t *)&buffer[0]);

	if (!is_success)
	{
		printf("Error: %s\n", hash_name);
	}
	else
	{
		matched = (strcmp(buffer, expected) == 0);
		if (matched)
		{
			printf(" OK: %s\nACT[%d]: %s\n\n", hash_name, strlen(buffer), buffer);
		}
		else
		{
			printf("BAD: %s\nEXP[%d]: %s\nACT[%d]: %s\n\n", hash_name, strlen(expected), expected, strlen(buffer), buffer);
		}
	}
	cgetc();
}