
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

int main(int argc, char **argv) {
	atexit(gc_stats);
	for(int i=0; i<9999; i++) {
		gc_alloc(64 + random()%192, (void*) &argc, 0x0);
		strncpy(
			gc_alloc(64 + random()%192, NULL, 0xdeadbeef),
			str+random()%600,
			8+random()%50
		);
		continue;
	}
	gc_collect((void*) &argc, 0xdeadbeef);
	return 0;
}
