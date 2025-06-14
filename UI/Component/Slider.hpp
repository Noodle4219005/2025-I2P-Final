#ifndef SLIDER_HPP
#define SLIDER_HPP
#include <functional>

#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"

class Slider : public Engine::ImageButton {
private:
    Image Bar;
    Image End1;
    Image End2;
    std::function<void(double value)> OnValueChangedCallback;
    double value = 0;

public:
    const double Min = 0;
    const double Max = 1;
    bool Down = false;
    Slider(double x, double y, double w, double h);
    void Draw() const override;
    void SetOnValueChangedCallback(std::function<void(double value)> onValueChangedCallback);
    void SetValue(double value);
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
};
#endif   // SLIDER_HPP
