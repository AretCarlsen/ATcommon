
#pragma once

namespace DataStore {

template <typename Data_t>
class AtomicBuffer {
public:
  Data_t old_data;
  volatile Data_t new_data;
  volatile bool readPending;

  AtomicBuffer(const Data_t &initial_data = 0)
  : old_data(initial_data), new_data(initial_data), readPending(false) { }

  bool sinkData(const Data_t &data){
    if(readPending) return false;
    new_data = data;
    readPending = true;
    return true;
  }
  Data_t sourceData(){
    if(readPending){
      old_data = new_data;
      readPending = false;
    }
    return old_data;
  }
  Data_t sourceNewData(){
    return new_data;
  }
};

// End namespace: DataStore
}

