

//int16_t fillNextWavBuffer() {
//  const uint32_t TOTAL_SAMPLES = sizeof(sound) / sizeof(uint16_t);
//  if (currentOffset > TOTAL_SAMPLES) {
//    return 0;
//  }
//  int16_t* ptr = fillBufferOne ? bufferOne : bufferTwo;
//  fillBufferOne = !fillBufferOne;
//  int16_t len = min(FRAME_SAMPLES * 2, TOTAL_SAMPLES - currentOffset);
//
//  for (int i = 0; i < len; i++) {
//    auto sample = sound[i + currentOffset];
//    ptr[i] =  sound[i + currentOffset]; // sample >> 8 | sample << 8;
//  }
//
//  currentOffset += len;
//  return len;
//}
