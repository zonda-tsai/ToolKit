#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int toBASE64(const char* src, const char* dest){
	if(src == NULL || src[0] == 0) return 0;
	static const unsigned char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	FILE *f = fopen(src, "rb");
	if(f == NULL) return 0;
	FILE *output = stdout;
	if(dest != NULL && !(output = fopen(dest, "w"))) return 0;
	unsigned int byte;
	unsigned char input_buf[4] = {0}, output_buf[5] = {0};
	while((byte = fread(input_buf, 1, 3, f)) == 3){
		output_buf[0] = input_buf[0] >> 2;
		output_buf[1] = ((input_buf[0] & 0x3) << 4) | (input_buf[1] >> 4);
		output_buf[2] = ((input_buf[1] & 0xF) << 2) | (input_buf[2] >> 6);
		output_buf[3] = input_buf[2] & 0x3F;
		fprintf(output, "%c%c%c%c", base[(int)output_buf[0]], base[(int)output_buf[1]], base[(int)output_buf[2]], base[(int)output_buf[3]]);
		memset(input_buf, 0, 4);
	}
	if(byte == 1){
		output_buf[0] = input_buf[0] >> 2;
		output_buf[1] = ((input_buf[0] & 0x3) << 4) | (input_buf[1] >> 4);
		fprintf(output, "%c%c==", base[(int)output_buf[0]], base[(int)output_buf[1]]);
	}
	else if(byte == 2){
		output_buf[0] = input_buf[0] >> 2;
		output_buf[1] = ((input_buf[0] & 0x3) << 4) | (input_buf[1] >> 4);
		output_buf[2] = ((input_buf[1] & 0xF) << 2) | (input_buf[2] >> 6);
		fprintf(output, "%c%c%c=", base[(int)output_buf[0]], base[(int)output_buf[1]], base[(int)output_buf[2]]);
	}
	fclose(f);
	if(dest != NULL) fclose(output);
	return 1;
}

int main(int argc, char* argv[]){
	if(argc != 2 && argc != 3) return 1;
	int result = toBASE64(argv[1], (argc == 3) ? argv[2] : NULL);
	if(!result) printf("\x1b[31mFailed...\x1b[0m\n");
	else printf("\x1b[32mSucceed...\x1b[0m\n");
	return 0;
}
