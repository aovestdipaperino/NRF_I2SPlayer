#pragma once
#include <Arduino.h>
#define FRAME_SAMPLES 1152
#define CHNLS 2
class Decoder {
public:
  virtual uint16_t fillBuffer(int16_t* buffer, uint16_t size) = 0;
  virtual void rewind();
};

class MP3BufferDecoder : public Decoder {
private:
  int totalSize;
  int bytesRemaining;
  const unsigned char* inputBuffer;
public:
  MP3BufferDecoder(const unsigned char* anInputBuffer, int aTotalSize);
  virtual uint16_t fillBuffer(int16_t* buffer, uint16_t size);
  virtual void rewind();
};

class I2SPlayer {
private:
  Decoder* _decoder;
  boolean fillBufferOne = true;
  int16_t bufferOne[FRAME_SAMPLES * CHNLS];
  int16_t bufferTwo[FRAME_SAMPLES * CHNLS];
  int16_t decodedBytes = 1;
  float volume = 1.0;
  void fillBuffer();

public:
  I2SPlayer(Decoder* aDecoder)
    : _decoder(aDecoder) {}
  void begin(uint8_t pinSDOUT, uint8_t pinSCK, uint8_t pinLRCK);
  void start();
  boolean loop();
  void setVolume(float value);
  Decoder* decoder() {
    return _decoder;
  }
};
