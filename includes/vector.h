
struct {
    int x;
    int y;
} typedef Vector2Int;

struct {
    float x;
    float y;
} typedef Vector2;

struct {
    int x;
    int y;
    int z;
} typedef Vector3Int;

struct {
    float x;
    float y;
    float z;
} typedef Vector3;

void normalize (Vector2 *v) {
    float length = sqrt(v->x * v->x + v->y * v->y);
    v->x /= length;
    v->y /= length;
}