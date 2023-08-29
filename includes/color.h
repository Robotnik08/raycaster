int blendColor (int c1, int c2) {
    int r1 = (c1 >> 16) & 0xFF;
    int g1 = (c1 >> 8) & 0xFF;
    int b1 = c1 & 0xFF;
    int r2 = (c2 >> 16) & 0xFF;
    int g2 = (c2 >> 8) & 0xFF;
    int b2 = c2 & 0xFF;
    int r = (r1 + r2) / 2;
    int g = (g1 + g2) / 2;
    int b = (b1 + b2) / 2;
    return (r << 16) | (g << 8) | b;
}
int getRed (int c) {
    return (c >> 16) & 0xFF;
}
int getGreen (int c) {
    return (c >> 8) & 0xFF;
}
int getBlue (int c) {
    return c & 0xFF;
}