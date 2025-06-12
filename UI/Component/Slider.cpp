#include <algorithm>
#include <string>

#include "Engine/Point.hpp"
#include "Slider.hpp"

Slider::Slider(double x, double y, double w, double h)
  : ImageButton("stage-select/slider.png", "stage-select/slider-blue.png", x, y),
  Bar("stage-select/bar.png", x, y, w, h),
  End1("stage-select/end.png", x, y + h / 2, 0, 0, 0.5, 0.5),
  End2("stage-select/end.png", x + w, y + h / 2, 0, 0, 0.5, 0.5) {
  Position.x += w;
  Position.y += h / 2;
  Anchor = Engine::Point(0.5, 0.5);
}
void Slider::Draw() const {
  Bar.Draw();
  End1.Draw();
  End2.Draw();
  ImageButton::Draw();
}
void Slider::SetOnValueChangedCallback(std::function<void(double value)> onValueChangedCallback) {
  OnValueChangedCallback = onValueChangedCallback;
}
void Slider::SetValue(double value) {
  ImageButton::Position.x=Bar.Position.x+value*Bar.Size.x;
  OnValueChangedCallback(value);

}
void Slider::OnMouseDown(int button, int mx, int my) {
  if ((button & 1) && mouseIn)
    Down = true;
}
void Slider::OnMouseUp(int button, int mx, int my) {
  Down = false;
}
void Slider::OnMouseMove(int mx, int my) {
  ImageButton::OnMouseMove(mx, my);
  if (Down) {
    // Clamp
    double clamped = std::min(std::max(static_cast<double>(mx), Bar.Position.x), Bar.Position.x + Bar.Size.x);
    double value = (clamped - Bar.Position.x) / Bar.Size.x * 1.0f;
    SetValue(value);
  }
}
