#ifndef VECTOR_H
#define VECTOR_H

// structs
struct {
    int x;
    int y;
} typedef Vector2Int;

struct {
    double x;
    double y;
} typedef Vector2;

struct {
    int x;
    int y;
    int z;
} typedef Vector3Int;

struct {
    double x;
    double y;
    double z;
} typedef Vector3;

// forward declarations
void normalize (Vector2 *v);
double toAngle (Vector2 *v);

// functions
void normalize (Vector2 *v) {
    double length = sqrt(v->x * v->x + v->y * v->y);
    v->x /= length;
    v->y /= length;
}
double toAngle (Vector2 *v) {
    return atan2(v->y, v->x);
}

#endif