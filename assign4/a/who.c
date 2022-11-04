#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define LINE 260
#define TYPE 296
#define TIME 300
#define RES_SIZE 32
#define BUFF_SIZE 629
#define HEADER_SIZE 1256

#define FILE_NAME "/var/run/utmp"
#define FORMAT "%b %d %H:%M"

time_t get_timestamp(char *buff)
{
    time_t timestamp = 0;

    ((unsigned char *)&timestamp)[0] = buff[0];
    ((unsigned char *)&timestamp)[1] = buff[1];
    ((unsigned char *)&timestamp)[2] = buff[2];
    ((unsigned char *)&timestamp)[3] = buff[3];
    return (timestamp);
}

int main(int argc, const char *argv[])
{
    char buff[BUFF_SIZE + 1] = {0};
    char res[RES_SIZE] = {0};
    FILE *file = NULL;
    time_t timestamp;
    struct tm lt;

    if ((file = fopen(FILE_NAME, "rb")) == NULL)
    {
        perror("file");
        exit(EXIT_FAILURE);
    }
    fseek(file, HEADER_SIZE, SEEK_SET);
    while (fgets(buff, BUFF_SIZE, file))
    {
        if (buff[TYPE] == 7)
        {
            timestamp = get_timestamp(&buff[TIME]);
            localtime_r(&timestamp, &lt);
            strftime(res, sizeof res, FORMAT, &lt);
            printf("%s\t %s  %s\n", buff, &buff[LINE], res);
        }
    }
    fclose(file);
    return 0;
}