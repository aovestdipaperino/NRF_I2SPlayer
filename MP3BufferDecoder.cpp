#include "I2SPlayer.h"
#include "mp3_decoder.h"

void MP3BufferDecoder::rewind() {
  bytesRemaining = totalSize - MP3FindSyncWord((unsigned char *)inputBuffer, totalSize);
}

MP3BufferDecoder::MP3BufferDecoder(const unsigned char* anInputBuffer, int aTotalSize) {
  // BUG: this will need to be guarded by a static.
  MP3Decoder_AllocateBuffers();
  totalSize = aTotalSize;
  inputBuffer = anInputBuffer;
  rewind();
}

uint16_t MP3BufferDecoder::fillBuffer(int16_t* buffer, uint16_t size) {
  if (bytesRemaining <= 0) return 0;
  int previousBytes = bytesRemaining;
  auto res = MP3Decode((unsigned char*) inputBuffer + (totalSize - bytesRemaining), &bytesRemaining, buffer, 0);
  if (res) {
    return 0;
  }
  return previousBytes - bytesRemaining;
}
