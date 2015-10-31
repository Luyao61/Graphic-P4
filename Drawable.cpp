#include "Drawable.h"
#include "Window.h"

Drawable::Drawable()
{
    this->toWorld.identity();
    this->center.set(0,0,0,1);
}

Drawable::~Drawable()
{
    //
}

void Drawable::draw(DrawData& data)
{
    std::cout << "Warning: draw() is not implemented" << std::endl;
}

void Drawable::update(UpdateData& data)
{
    std::cout << "Warning: update() is not implemented" << std::endl;
}

void Drawable::makeRotateArbitrary(Vector3 axis, float angle){
    if (axis.magnitude() != 0) {
        Matrix4 rotation;
        rotation.makeRotateArbitrary(axis, angle);
        toWorld = toWorld*rotation;
    }
}
void Drawable::makeTranslate(float x, float y, float z){
    Matrix4 move;
    move = move.makeTranslate(x, y, z);
    toWorld = move * toWorld;
    center = move * center;
    center.toVector3().print("Object new Position");
}
