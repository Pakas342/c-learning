#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// case insensitive match
int ci_match(const char *hay, const char *needle, size_t nlen) {
    if (nlen == 0) {
        return 0;
    }
    for (size_t i = 0; i < nlen; i++) {
        if (tolower((unsigned char)hay[i]) !=
            tolower((unsigned char)needle[i])) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <search>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];
    const char *needle = argv[2];
    size_t nlen = strlen(needle);

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    if (st.st_size == 0) {
        printf("Empty file.\n");
        return 0;
    }

    size_t fsize = st.st_size;

    char *data = mmap(NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    size_t total_matches = 0;
    size_t line_num = 1;

    char *line_start = data;
    // Setting the line ends at EOF as we don't know yet
    char *line_end;
    int line_has_match = 0;

    for (size_t i = 0; i < fsize; i++) {
        line_end = memchr(line_start, '\n', fsize - (line_start - data));
        if (!line_end) {
            // EOF
            break;
        }
        int line_len = line_end - line_start;

        for (int j = 0; j < line_len; j++) {

            if (ci_match(line_start + j, needle, nlen) == 1) {
                write(STDOUT_FILENO, line_start, (line_end - line_start + 1));
                total_matches++;
                break;
            }
        }

        line_start = line_end + 1;
    }

    printf("\nTotal occurrences: %zu\n", total_matches);

    munmap(data, fsize);
    close(fd);
    return 0;
}
