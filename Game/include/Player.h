#pragma once
#include "Component.h"
#include "InputModule.h"

class Physics;
class AnimatedSpriteComponent;
class Background;
class SquareCollider;

class Player : public Component
{
public:

    void Update(const float _delta_time) override
    {
        GameObject* owner = GetOwner();
        Maths::Vector2<float> position = owner->GetPosition();
        Maths::Vector2<float> velocity = owner->GetVelocity();
        gravity = owner->GetComponent<Physics>()->gravity;

        GetOwner()->SetPosition(Maths::Vector2f(position.x, position.y - 0.1f));

        if (InputModule::GetKey(sf::Keyboard::D) && !owner->GetComponent<Physics>()->collidingRight)
        {
            owner->GetComponent<AnimatedSpriteComponent>()->SetDirection(AnimatedSpriteComponent::MovementDirection::Right);
            MoveWorld(-speed * _delta_time);
        }
        if (InputModule::GetKey(sf::Keyboard::Q) && !owner->GetComponent<Physics>()->collidingLeft)
        {
            owner->GetComponent<AnimatedSpriteComponent>()->SetDirection(AnimatedSpriteComponent::MovementDirection::Left);
            MoveWorld(speed * _delta_time);
        }

        if (InputModule::GetKey(sf::Keyboard::S))
        {
            //velocity.y += speed * _delta_time;
        }

        if (InputModule::GetKey(sf::Keyboard::Z) && !owner->GetComponent<Physics>()->IsJumping())
        {
            velocity.y = -jumpForce;
            owner->GetComponent<Physics>()->SetJumping(true);
            owner->GetComponent<AnimatedSpriteComponent>()->SetDirection(AnimatedSpriteComponent::MovementDirection::Up);
        }

        if (owner->GetComponent<Physics>()->IsJumping())
        {
            velocity.y += gravity.y * _delta_time;
        }

        position += velocity * _delta_time;

        owner->SetVelocity(velocity);
        owner->SetPosition(position);
    }

    void MoveWorld(float delta)
    {
        for (auto& game_object : GetOwner()->GetScene()->GetGameObjects())
        {
            game_object->SetPosition(Maths::Vector2f(game_object->GetPosition().x + delta, game_object->GetPosition().y));
        }
        for (auto& background : GetOwner()->GetScene()->GetBackgrounds())
        {
            background->SetPosition(background->GetPosition().x + delta, background->GetPosition().y);
            if (background->GetPosition().x <= 0.0f - 1600.0f)
            {
                background->SetPosition(1600.0f, background->GetPosition().y);
            }
            else if (background->GetPosition().x >= 1600.0f)
            {
                background->SetPosition(-1600.0f, background->GetPosition().y);
            }
        }
    }


private:

	float targetY = 0.0f;
	float newY = 0.0f;
	float jumpForce = 800.0f;
	float speed = 600.0f;
	Maths::Vector2<float> gravity;
};

