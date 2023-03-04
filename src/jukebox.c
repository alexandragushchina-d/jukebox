#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>



#include "jukebox.h"
#include "songs.h"

int shouldRun = 0;

pthread_t stdin_thread_id = 0;
pthread_t pipe_thread_id = 0;

int song_survey[NUM_SONGS];

void playPrioritySong(int delay)
{
  // find maximum votes
  int maxVotes = 0;
  int songToPlay = -1;
  for(int i=0; i<NUM_SONGS; i++)
  {
    if(song_survey[i]>maxVotes)
    {
      maxVotes = song_survey[i];
      songToPlay = i;
    }
  }
  if(maxVotes == 0) return;

  // create new process
  pid_t pid = fork();
  if(pid == 0)
  {
    printf("Playing: %s\n", SONGS[songToPlay].title);
    for(size_t i=0; i<SONGS[songToPlay].lines; i++)
    {
      printf("%s", SONGS[songToPlay].lyrics[i]);
      sleep(delay);
    }
    exit(0);
  }
  else
  {
    song_survey[songToPlay] = 0;
    wait(0);
    return;
  }
}

void runJukebox(char* pipeFilePath, int delay) {
  printf("Welcome to the GBS Jukebox!\n");
  shouldRun = 1;
  memset(song_survey, 0, sizeof(song_survey));
  startStdinReader();
  startPipeReader(pipeFilePath);
  while (shouldRun) {
    playPrioritySong(delay);
    sleep(1);
  }
  pthread_join(stdin_thread_id, NULL);
  int pipe = open((char*)pipeFilePath, O_WRONLY);
  write(pipe, "\n", 1);
  pthread_join(pipe_thread_id, NULL);
  printf("Goodbye\n");
}

void* readPipe(void* pipeFilePath)
{
  // create pipe
  mkfifo((char*)pipeFilePath, 0666);

  int pipe;
  int songID = -1;
  char buffer[256];
  while(shouldRun)
  {
    pipe = open((char*)pipeFilePath, O_RDONLY);
    memset(buffer, 0, sizeof(buffer));
    char symbol = 0;
    int index = 0;
    while (read(pipe, &symbol, 1) == 1 && symbol != '\n') {
      buffer[index++] = symbol;
    }
    close(pipe);

    if (strlen(buffer) == 0) {
      //sleep(1);
      continue;
    }

    songID = strtol(buffer, NULL, 10);
    if(songID < 0 || songID >= NUM_SONGS)
    {
      printf("Invalid song id: %d\n", songID);
    }
    else
    {
      ++song_survey[songID];
      printf("Vote for song id %d received.\n", songID);
    }
  }
  pthread_exit(0);
}

// pipe-Thread
void startPipeReader(char* pipeFilePath) {
  //create Thread
  pthread_create(&pipe_thread_id, NULL, readPipe, pipeFilePath);
}

// stdin-Thread
void* stdinThread() {
  char ch;
  for (;;) {
    scanf("%c", &ch);
    if (ch == 'q')
    {
      break;
    }
  }
  shouldRun = 0;
  pthread_exit(0);
}


void startStdinReader() {
  pthread_create(&stdin_thread_id, NULL, stdinThread, NULL);
}
