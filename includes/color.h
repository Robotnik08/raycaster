#ifndef COLOR_H
#define COLOR_H

#include <time.h>


enum Colors {
    BLACK = 0x000000,
    WHITE = 0xFFFFFF,
    RED = 0xFF0000,
    GREEN = 0x00FF00,
    BLUE = 0x0000FF,
    YELLOW = 0xFFFF00,
    CYAN = 0x00FFFF,
    MAGENTA = 0xFF00FF,
    ORANGE = 0xFF8000,
    PURPLE = 0x8000FF,
    PINK = 0xFF0080,
    LIME = 0x80FF00,
    TEAL = 0x008080,
    LAVENDER = 0x800080,
    BROWN = 0x804000,
    MAROON = 0x800000,
    OLIVE = 0x808000,
    NAVY = 0x000080,
    SKY = 0x0080FF,
    TAN = 0x808040,
    BEIGE = 0x808080,
    GRAY = 0x404040,
    DARK_GREEN = 0x008000,

    RAINBOW = -1
};


int getRed (int c) {
    return (c >> 16) & 0xFF;
}
int getGreen (int c) {
    return (c >> 8) & 0xFF;
}
int getBlue (int c) {
    return c & 0xFF;
}
int assembleColor (int r, int g, int b) {
    return (r << 16) | (g << 8) | b;
}
/// @brief Returns a color from the rainbow spectrum, based on the current time.
/// @return 
int rainbow (int speed) {
    clock_t cur = clock();
    double t = (double)cur / (double)CLOCKS_PER_SEC;
    int r = (int)(sin(t * speed + 0) * 127 + 128);
    int g = (int)(sin(t * speed + 2) * 127 + 128);
    int b = (int)(sin(t * speed + 4) * 127 + 128);
    return assembleColor(r, g, b);
}
int blendColor (int c1, int c2, double amount) {
    int r1 = getRed(c1);
    int g1 = getGreen(c1);
    int b1 = getBlue(c1);
    int r2 = getRed(c2);
    int g2 = getGreen(c2);
    int b2 = getBlue(c2);
    int r = (int)(r1 * (1 - amount) + r2 * amount);
    int g = (int)(g1 * (1 - amount) + g2 * amount);
    int b = (int)(b1 * (1 - amount) + b2 * amount);
    return assembleColor(r, g, b);
}

#endif