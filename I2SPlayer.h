#pragma once
#include <Arduino.h>
#define FRAME_SAMPLES 1152
#define CHNLS            2
class Decoder {
  public:
    virtual uint16_t fillBuffer(int16_t* buffer, uint16_t size) = 0;
};

class MP3BufferDecoder : public Decoder {
  private:
    int bytesRemaining;
    const unsigned char * inputBuffer;
  public:
    void init(const unsigned char* anInputBuffer, int aTotalSize);
    virtual uint16_t fillBuffer(int16_t* buffer, uint16_t size);
};

class I2SPlayer {
  private:
    Decoder* decoder;
    boolean fillBufferOne = true;
    int16_t bufferOne[FRAME_SAMPLES * CHNLS];
    int16_t bufferTwo[FRAME_SAMPLES * CHNLS];
    int16_t decodedBytes = 1;
    float volume = 1.0;
    void fillBuffer();

  public:
    void begin(uint8_t pinSDOUT, uint8_t pinSCK, uint8_t pinLRCK);
    void init(Decoder *aDecoder);
    boolean loop() ;
    void setVolume(float value);
};
