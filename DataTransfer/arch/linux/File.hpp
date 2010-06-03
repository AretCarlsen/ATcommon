#pragma once

#include "DataTransfer.hpp"

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>


int setNonBlocking(int fd)
{
    int flags;

    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}

// Takes a byte, returns a byte.
class FileDataSource : public DataTransfer::DataSource<uint8_t, DataTransfer::Status_t> {
  FILE* inFile;

public:

  FileDataSource(FILE* new_inFile)
  : inFile(new_inFile)
  {
    setNonBlocking(fileno(inFile));
  }

// Returns 0 on success, 1 on failure (EOF).
  uint8_t sourceData(uint8_t *byte){
    assert(inFile != NULL);

  // Attempt to read
    int8_t read_return = read(fileno(inFile), (void*) byte, 1);

// Busy?
    if(read_return == -1)
      return DataTransfer::Status__Busy;
// Bad?
    else if(read_return == 0)
      return DataTransfer::Status__Bad;
    else
      return DataTransfer::Status__Good;
  }

  bool eof(){
    return feof(inFile);
  }
};

// Takes a byte, returns a byte.
class FileDataSink : public DataTransfer::DataSink<uint8_t, DataTransfer::Status_t> {
public:
  FILE* outFile;

public:

  FileDataSink(FILE* new_outFile)
  : outFile(new_outFile)
  {
    setNonBlocking(fileno(outFile));
  }

// Returns 0 on success, 1 on failure.
  uint8_t sinkData(uint8_t byte){
    assert(outFile != NULL);
    if(write(fileno(outFile), (void*) &byte, 1) < 1){
      return (ferror(outFile) != 0)? DataTransfer::Status__Bad : DataTransfer::Status__Busy;
    }

    return DataTransfer::Status__Good;
  }
};

