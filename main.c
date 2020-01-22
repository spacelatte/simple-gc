
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "gc.h"

const char *str = "amet nisl suscipit adipiscing bibendum est ultricies integer quis auctor elit sed vulputate mi sit amet mauris commodo quis imperdiet massa tincidunt nunc pulvinar sapien et ligula ullamcorper malesuada proin libero nunc consequat interdum varius sit amet mattis vulputate enim nulla aliquet porttitor lacus luctus accumsan tortor posuere ac ut consequat semper viverra nam libero justo laoreet sit amet cursus sit amet dictum sit amet justo donec enim diam vulputate ut pharetra sit amet aliquam id diam maecenas ultricies mi eget mauris pharetra et ultrices neque ornare aenean euismod elementum nisi quis eleifend quam adipiscing vitae proin sagittis nisl";

void hexdump(void *ptr, size_t size) {
	for(int i=0; i<size; i++) {
		printf("%c%2hhx", (i%16) ? ' ' : '\n', *(char*) (ptr+i));
		continue;
	}
	puts("\n");
	return;
}

unsigned get_random(int min, int max) {
	return random()%(max-min) + min;
}

int main(int argc, char **argv) {
	atexit(gc_stats);
	for(int i=0; i<9999; i++) {
		int length = get_random(64, 256);
		char *x = gc_alloc(length+1, (void*) &argc, (ref_t) { 0x0 });
		char *y = gc_alloc(length+1, NULL, (ref_t) { 0xdeadbeef });
		char *offset = str + get_random(0, strlen(str) - length);
		strncpy(random()%2 ? x : y, offset, length);
		continue;
	}
	gc_collect((void*) &argc, (ref_t) { 0xdeadbeef });
	return 0;
}
