#include <functional>
#include <memory>

#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "Image.hpp"
#include "ImageButton.hpp"

namespace Engine {
    ImageButton::ImageButton(std::string img, std::string imgIn, float x, float y, float w, float h, float anchorX, float anchorY) : Image(img, x, y, w, h, anchorX, anchorY), imgOut(Resources::GetInstance().GetBitmap(img)), imgIn(Resources::GetInstance().GetBitmap(imgIn)) {
        Point mouse = GameEngine::GetInstance().GetMousePosition();
        // mouseIn = Collider::IsPointInBitmap(, bmp);
        mouseIn = Collider::IsPointInRect(Point{(float)mouse.x, (float)mouse.y},
                                          Point{Image::Position.x - GetBitmapWidth()*Anchor.x, Image::Position.y - GetBitmapHeight()*Anchor.y},
                                          Point{(float)GetBitmapWidth(), (float)GetBitmapHeight()}
                                          );
        imgInName=imgIn;
        imgOutName=img;
        if (!mouseIn || !Enabled) bmp = imgOut;
        else bmp = this->imgIn;
    }

    void ImageButton::SetOnClickCallback(std::function<void(void)> onClickCallback) {
        OnClickCallback = onClickCallback;
    }

    void ImageButton::OnMouseDown(int button, int mx, int my) {
        if ((button & 1) && mouseIn && Enabled) {
            if (OnClickCallback)
                OnClickCallback();
        }
    }

    void ImageButton::OnMouseMove(int mx, int my) {
        // mouseIn=Position.x<=mx && mx<=Position.x+GetBitmapWidth() && Position.y<=my && my<=Position.y+GetBitmapHeight();
        // mouseIn = Collider::IsPointInBitmap(Point((mx - Position.x) * GetBitmapWidth() / Size.x + Anchor.x * GetBitmapWidth(), (my - Position.y) * GetBitmapHeight() / Size.y + Anchor.y * GetBitmapHeight()), bmp);
        mouseIn = Collider::IsPointInRect(Point{(float)mx, (float)my},
                                          Point{Image::Position.x - GetBitmapWidth()*Anchor.x, Image::Position.y - GetBitmapHeight()*Anchor.y},
                                          Point{(float)GetBitmapWidth(), (float)GetBitmapHeight()}
                                          );
        if (!mouseIn || !Enabled) bmp = imgOut;
        else bmp = imgIn;
    }

    void ImageButton::Draw() const
    {
        if (imgInName!=imgOutName) Image::Draw();
        else {
            if (!mouseIn) {
                al_draw_tinted_scaled_bitmap
                (
                    bmp.get(), al_map_rgba(255, 255, 255, 100),
                    0, 0, GetBitmapWidth(), GetBitmapHeight(),
                    Position.x - Anchor.x * GetBitmapWidth(), Position.y - Anchor.y * GetBitmapHeight(),
                    Size.x, Size.y, 0
                );
            }
            else {
                Image::Draw();
            }
        }
    }

}
