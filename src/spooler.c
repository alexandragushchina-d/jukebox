#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>

#include "jukebox.h"
#include "requester.h"
#include "songs.h"

void printHelp(char* progName);

bool hasSwitch(char* argv[], int argc, char* parameter) {
  for (int i = 0; i < argc; ++i) {
    if(strcmp(argv[i], parameter) == 0) {
      return true;
    }
  }
  return false;
}

bool getSwitchStrValue(char* argv[], int argc, char* parameter, char** value) {
  for (int i = 0; i < (argc - 1); ++i) {
    if(strcmp(argv[i], parameter) == 0) {
      *value = argv[++i];
      return true;
    }
  }
  return false;
}

bool getSwitchIntValue(char* argv[], int argc, char* parameter, int* value) {
  for (int i = 0; i < (argc - 1); ++i) {
    if(strcmp(argv[i], parameter) == 0) {
      *value = atoi(argv[++i]);
      return true;
    }
  }
  return false;
}

int main(int argc, char* argv[]) {
  if (argc == 2 && hasSwitch(argv, argc, "-help")) {
    printHelp(argv[0]);
    return EXIT_SUCCESS;
  }

  if (argc != 6) {
    printHelp(argv[0]);
    return EXIT_FAILURE;
  }

  if (hasSwitch(argv, argc, "-request")) {
    char* pipe_name = 0;
    if (!getSwitchStrValue(argv, argc, "-path", &pipe_name)) {
      printHelp(argv[0]);
      return EXIT_FAILURE;
    }

    int song_id = -1;
    if (!getSwitchIntValue(argv, argc, "-id", &song_id)) {
      printHelp(argv[0]);
      return EXIT_FAILURE;
    }

    if (song_id < 0 || song_id >= NUM_SONGS) {
      printf("Invalid song ID %d. Has to be from range 0 to %d\n",
             song_id, NUM_SONGS - 1);
      return EXIT_FAILURE;
    }

    requestSong(pipe_name, song_id);
    return EXIT_SUCCESS;
  } else if (hasSwitch(argv, argc, "-jukebox")) {
   	char* pipe_name = 0;
   	if (!getSwitchStrValue(argv, argc, "-path", &pipe_name)) {
    	printHelp(argv[0]);
    	return EXIT_FAILURE;
   	}

   	int delay = -1;
   	if (!getSwitchIntValue(argv, argc, "-delay", &delay)) {
   	  printHelp(argv[0]);
   	  return EXIT_FAILURE;
  	}


    runJukebox(pipe_name, delay);
    return EXIT_SUCCESS;
 	}

 	return EXIT_FAILURE;
}

void printHelp(char* progName) {
  printf("================Help================\n");
  printf("%s -jukebox -path <char*> -delay <int> : Starts a spooler jukebox process with the path for the named pipe and a delay in seconds between lines.\n", progName);
  printf("%s -request -path <char*> -id <int> : Request the given song id at the given named pipe path.\n", progName);
  printf("%s -help : Prints this help.\n", progName);
  printf("====================================\n");
}
