/**
 * Utility to access bus using dtree information (example of use).
 * Jan Viktorin <xvikto03@stud.fit.vutbr.cz>
 */

#include "dtree.h"
#include "dtree_util.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <fcntl.h>

static int verbosity = 0;

void verbosity_printf(int level, const char *fmt, ...)
{
	if(level > verbosity)
		return;

	va_list arg;
	va_start(arg, fmt);
	vfprintf(stderr, fmt, arg);
	fprintf(stderr, "\n");
	va_end(arg);
}

//
// Bus access
//

void *bus_devmem_access(uint32_t base, uint32_t mlen, int *fd)
{
	*fd = open("/dev/mem", O_RDWR);
	if(*fd == -1) {
		perror("open(/dev/mem)");
		return NULL;
	}

	void *m = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, *fd, base);
	if(m == NULL) {
		perror("mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, base)");
		close(*fd);
		return NULL;
	}

	return m;
}

void bus_devmem_forget(void *m, int fd)
{
	munmap(m, getpagesize());
	close(fd);
}

static int memfd;

void *bus_access(uint32_t base, uint32_t mlen)
{
	return bus_devmem_access(base, mlen, &memfd);
}

void bus_forget(void *mapping)
{
	bus_devmem_forget(mapping, memfd);
}

void bus_write(uint32_t base, uint32_t off, uint32_t value, int len)
{
	void *m = bus_access(base, len);
	if(m == NULL)
		return;

	uint8_t *cdata = (uint8_t *) m;
	uint8_t *wdata  = cdata + off;

	switch(len) {
	case 1:
		verbosity_printf(2, "Writing at 0x%08X value 0x%02X (1)", base + off, value & 0x000000FF);
		*(uint8_t *)  wdata = (uint8_t)  (value & 0x000000FF);
		break;
		
	case 2:
		verbosity_printf(2, "Writing at 0x%08X value 0x%04X (2)", base + off, value & 0x0000FFFF);
		*(uint16_t *) wdata = (uint16_t) (value & 0x0000FFFF);
		break;

	case 4:
		verbosity_printf(2, "Writing at 0x%08X value 0x%08X (4)", base + off, value & 0xFFFFFFFF);
		*(uint32_t *) wdata = (uint32_t) (value & 0xFFFFFFFF);
		break;

	default:
		verbosity_printf(1, "BUG? Invalid data length passed to bus_write(): %d", len);
		abort();
	}

	bus_forget(m);
}

uint32_t bus_read(uint32_t base, uint32_t off, int len)
{
	void *m = bus_access(base, 4);
	if(m == NULL)
		return 0xFFFFFFFE;

	verbosity_printf(2, "Reading from address '0x%08X'", base + off);

	uint8_t  *cdata = (uint8_t  *) m;
	uint32_t *rdata = (uint32_t *) cdata + off;
	uint32_t value  = *rdata;

	verbosity_printf(2, "Raw value: 0x%08X", value);

	if(value == 0xFFFFFFFE)
		verbosity_printf(1, "WARN: Possible error when accessing the bus");

	switch(len) {
	case 1:
		value &= 0x000000FF;
		verbosity_printf(2, "Masking out %d bytes: 0x%08X", 4 - len, value);
		break;

	case 2:
		value &= 0x0000FFFF;
		verbosity_printf(2, "Masking out %d bytes: 0x%08X", 4 - len, value);
		break;

	case 4:
		value &= 0xFFFFFFFF;
		verbosity_printf(2, "Masking out %d bytes: 0x%08X", 4 - len, value);
		break;

	default:
		verbosity_printf(1, "BUG? Invalid data length passed to bus_read(): %d", len);
		abort();
	}

	bus_forget(m);
	return value;
}

void bus_list(void)
{
	struct dtree_dev_t *dev;
	while((dev = dtree_next()) != NULL) {
		printf("%s", dtree_dev_name(dev));
		printf(" at 0x%X", dtree_dev_base(dev));

		const char **compat = dtree_dev_compat(dev);
		if(compat[0] != NULL)
			printf(" [");

		for(size_t i = 0; compat[i] != NULL; ++i)
			printf("%s%s", compat[i], compat[i + 1] == NULL? "]" : ", ");

		printf("\n");
	}
}


//
// Actions
//

int perform_list(void)
{
	verbosity_printf(1, "Action: list");
	bus_list();
	return 0;
}

int perform_read(const char *dev, uint32_t addr, int len)
{
	struct dtree_dev_t *d = dtree_byname(dev);
	if(d == NULL) {
		fprintf(stderr, "No device '%s' found\n", dev);
		return 1;
	}

	verbosity_printf(1, "Action: read, device: '%s', offset: '0x%08X', len: '%d'", dev, addr, len);

	uint32_t value = bus_read(dtree_dev_base(d), addr, len);
	printf("0x%08X\n", value);

	return 0;
}

int perform_write(const char *dev, uint32_t addr, uint32_t len, uint32_t value)
{
	struct dtree_dev_t *d = dtree_byname(dev);
	if(d == NULL) {
		fprintf(stderr, "No device '%s' found\n", dev);
		return 1;
	}
	
	verbosity_printf(1, "Action: write, device: '%s', offset: '0x%08X', data: '0x%08X', len: '%d'", dev, addr, value, len);

	bus_write(dtree_dev_base(d), addr, value, len);
	return 0;
}


//
// Main
//

uint32_t parse_addr(const char *s)
{
	return parse_hex(s, strlen(s));
}

uint32_t parse_value(const char *s)
{
	return parse_hex(s, strlen(s));
}

#define GETOPT_STR "hlr:w:t:a:d:124vV"
#define DTREE_PATH "/proc/device-tree"

int print_help(const char *prog)
{
	fprintf(stderr, "Usage: %s [ -V | -h | -l | -r <dev> | -w <dev> ] [ -t <path> ] [ -a <addr> ] [ -d <data> ] [ -1 | -2 | -4 ]\n", prog);
	fprintf(stderr, "Examples:\n");
	fprintf(stderr, "* List all devices in default device-tree: %s\n", DTREE_PATH);
	fprintf(stderr, "  $ %s -l\n", prog);
	fprintf(stderr, "* List all devices in device-tree: test/device-tree\n");
	fprintf(stderr, "  $ %s -l -t test/device-tree\n", prog);
	fprintf(stderr, "* Read a word (4) from peripheral named 'plb' from offset 0x00\n");
	fprintf(stderr, "  $ %s -r plb -a 0x00\n", prog);
	fprintf(stderr, "* Write a word 0x000000FF to peripheral named 'plb' to offset 0x00\n");
	fprintf(stderr, "  $ %s -w plb -a 0x00 -d 0xFF\n", prog);
	fprintf(stderr, "* Read a byte from peripheral named 'timer' from offset 0x04\n");
	fprintf(stderr, "  $ %s -r timer -a 0x04 -1\n", prog);
	fprintf(stderr, "* Write 0x00FF to peripheral named 'timer' to offset 0x08\n");
	fprintf(stderr, "  $ %s -w timer -a 0x08 -d 0xFF -2\n", prog);
	return 0;
}

int print_opterr(int opt)
{
	fprintf(stderr, "Invalid option: %c\n", opt);
	return 1;
}

int print_version(const char *prog)
{
	printf("%s (%s build on %s at %s)\n", prog, __FILE__, __DATE__, __TIME__);
	return 0;
}

int main(int argc, char **argv)
{
	// action to be performed
	int act = '\0';

	// address offset to the device
	uint32_t addr  = 0;
	int addr_valid = 0;

	// value to be written
	uint32_t value = 0;
	int value_valid = 0;

	// number of bytes to be written or read
	int len = 4;

	// used device-tree to get address
	const char *dtree = DTREE_PATH;

	// name of the device to access
	const char *dev   = NULL;

	int opt;
	opterr = 0;

	while((opt = getopt(argc, argv, GETOPT_STR)) != -1) {
		switch(opt) {
		case 'h':
			return print_help(argv[0]);
		
		case 'V':
			return print_version(argv[0]);

		case 'v':
			verbosity += 1;
			break;

		case 'l':
			act = opt;
			break;

		case 'r':
		case 'w':
			dev = optarg;
			act = opt;
			break;

		case 't':
			dtree = optarg;
			break;

		case 'a':
			addr = parse_addr(optarg);
			addr_valid = 1;
			break;

		case 'd':
			value = parse_value(optarg);
			value_valid = 1;
			break;

		case '1':
		case '2':
		case '3':
		case '4':
			len = opt - '0';
			break;

		case '?':
		default:
			return print_opterr(optopt);
		}
	}

	verbosity_printf(1, "Attempt to open device-tree '%s'", dtree);
	if(dtree_open(dtree) != 0) {
		fprintf(stderr, "dtree_open(%s): %s\n", dtree, dtree_errstr());
		return 1;
	}

	int err = 0;

	switch(act) {
	case 'l':
		err = perform_list();
		goto exit;

	case 'r':
		assert(dev != NULL);
		if(addr_valid) {
			err = perform_read(dev, addr, len);
			goto exit;
		}

		break;

	case 'w':
		assert(dev != NULL);
		if(addr_valid && value_valid) {
			err = perform_write(dev, addr, len, value);
			goto exit;
		}

		break;

	case '\0':
		fprintf(stderr, "No action has been specified\n");
		err = 1;
		goto exit;

	default:
		fprintf(stderr, "Unknown action to be performed: %c\n", act);
		err = 1;
		goto exit;
	}

	if(!addr_valid) {
		fprintf(stderr, "Address option (-a) is missing\n");
		err = 1;
		goto exit;
	}
	if(!value_valid) {
		fprintf(stderr, "Data option (-d) is missing\n");
		err = 1;
		goto exit;
	}

	fprintf(stderr, "Unknown option error\n");
	err = 1;

exit:
	dtree_close();
	return err;
}

