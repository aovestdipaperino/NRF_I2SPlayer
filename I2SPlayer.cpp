#include "I2SPlayer.h"

void I2SPlayer::setSampleRate(uint16_t sampleRate)
{
  switch (sampleRate)
  {
    case 22050: //LRCLK at 44.1kHz (for playback speed of 22.05kHz)
      NRF_I2S->CONFIG.MCKFREQ =  I2S_CONFIG_MCKFREQ_MCKFREQ_32MDIV11 << I2S_CONFIG_MCKFREQ_MCKFREQ_Pos;
      NRF_I2S->CONFIG.RATIO = I2S_CONFIG_RATIO_RATIO_128X << I2S_CONFIG_RATIO_RATIO_Pos;
      break;

    default:
      NRF_I2S->CONFIG.MCKFREQ =  I2S_CONFIG_MCKFREQ_MCKFREQ_32MDIV11 << I2S_CONFIG_MCKFREQ_MCKFREQ_Pos;
      NRF_I2S->CONFIG.RATIO = I2S_CONFIG_RATIO_RATIO_64X << I2S_CONFIG_RATIO_RATIO_Pos;
      break;
  }
}

void I2SPlayer::begin(uint8_t pinSDOUT, uint8_t pinSCK, uint8_t pinLRCK) {
  // Enable transmission
  NRF_I2S->CONFIG.TXEN = (I2S_CONFIG_TXEN_TXEN_ENABLE << I2S_CONFIG_TXEN_TXEN_Pos);
  // Enable MCK generator
  NRF_I2S->CONFIG.MCKEN = (I2S_CONFIG_MCKEN_MCKEN_ENABLE << I2S_CONFIG_MCKEN_MCKEN_Pos);
  NRF_I2S->CONFIG.MCKFREQ =  I2S_CONFIG_MCKFREQ_MCKFREQ_32MDIV11 << I2S_CONFIG_MCKFREQ_MCKFREQ_Pos;
  NRF_I2S->CONFIG.RATIO = I2S_CONFIG_RATIO_RATIO_64X << I2S_CONFIG_RATIO_RATIO_Pos;
  // Master mode, 16Bit, left aligned
  NRF_I2S->CONFIG.MODE = I2S_CONFIG_MODE_MODE_MASTER << I2S_CONFIG_MODE_MODE_Pos;
  NRF_I2S->CONFIG.SWIDTH = I2S_CONFIG_SWIDTH_SWIDTH_16BIT << I2S_CONFIG_SWIDTH_SWIDTH_Pos;
  NRF_I2S->CONFIG.ALIGN = I2S_CONFIG_ALIGN_ALIGN_LEFT << I2S_CONFIG_ALIGN_ALIGN_Pos;
  // Format = I2S
  NRF_I2S->CONFIG.FORMAT = I2S_CONFIG_FORMAT_FORMAT_I2S << I2S_CONFIG_FORMAT_FORMAT_Pos;
  // Use stereo
  NRF_I2S->CONFIG.CHANNELS = I2S_CONFIG_CHANNELS_CHANNELS_STEREO << I2S_CONFIG_CHANNELS_CHANNELS_Pos;

  // Configure pins
  NRF_I2S->PSEL.SCK = (pinSCK << I2S_PSEL_SCK_PIN_Pos);
  NRF_I2S->PSEL.LRCK = (pinLRCK << I2S_PSEL_LRCK_PIN_Pos);
  NRF_I2S->PSEL.SDOUT = (pinSDOUT << I2S_PSEL_SDOUT_PIN_Pos);
}

uint16_t I2SPlayer::fillBuffer() {
  int16_t* ptr =  &buffers[bufferToFill][0];
  uint16_t sampleRate;
  // Swap buffer.
  bufferToFill = 1 - bufferToFill;
  
  decodedBytes = _decoder->fillBuffer(ptr, 0, &sampleRate);
  if (volume != 1.0) {
    for (int i = 0; i < FRAME_SAMPLES*2; i++) {
      ptr[i] = (int16_t)(((double)ptr[i]) * volume);
    }
  }
  return sampleRate;
}

void I2SPlayer::play() {
  bufferToFill = 0;
  // in NRF jargon MAXCNT is the number of 32 bit words.
  NRF_I2S->RXTXD.MAXCNT = FRAME_SAMPLES;
  NRF_I2S->EVENTS_TXPTRUPD = 0;
  NRF_I2S->ENABLE = 1;
  fillBuffer();

  NRF_I2S->TXD.PTR =  (uint32_t) &buffers[0][0];
  NRF_I2S->EVENTS_TXPTRUPD = 0;
  NRF_I2S->ENABLE = 1;
  NRF_I2S->TASKS_START = 1;
}

void I2SPlayer::pause() {
  NRF_I2S->TASKS_STOP = 1;
}

void I2SPlayer::stop() {
  pause();
  _decoder->rewind();
}

boolean I2SPlayer::loop() {
  if (decodedBytes) {
    if (NRF_I2S->EVENTS_TXPTRUPD) {
      NRF_I2S->TXD.PTR = (uint32_t)(&buffers[bufferToFill][0]);
      NRF_I2S->EVENTS_TXPTRUPD = 0;
      fillBuffer();
    }
    return true;
  }
  stop();
  return false;
}

void I2SPlayer::setVolume(float value) {
  if (value >= 0 && value <= 10) {
    volume = value;
  }
}
