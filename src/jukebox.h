#pragma once

#define NAMED_PIPE_ACCESS_RIGHT 0666

void runJukebox(char* pipeFilePath, int delay);

void startPipeReader(char* pipeFilePath);
void startStdinReader();
