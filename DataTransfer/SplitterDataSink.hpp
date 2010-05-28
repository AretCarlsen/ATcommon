// SplitterDataSink class

#pragma once

#include "DataSink.hpp"
#include <vector>

template <typename DataType_t>
class SplitterDataSink : public DataSink<DataType_t> {
  std::vector< DataSink<DataType_t>* > dataSinks;

public:

  void addSink(DataSink<DataType_t> *dataSink){
    dataSinks.push_back(dataSink);
  }

  bool sinkData(DataType_t data){
    for(typename std::vector< DataSink<DataType_t>* >::iterator dataSinkIter = dataSinks.begin();
      dataSinkIter < dataSinks.end();
      dataSinkIter++
    ){ 
      (*dataSinkIter)->sinkData(data);
    }
  }
};

