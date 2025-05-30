#include "Skin/Skin.h"
#include "UI/Component/Image.hpp"
#include "util/GameData.h"

class Node : public Engine::Image, protected Skin
{
private:
    m_perfectHitPosition;
public:
    Node(int column, int position)
    {
    }

    void Draw() const;
};

void Update() 
{
    if ()
}

void Draw() const 
{
}