#include "BoxCollider.h"

#include "Sprite/Sprite.h"
#include "Sprite/SpriteRepository.h"

// Constructor / Destructor

spe::BoxCollider::BoxCollider()
{
    this->Init();
}

spe::BoxCollider::BoxCollider(spe::Sprite* sprite, spe::BoxCollider& rhs)
{
    this->Init();
    this->ptr_Sprite = sprite;
    this->Height = rhs.Height;
    this->Width = rhs.Width;
    this->Exist = rhs.Exist;
    this->IsSolid = rhs.IsSolid;
    this->Controller = rhs.Controller;
}

spe::BoxCollider::BoxCollider(spe::Sprite* ptr_sprite)
{
    this->Init();
    this->ptr_Sprite = ptr_sprite;
}

void spe::BoxCollider::Init()
{
    this->BaseComponent = false;
    this->ptr_Sprite = nullptr;
    this->CanCollide = false;
    this->IsSolid = false;
    this->Exist = false;

    this->Left = false;
    this->Right = false;
    this->Up = false;
    this->Down = false;
    this->Collided = false;

    this->m_GotDown = false;
    this->m_GotLeft = false;
    this->m_GotRight = false;
    this->m_GotUp = false;

    this->Controller = false;
}

bool spe::BoxCollider::CheckCollision(spe::BoxCollider& other)
{
    spe::Sprite* ptr_attached_sprite = this->ptr_Sprite;
    if (!this->Exist || !other.Exist)
    {
        return false;
    }

    float getPosX = ptr_attached_sprite->Transform.GetOrigininalPosition().X;
    float getPosY = ptr_attached_sprite->Transform.GetOrigininalPosition().Y;

    float otherGetPosX = other.ptr_Sprite->Transform.GetOrigininalPosition().X;
    float otherGetPosY = other.ptr_Sprite->Transform.GetOrigininalPosition().Y;

    bool isHorizontalOverlapLeft =
        getPosX + ptr_attached_sprite->Transform.TextureSize.X + this->Width.Y >= otherGetPosX + other.Width.X;

    bool isHorizontalOverlapRight =
        getPosX + this->Width.X <= otherGetPosX + other.Width.Y + other.ptr_Sprite->Transform.TextureSize.X;

    bool isVerticalOverlapTop =
        getPosY + ptr_attached_sprite->Transform.TextureSize.Y + this->Height.Y >= otherGetPosY + other.Height.X;

    bool isVerticalOverlapBottom =
        getPosY + this->Height.X <= otherGetPosY + other.Height.Y + other.ptr_Sprite->Transform.TextureSize.Y;

    if (isHorizontalOverlapLeft && isHorizontalOverlapRight && isVerticalOverlapTop && isVerticalOverlapBottom) 
    {
        other.Collided = true;
        other.CollidedInFrame = true;
        this->Collided = true;
        this->CollidedInFrame = true;

        if (this->IsSolid && other.IsSolid)
        {
            this->CheckCollisionPosition(other);
        }
        return true;
    }
    return false;
}

void spe::BoxCollider::CheckCollisionPosition(spe::BoxCollider& other) noexcept
{
    const short range = 10;

    // OTHER
    const float other_right = other.ptr_Sprite->Transform.GetOrigininalPosition().X + other.ptr_Sprite->Transform.TextureSize.X + other.Width.Y;
    const float other_left = other.ptr_Sprite->Transform.GetOrigininalPosition().X + other.Width.X;

    const float other_top = other.ptr_Sprite->Transform.GetOrigininalPosition().Y + other.Height.X;

    // THIS    
    const float this_bottom = this->ptr_Sprite->Transform.GetOrigininalPosition().Y + this->ptr_Sprite->Transform.TextureSize.Y + this->Height.Y;

    const float this_right = this->ptr_Sprite->Transform.GetOrigininalPosition().X + this->ptr_Sprite->Transform.TextureSize.X + this->ptr_Sprite->Collider.Width.Y;
    const float this_left = this->ptr_Sprite->Transform.GetOrigininalPosition().X + this->Width.X;

    // Right
    if (this_right >= other_left
        && this_right <= other_left + range)
    {
        other.m_GotLeft = true;
        other.Left = true,

        this->m_GotRight = true;
        this->Right = true;
        return;
    }


    // Left

    if (this_left <= other_right
        && this_left + range >= other_right)
    {
        other.m_GotRight = true;
        other.Right = true,

        this->m_GotLeft = true;
        this->Left = true;
        return;
    }

    // Down
    if (this_bottom >= other_top
        && this_bottom <= other_top + range)
    {
        other.m_GotUp = true;
        other.Up = true,

        this->m_GotDown = true;
        this->Down = true;
        return;
    }

    other.m_GotDown = true;
    other.Down = true;

    this->m_GotUp = true;
    this->Up = true;
    return;
}

void spe::BoxCollider::ResetPosition() noexcept
{ 
    this->CollidedSprites.clear();
    this->Collided = false;
    this->Right = false;
    this->Left = false;
    this->Down = false;
    this->Up = false;
}
 
// Public functions

void spe::BoxCollider::Reset() 
{
    this->Exist = false;
    this->IsSolid = false;
    this->CanCollide = false;
    this->Height = Vector2(0, 0);
    this->Width = Vector2(0, 0);
}

void spe::BoxCollider::Update(spe::SpriteRepository& tocheck)
{
    if (!this->Controller)
    {
        return;
    }

    spe::Sprite* sprite = this->ptr_Sprite;

    sprite->Collider.CollidedInFrame = false;
    sprite->Collider.m_GotDown = false;
    sprite->Collider.m_GotUp = false;
    sprite->Collider.m_GotRight = false;
    sprite->Collider.m_GotLeft = false;


    spe::Sprite* i_s = sprite;
    
    std::list<spe::Sprite*>& sprites = tocheck.GetSprites();


    for (auto it = sprites.begin(); it != sprites.end(); ++it)
    {
        spe::Sprite* j_s = *it;

        if (i_s->GetId() == j_s->GetId())
        {
            continue;
        }

        if (this->CheckCollision(j_s->Collider))
        {
            i_s->Collider.CollidedSprites[j_s->GetId()] = j_s;
            j_s->Collider.CollidedSprites[i_s->GetId()] = i_s;
        }
        else
        {
            i_s->Collider.CollidedSprites.erase(j_s->GetId());
            j_s->Collider.CollidedSprites.erase(i_s->GetId());
        }
    }


    if (!sprite->Collider.CollidedInFrame)
    {
        sprite->Collider.ResetPosition();
    }
    if (!sprite->Collider.m_GotDown)
    {
        sprite->Collider.Down = false;
    }
    if (!sprite->Collider.m_GotUp)
    {
        sprite->Collider.Up = false;
    }
    if (!sprite->Collider.m_GotRight)
    {
        sprite->Collider.Right = false;
    }
    if (!sprite->Collider.m_GotLeft)
    {
        sprite->Collider.Left = false;
    }
}

// User utility

spe::Sprite* spe::BoxCollider::CollidedWithTag(const std::string& tag)
{
    for (const auto& pair : this->CollidedSprites)
    {
        if (pair.second->Tag == tag)
        {
            return pair.second;
        }
    }

    return nullptr;
}

spe::Sprite* spe::BoxCollider::CollidedWithName(const std::string& name)
{
    for (const auto& pair : this->CollidedSprites)
    {
        if (pair.second->Name == name)
        {
            return pair.second;
        }
    }

    return nullptr;
}

bool spe::BoxCollider::ProcessSprite(spe::Sprite* other, const spe::Camera& camera)
{
    if (other->Name == "CameraCollider")
    {
        return false;
    }

    float zoomfactor = camera.GetZoom();
    if (other->Light.Exist)
    {
        zoomfactor *= 2;
    }
    if (camera.GetZoom() < 0.5f)
    {
		zoomfactor *= 8;
	}

    const spe::Vector2 pos = spe::Vector2(camera.Position.X, camera.Position.Y * -1);
    spe::BoxCollider::s_ptr_CameraCollider->Transform.SetPosition(pos);
    spe::BoxCollider::s_ptr_CameraCollider->Collider.Width = Vector2(-960 * zoomfactor, 960 * zoomfactor);
    spe::BoxCollider::s_ptr_CameraCollider->Collider.Height = Vector2(-540 * zoomfactor, 540 * zoomfactor);

    const spe::Sprite* this_s = spe::BoxCollider::s_ptr_CameraCollider;

    float getPosX = this_s->Transform.GetOrigininalPosition().X;
    float getPosY = this_s->Transform.GetOrigininalPosition().Y;

    float otherGetPosX = other->Transform.GetOrigininalPosition().X;
    float otherGetPosY = other->Transform.GetOrigininalPosition().Y;

    bool isHorizontalOverlapLeft =
        getPosX  + fabs(this_s->Collider.Width.Y) >= otherGetPosX + -fabs(other->Collider.Width.X);

    bool isHorizontalOverlapRight =
        getPosX + -fabs(this_s->Collider.Width.X) <= otherGetPosX + fabs(other->Collider.Width.Y) + other->Transform.TextureSize.X;

    bool isVerticalOverlapTop =
        getPosY + fabs(this_s->Collider.Height.Y) >= otherGetPosY + -fabs(other->Collider.Height.X);

    bool isVerticalOverlapBottom =
        getPosY + -fabs(this_s->Collider.Height.X) <= otherGetPosY + fabs(other->Collider.Height.Y) + other->Transform.TextureSize.Y;

    if (isHorizontalOverlapLeft && isHorizontalOverlapRight && isVerticalOverlapTop && isVerticalOverlapBottom)
    {
        other->Light.EnableProcess();
        return true;
    }
    other->Light.DisableProcess();
    return false;
}

void spe::BoxCollider::InitCameraCollider(spe::LightRepository& repo)
{
    spe::BoxCollider::s_ptr_CameraCollider = new spe::Sprite("CameraCollider", spe::Vector2(0, 0), PATH_TO_RESSOURCES"\\Sprites\\CamColl.png", repo);

    s_ptr_CameraCollider->Collider.Width = Vector2(-960, 960);
    s_ptr_CameraCollider->Collider.Height = Vector2(-540, 540);

}

void spe::BoxCollider::DeleteCameraCollider()
{
    delete spe::BoxCollider::s_ptr_CameraCollider;
}

spe::Sprite* spe::BoxCollider::s_ptr_CameraCollider = nullptr;