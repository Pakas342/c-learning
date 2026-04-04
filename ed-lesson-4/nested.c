#include <assert.h>
#include <math.h>

enum ShapeType { CIRCLE, RECTANGLE, TRIANGLE };

struct Shape {
    enum ShapeType type;
    union {
        struct {
            float radius;
        } circle;

        struct {
            float width;
            float height;
        } rectangle;

        struct {
            float base;
            float height;
        } triangle;
    } data;
};

float area(struct Shape *ptr) {
    switch (ptr->type) {
    case CIRCLE:
        return 3.14159f * pow(ptr->data.circle.radius, 2);

    case RECTANGLE:
        return (ptr->data.rectangle.height * ptr->data.rectangle.width);

    case TRIANGLE:
        return (ptr->data.triangle.height * ptr->data.triangle.base) / 2;
    }
}
int main() {

    const float alpha = 0.0001;

    // Test circle
    struct Shape circle;
    circle.type = CIRCLE;
    circle.data.circle.radius = 2.0;

    float circle_area = area(&circle);
    assert(fabs(circle_area - (3.14159f * 2.0f * 2.0f)) < alpha);

    // Rect area
    struct Shape rect;
    rect.type = RECTANGLE;
    rect.data.rectangle.width = 2.0;
    rect.data.rectangle.height = 2.0;

    float rect_area = area(&rect);
    assert(fabs(rect_area - (2.0 * 2.0)) < alpha);

    // triangle area
    struct Shape triangle;
    triangle.type = TRIANGLE;
    triangle.data.triangle.height = 2.0;
    triangle.data.triangle.base = 2.0;

    float tri_area = area(&triangle);
    assert(fabs(tri_area - ((2.0 * 2.0) / 2.0)) < alpha);
}
