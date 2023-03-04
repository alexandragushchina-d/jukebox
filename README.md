# spooling on a jukebox example

Spooling is mostly used in the operating system to synchronise access to external resources such as printers or data storage. The incoming requests (from different processes) are stored in a buffer, and a single process then processes them sequentially.

Several processes should be able to express song requests, which are then processed by spooling. The song with the most "votes" should always be played first. If none of the songs has a "vote", the system should wait until a song request (vote) comes.

In Requester mode, it should be possible to forward a song request to the jukebox via a named pipe. Jukebox should output the desired song texts to the console.

Since reading a named pipe is normally blocking, we want to start a second thread whose task is to permanently accept song requests via the pipe.
