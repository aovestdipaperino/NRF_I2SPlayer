#include "I2SPlayer.h"
#include "mp3_decoder.h"

void MP3BufferDecoder::init(const unsigned char* anInputBuffer, int aTotalSize) {
  bytesRemaining = aTotalSize;
  inputBuffer = anInputBuffer;
  auto offset = MP3FindSyncWord((unsigned char *)anInputBuffer, bytesRemaining);
  bytesRemaining -= offset;
  inputBuffer += offset;
}

uint16_t MP3BufferDecoder::fillBuffer(int16_t* buffer, uint16_t size) {
  if (bytesRemaining <= 0) return 0;
  int previousBytes = bytesRemaining;
  auto res = MP3Decode((unsigned char*) inputBuffer, &bytesRemaining, buffer, 0);
  if (res) {
    return 0;
  }
  int consumed = previousBytes - bytesRemaining;
  inputBuffer += consumed;

  return consumed;
}
