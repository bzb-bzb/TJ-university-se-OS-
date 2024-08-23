#include <am.h>
#include <amdev.h>
#include <klib.h>
#include <klib-macros.h>

#define SIDE 16
#define TILE_SIZE 40  // Tile size for the image

static int w, h;  // Screen size

#define KEYNAME(key) \
  [AM_KEY_##key] = #key,
static const char *key_names[] = { AM_KEYS(KEYNAME) };

// Define at least 10 colors
static const uint32_t colors[] = {
    0xff0000, // Red
    0x00ff00, // Green
    0x0000ff, // Blue
    0xffff00, // Yellow
    0xff00ff, // Magenta
    0x00ffff, // Cyan
    0xffffff, // White
    0x000000, // Black
    0x808080, // Gray
    0xffa500, // Orange
    0x800080, // Purple
    0x00ff7f  // Spring Green
};

static inline void puts(const char *s) {
  for (; *s; s++) putch(*s);
}

void print_key() {
  AM_INPUT_KEYBRD_T event = { .keycode = AM_KEY_NONE };
  ioe_read(AM_INPUT_KEYBRD, &event);
  if (event.keycode != AM_KEY_NONE && event.keydown) {
    if (strcmp(key_names[event.keycode], "ESCAPE") == 0){
      halt(0);  // Exit on ESC key
    }
    puts("Key pressed: ");
    puts(key_names[event.keycode]);
    puts("\n");
  }
}




static void draw_tile(int x, int y, int w, int h, uint32_t color) {
  uint32_t pixels[w * h]; // WARNING: large stack-allocated memory
  AM_GPU_FBDRAW_T event = {
    .x = x, .y = y, .w = w, .h = h, .sync = 1,
    .pixels = pixels,
  };
  for (int i = 0; i < w * h; i++) {
    pixels[i] = color;
  }
  ioe_write(AM_GPU_FBDRAW, &event);
}

void splash() {
  AM_GPU_CONFIG_T info = {0};
  ioe_read(AM_GPU_CONFIG, &info);
  w = info.width;
  h = info.height;

  // Drawing an image pattern with at least 10 colors
  for (int x = 0; x * TILE_SIZE < w; x++) {
    for (int y = 0; y * TILE_SIZE < h; y++) {
      // Use modulo to cycle through the colors array
      uint32_t color = colors[(x + y) % (sizeof(colors) / sizeof(colors[0]))];
      draw_tile(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
    }
  }
}

// Operating system is a C program!
int main(const char *args) {
  ioe_init();

  puts("mainargs = \"");
  puts(args);  // make run mainargs=xxx
  puts("\"\n");

  splash();

  puts("Press any key to see its key code...\n");
  while (1) {
    print_key();
  }
  return 0;
}
