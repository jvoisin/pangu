OUTPUT_DIR=./bin/

CC=gcc
CFLAGS=-m32


all: directory prog

directory:
	@mkdir -p $(OUTPUT_DIR)

prog: detection crash

crash: header_screwer 10123

10123:
	$(CC) $(CFLAGS) ./crash/10123.c -o $(OUTPUT_DIR)$@

header_screwer:
	$(CC) $(CFLAGS) ./crash/header_screwer.c -o $(OUTPUT_DIR)$@

detection:
	$(CC) $(CFLAGS) ./detect/*.c -o $(OUTPUT_DIR)$@

clean:
	rm -Rf $(OUTPUT_DIR)
