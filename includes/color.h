#include <time.h>

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
    float t = (float)cur / (float)CLOCKS_PER_SEC;
    int r = (int)(sin(t * speed + 0) * 127 + 128);
    int g = (int)(sin(t * speed + 2) * 127 + 128);
    int b = (int)(sin(t * speed + 4) * 127 + 128);
    return assembleColor(r, g, b);
}
int blendColor (int c1, int c2, float amount) {
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