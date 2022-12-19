#include <nrf.h>
#include "sine.h"
#include "I2SPlayer.h"

// properly destruct the decoder
// better double buffering naming. e.g. bufferToPlay, bufferToFill
// map pin

#define PIN_SDOUT  (44)
#define PIN_SCK    (45)
#define PIN_LRCK   (46)

void setup()
{
  Serial.begin(9600);
  Serial.println("started");
  I2SPlayer* player = new I2SPlayer();
  Serial.println("I2SPlayer");
  MP3BufferDecoder* decoder = new MP3BufferDecoder();
  Serial.println("Decoder");
  player->begin(PIN_SDOUT, PIN_SCK, PIN_LRCK);
  Serial.println("begin");
  //  player->setVolume(0.01);
  for (int i = 0; i < 3; i++) {
    decoder->init(sound, (int) sizeof(sound));
    player->init(decoder);
    while (player -> loop());
    delay(2000);
  }
  delete(decoder);
  delete(player);
}

void loop() {

}
