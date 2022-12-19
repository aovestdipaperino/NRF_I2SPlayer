#include <nrf.h>
#include "shot.h"
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
  I2SPlayer* player = new I2SPlayer(new MP3BufferDecoder(sound, (int) sizeof(sound)));
  Serial.println("I2SPlayer");

  Serial.println("Decoder");
  player->begin(PIN_SDOUT, PIN_SCK, PIN_LRCK);
  Serial.println("begin");
  player->setVolume(0.1);
  for (int i = 0; i < 3; i++) {
    player->decoder()->rewind();
    player->start();
    while (player -> loop());
    delay(2000);
  }
  delete(player);
}

void loop() {

}
