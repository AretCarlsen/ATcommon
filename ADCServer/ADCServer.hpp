
#include <Upacket/MAP/MAP.hpp>
#include <ATcommon/DataStore/AtomicBuffer.hpp>

class ADCServer : public Process {
// Packet memory pool
  MemoryPool *memoryPool;
// Outgoing packet sink
  MAP::MAPPacketSink *packetSink;

// Measurement type
  typedef uint16_t Measurement_t;
// Input measurement ringbuffers
  DataStore::ArrayBuffer< DataStore::AtomicBuffer<Measurement_t>, uint8_t> *inputBuffers;

// Destination address
  MAP::Data_t destAddressType;
  MAP::Data_t destAddress;

// Initial outgoing packet capacity
  static const uint8_t InitialPacketCapacity = 15;
  static const uint8_t PacketCapacityIncrement = 10;
  static const uint8_t PacketCapacityMax = 40;

public:

  ADCServer(DataStore::ArrayBuffer<DataStore::AtomicBuffer<Measurement_t>, uint8_t> *new_inputBuffers,
            MAP::MAPPacketSink *new_packetSink, MemoryPool *new_memoryPool,
            const MAP::Data_t &new_destAddressType, const MAP::Data_t &new_destAddress)
  : memoryPool(new_memoryPool), packetSink(new_packetSink),
    inputBuffers(new_inputBuffers),
    destAddressType(new_destAddressType), destAddress(new_destAddress)
  {
    assert(new_inputBuffers != NULL);
    assert(new_memoryPool != NULL);
    assert(new_packetSink != NULL);
  // Over 127 unlikely, and complicates packet.
    assert(inputBuffers->get_size() <= 127);
  }

  Status::Status_t process();
};

