#include <Arduino.h>
#include <TFT_eSPI.h> 

TFT_eSPI tft = TFT_eSPI();

#define TFT_HALF_WIDTH (TFT_WIDTH >> 1)
#define TFT_HALF_HEIGHT (TFT_HEIGHT >> 1)

#define NSTARS 1024
uint8_t sx[NSTARS] = {};
uint8_t sy[NSTARS] = {};
uint8_t sz[NSTARS] = {};

uint8_t za, zb, zc, zx;

uint8_t __attribute__((always_inline)) rng() {
  zx++;
  za = (za^zc^zx);
  zb = (zb+za);
  zc = ((zc+(zb>>1))^za);
  return zc;
}

void setup() {
  za = random(256);
  zb = random(256);
  zc = random(256);
  zx = random(256);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  uint8_t spawnDepthVariation = 255;

  for (int i=0; i<NSTARS; ++i) {
    if (sz[i] <= 1) {
      sx[i] = TFT_HALF_WIDTH - TFT_HALF_HEIGHT + rng();
      sy[i] = rng();
      sz[i] = spawnDepthVariation--;
    } else {
      int old_screen_x = ((int)sx[i] - TFT_HALF_WIDTH) * 256 / sz[i] + TFT_HALF_WIDTH;
      int old_screen_y = ((int)sy[i] - TFT_HALF_HEIGHT) * 256 / sz[i] + TFT_HALF_HEIGHT;
      tft.drawPixel(old_screen_x, old_screen_y, TFT_BLACK);

      sz[i] -= 2;
      if (sz[i] > 1) {
        int screen_x = ((int)sx[i] - TFT_HALF_WIDTH) * 256 / sz[i] + TFT_HALF_WIDTH;
        int screen_y = ((int)sy[i] - TFT_HALF_HEIGHT) * 256 / sz[i] + TFT_HALF_HEIGHT;

        if ((screen_x >= 0) && (screen_y >= 0) && (screen_x < TFT_WIDTH) && (screen_y < TFT_HEIGHT)) {
          int r, g, b;
          r = g = b = 255 - sz[i];
          tft.drawPixel(screen_x, screen_y, tft.color565(r, g, b));
        } else {
          sz[i] = 0;
        }
      }
    }
  }
}
