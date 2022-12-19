#include "I2SPlayer.h"

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

void I2SPlayer::fillBuffer() {
  Serial.println("+");
  int16_t* ptr = fillBufferOne ? &bufferOne[0] : &bufferTwo[0];

  fillBufferOne = !fillBufferOne;
  decodedBytes = _decoder->fillBuffer(ptr, 0);
  if (volume != 1.0) {
    for (int i = 0; i < FRAME_SAMPLES; i++) {
      ptr[i] = (int16_t)(((double)ptr[i]) * volume);
    }
  }
}

void I2SPlayer::start() {
  NRF_I2S->RXTXD.MAXCNT = FRAME_SAMPLES / 2;
  NRF_I2S->EVENTS_TXPTRUPD = 0;
  NRF_I2S->ENABLE = 1;
  fillBuffer();

  NRF_I2S->TXD.PTR =  (uint32_t) &bufferOne[0];
  NRF_I2S->EVENTS_TXPTRUPD = 0;
  NRF_I2S->ENABLE = 1;
  NRF_I2S->TASKS_START = 1;
}

boolean I2SPlayer::loop() {
  if (decodedBytes) {
    if (NRF_I2S->EVENTS_TXPTRUPD) {
      NRF_I2S->TXD.PTR = (uint32_t)(!fillBufferOne ? &bufferTwo[0] : &bufferOne[0]);
      NRF_I2S->EVENTS_TXPTRUPD = 0;
      fillBuffer();
    }
    return true;
  }
  NRF_I2S->TASKS_STOP = 1;
  return false;
}

void I2SPlayer::setVolume(float value) {
  if (value >= 0 && value <= 10) {
    volume = value;
  }
}
