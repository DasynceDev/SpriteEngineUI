#include "boxCollider.h"
#include <physicalComponents/sprite.h>

// Constructor / Destructor

s2d::BoxCollider::BoxCollider()
{
    this->can_collide = false;
    this->is_solid = false;
    this->ptr_sprite = nullptr;
}

s2d::BoxCollider::BoxCollider(s2d::Sprite* sprite, s2d::BoxCollider& rhs)
{
    this->init();
    this->ptr_sprite = sprite;
    this->box_collider_height = rhs.box_collider_height;
    this->box_collider_width = rhs.box_collider_width;
    this->exist = rhs.exist;
    this->is_solid = rhs.is_solid;
}

s2d::BoxCollider::BoxCollider(s2d::Sprite* ptr_sprite)
{
    this->init();
    this->ptr_sprite = ptr_sprite;
}

void s2d::BoxCollider::init()
{
    this->base_component = false;
    this->ptr_sprite = nullptr;
    this->can_collide = false;
    this->is_solid = false;
    this->exist = false;
}
 
// Public functions

void s2d::BoxCollider::reset() 
{
    this->exist = false;
    this->is_solid = false;
    this->can_collide = false;
    this->box_collider_height = Vector2(0, 0);
    this->box_collider_width = Vector2(0, 0);
}
