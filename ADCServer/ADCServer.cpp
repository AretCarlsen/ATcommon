#include "ADCServer.hpp"

// Prepare a packet
Status::Status_t ADCServer::process(){
  DataStore::AtomicBuffer<Measurement_t> *inputBuffer = inputBuffers->front();

//    DEBUGprint("ADC server: Preparing packet.\n");

/*
// Estimate the outgoing packet data size.
  // Header + destAddr + channel count + values + checksum
  uint8_t dataSize = 1 + 1 + 2*inputBuffers->get_size() + MAP::ChecksumLength;
*/

  MAP::MAPPacket *packet;
// Attempt to prepare the outgoing packet. Return if failed, to try again later.
  // Capacity needs to be large enough s.t. headers can be sunk without expansion.
  if(! MAP::allocateNewPacket(&packet, InitialPacketCapacity, memoryPool)) return Status::Status__Good;

// Header byte.
  packet->sinkData(MAP::DestAddressPresent_Mask | destAddressType);
// Destination address
  packet->sinkData(destAddress);

// Channel count (c78)
  // Cannot handle over 127
  packet->sinkData(inputBuffers->get_size());

// Sink individual measurements
  for(inputBuffer = inputBuffers->front(); inputBuffer < inputBuffers->back(); inputBuffer++){
  // Sink measurement. Abort if run out of space.
    if(! packet->sinkC78(inputBuffer->sourceData(), PacketCapacityIncrement, PacketCapacityMax)){
      MAP::dereferencePacket(packet);
      return Status::Status__Good;
    }
  }

// Send the packet on its way.
  MAP::referencePacket(packet);
  packetSink->sinkPacket(packet);
  MAP::dereferencePacket(packet);

// Success!
  return Status::Status__Good;
}


