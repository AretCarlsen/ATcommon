// Copyright (C) 2010, Aret N Carlsen (aretcarlsen@autonomoustools.com).
// DataTransfer classes are designed to allow for generic data streams
// to be freely redirected through a graph of descendants from these generic classes.
// (C++ file streams and the '<<' operator are often used for this purpose on x86.)
// Licensed under GPLv3 and later versions. See license.txt or <http://www.gnu.org/licenses/>.

#pragma once

namespace DataTransfer {
// Class to receive data in a sequential stream of buffers
  template <typename DataType_t, typename ReturnType_t>
  class DataSink {
  public:
    // Receive data
    virtual ReturnType_t sinkData(const DataType_t &data) = 0;
  };
}

namespace DataTransfer {
// Class to transmit data in a sequential stream of buffers
  template <typename DataType_t, typename ReturnType_t>
  class DataSource {
  public:
// Purely virtual. MUST be implemented.
    virtual ReturnType_t sourceData(DataType_t *data) = 0;
  };
}

namespace DataTransfer {
  typedef uint8_t Status_t;

// Status codes
  typedef uint8_t Status_t;
// Successful / Acceptable
  static const Status_t Status__Good = 0;
// Rejected / Aborted
  static const Status_t Status__Bad = 1;
// Busy / In Progress / Unavailable
  static const Status_t Status__Busy = 2;
// Completed / Finished
  static const Status_t Status__Complete = 3;

// Transfer a single piece of data from a Source to a Sink
  template <typename DataType_t, typename ReturnType_t>
  ReturnType_t dataTransfer_one(DataSource<DataType_t, ReturnType_t> *source, DataSink<DataType_t, ReturnType_t> *sink){
    DataType_t data;

    Status_t status = source->sourceData(&data);

    if(status != Status__Good)
      return status;

    return sink->sinkData(data);
  }

// Transfer a precise number of data items from a Source to a Sink.
  template <typename DataType_t, typename ReturnType_t, typename Count_t>
  ReturnType_t dataTransfer_multiple(DataSource<DataType_t, ReturnType_t> *source, DataSink<DataType_t, ReturnType_t> *sink, Count_t count){
    for(; count > 0; count--){
      Status_t status = dataTransfer(source, sink);
      if(status != Status__Good)
        return status;
    }

    return Status__Good;
  }

// Transfer all of the available items from a Source to a Sink.
  template <typename DataType_t, typename ReturnType_t>
  ReturnType_t dataTransfer_all(DataSource<DataType_t, ReturnType_t> *source, DataSink<DataType_t, ReturnType_t> *sink){
    DataType_t data;
    while(source->sourceData(&data) == 0){
      Status_t status = sink->sinkData(data);
      if(status != Status__Good)
        return status;
    }
    return Status__Good;
  }

// Transfer a precise number of data items from an array to a Sink.
  template <typename DataType_t, typename ReturnType_t, typename Count_t>
  ReturnType_t sinkData_multiple(DataSink<DataType_t, ReturnType_t> *dataSink, const DataType_t *data_ptr, Count_t count){
    for(; count > 0; count--){
      Status_t status = dataSink->sinkData(*data_ptr);
      if(status != Status__Good)
        return status;
      data_ptr++;
    }

    return Status__Good;
  }
}

