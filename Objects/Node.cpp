#include "Node.h"
#include "Skin/Skin.h"
#include "UI/Component/Image.hpp"
#include "util/GameData.h"
#include "util/ErrorCalculator.h"

void Node::KeyDown(int position) 
{
    if (GetHitValue(m_perfectHitPosition) != NONE && m_isFirstKeyDown) {
        m_isFirstKeyDown=false;
        m_isKeyDown=true;
        m_firstKeyPosition=position;
    }
}

void Node::KeyUp() 
{

}

void Node::Update() 
{
    if (isKeyDown)
}

void Node::Draw() const 
{
}

void Node::GetColumn() const
{
    return m_column;
}