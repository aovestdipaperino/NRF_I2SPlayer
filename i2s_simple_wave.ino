#include <nrf.h>
#include "shot.h"
#include "I2SPlayer.h"

// properly destruct the decoder
// map pin

#define PIN_SDOUT  (44)
#define PIN_SCK    (45)
#define PIN_LRCK   (46)

unsigned char stupid_buffer [ 70*1024];
void setup()
{
  Serial.begin(9600);
  while(!Serial) delay(10);
  I2SPlayer* player = new I2SPlayer(new MP3BufferDecoder(sound, (int) sizeof(sound), stupid_buffer));
  player->begin(PIN_SDOUT, PIN_SCK, PIN_LRCK);
  player->setVolume(0.5);
  for (int i = 0; i < 3; i++) {
    player->play();
    while (player -> loop());
    delay(2000);
  }
  delete(player);
}

void loop() {

}
