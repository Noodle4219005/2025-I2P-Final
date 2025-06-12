#include "IObject.hpp"
#include "Engine/Point.hpp"

namespace Engine {
    IObject::IObject(double x, double y, double w, double h, double anchorX, double anchorY) : Position(Point(x, y)), Size(Point(w, h)), Anchor(Point(anchorX, anchorY)) {}
    std::list<std::pair<bool, IObject *>>::iterator IObject::GetObjectIterator() const {
        return objectIterator;
    }
    void IObject::Draw() const {}
    void IObject::Update(double deltaTime) {}
}
