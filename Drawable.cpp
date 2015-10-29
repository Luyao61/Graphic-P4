#include "Drawable.h"
#include "Window.h"

Drawable::Drawable()
{
    this->toWorld.identity();
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