#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
void generate_test_file(int count)
{
	FILE *f;
	int i;

	f = fopen("testfile", "w+");
	for (i=0;i<count;i++) {
		fwrite(&i, 1, sizeof(i), f);
	}
	fclose(f);
}
void test_test_file(int count, bool expect_zeros)
{
	FILE *f;
	int i, j;

	f = fopen("testfile", "r");
	for (i=0;i<count;i++) {
		fread(&j, 1, sizeof(j), f);
		assert(j == (expect_zeros ? 0 : i));
	}
	fclose(f);
}
int filesize(const char* file)
{
    struct stat buf;
    stat(file, &buf);
    return buf.st_size;
}
void test_mmap_directly()
{
	int fd;
	void *area_short, *area_long;

	generate_test_file(1024);
	fd = open("testfile", O_RDONLY);
	assert(fd>=0);
    printf("11111111111111\n");
	area_short = mmap(0, 4096, PROT_READ, MAP_PRIVATE, fd, 0);
	assert(area_short != MAP_FAILED);
	
    printf("11111111222222222222222\n");
	close(fd);
	generate_test_file(1024*1024);
	fd = open("testfile", O_RDONLY);
	assert(fd>=0);

    printf("1111133333333333111\n");
	area_long = mmap(area_short, 4096*1024, PROT_READ, MAP_PRIVATE | MAP_FIXED, fd, 0);
	assert(area_long != MAP_FAILED);
	assert(area_short == area_long);
}

int main(int argc, char ** argv)
{
	test_mmap_directly();
/* Crashes on shutdown for libc 2.5 and kernel 2.6.18 (CentOS 5) */
}
