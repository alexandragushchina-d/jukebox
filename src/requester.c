#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "requester.h"

void requestSong(char* pipeFilePath, int songId) {
  printf("Requesting song %d...\n", songId);
  int fd;
  char buffer[32];
  sprintf(buffer, "%d\n", songId);
  mkfifo(pipeFilePath, 0666);
  fd = open(pipeFilePath, O_WRONLY);
  write(fd, buffer, strlen(buffer));
  close(fd);
  printf("Song %d requested.\n", songId);
}
