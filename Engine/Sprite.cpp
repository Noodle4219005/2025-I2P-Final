#include <allegro5/allegro.h>
#include <memory>

#include "Engine/Point.hpp"
#include "Sprite.hpp"
#include "UI/Component/Image.hpp"

namespace Engine {
    Sprite::Sprite(std::string img, double x, double y, double w, double h, double anchorX, double anchorY,
                   double rotation, double vx, double vy, unsigned char r, unsigned char g, unsigned char b, unsigned char a) : Image(img, x, y, w, h, anchorX, anchorY), Rotation(rotation), Velocity(Point(vx, vy)), Tint(al_map_rgba(r, g, b, a)) {
    }
    void Sprite::Draw() const {
        al_draw_tinted_scaled_rotated_bitmap(bmp.get(), Tint, Anchor.x * GetBitmapWidth(), Anchor.y * GetBitmapHeight(),
                                             Position.x, Position.y, Size.x / GetBitmapWidth(), Size.y / GetBitmapHeight(), Rotation, 0);
    }
    void Sprite::Update(double deltaTime) {
        Position.x += Velocity.x * deltaTime;
        Position.y += Velocity.y * deltaTime;
    }
};
