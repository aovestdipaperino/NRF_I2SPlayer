#pragma once
#include <Arduino.h>
#define FRAME_SAMPLES 1152
#define CHNLS 2
class Decoder {
public:
  virtual uint16_t fillBuffer(int16_t* buffer, uint16_t size, uint16_t* bufferSampleRate) = 0;
  virtual void rewind() = 0;
};

class MP3BufferDecoder : public Decoder {
private:
  int totalSize;
  int bytesRemaining;
  const unsigned char* inputBuffer;
public:
  MP3BufferDecoder(const unsigned char* anInputBuffer, int aTotalSize);
  virtual uint16_t fillBuffer(int16_t* buffer, uint16_t size, uint16_t* bufferSampleRate);
  virtual void rewind();
};

class I2SPlayer {
private:
  Decoder* _decoder;
  char bufferToFill = 0;
  int16_t buffers[2][FRAME_SAMPLES * CHNLS];
  int16_t decodedBytes = 1;
  float volume = 1.0;
  uint16_t fillBuffer();
  void setSampleRate(uint16_t rate);

public:
  I2SPlayer(Decoder* aDecoder)
    : _decoder(aDecoder) {}
  void begin(uint8_t pinSDOUT, uint8_t pinSCK, uint8_t pinLRCK);
  void play();
  void pause();
  void stop();
  boolean loop();
  void setVolume(float value);
  Decoder* decoder() {
    return _decoder;
  }
};
