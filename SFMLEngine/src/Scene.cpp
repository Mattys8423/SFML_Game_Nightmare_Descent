#include "Scene.h"
#include <SFML/Graphics/RenderWindow.hpp>

// Constructeur de la classe Scene. Initialise le nom de la scene.
Scene::Scene(const std::string& _name)
{
	name = _name;
}

// Methode pour reveiller tous les GameObjects de la scene.
void Scene::Awake() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->Awake();
	}
}

// Methode pour demarrer tous les GameObjects de la scene.
void Scene::Start() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->Start();
	}
}

// Methode pour pre-rendre tous les GameObjects de la scene.
void Scene::PreRender() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->PreRender();
	}
}

// Methode pour dessiner l'interface utilisateur de tous les GameObjects de la scene.
void Scene::OnGUI() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->OnGUI();
	}
}

// Methode pour post-rendre tous les GameObjects de la scene.
void Scene::PostRender() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->PostRender();
	}
}

// Methode pour dessiner les informations de debogage de tous les GameObjects de la scene.
void Scene::OnDebug() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->OnDebug();
	}
}

// Methode pour dessiner les informations de debogage des GameObjects selectionnes de la scene.
void Scene::OnDebugSelected() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->OnDebugSelected();
	}
}

// Methode pour presenter tous les GameObjects de la scene.
void Scene::Present() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->Present();
	}
}

// Methode pour activer tous les GameObjects de la scene.
void Scene::OnEnable() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->OnEnable();
	}
}

// Methode pour desactiver tous les GameObjects de la scene.
void Scene::OnDisable() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->OnDisable();
	}
}

// Methode pour detruire tous les GameObjects de la scene.
void Scene::Destroy() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->Destroy();
	}
}

// Methode pour finaliser tous les GameObjects de la scene.
void Scene::Finalize() const
{
	for (GameObject* const& game_object : gameObjects)
	{
		game_object->Finalize();
	}
}

// Methode pour mettre a jour tous les GameObjects de la scene.
void Scene::Update(const float _delta_time)
{
	// Update all GameObjects
	for (auto& gameObject : gameObjects) {
		gameObject->Update(_delta_time);
	}

	// Delete all GameObjects marked for deletion
	for (auto it = gameObjects.begin(); it != gameObjects.end(); /* no increment here */) {
		if ((*it)->IsMarkedForDeletion()) {
			delete* it;
			it = gameObjects.erase(it);
		}
		else {
			++it;
		}
	}
}


// Methode pour dessiner tous les GameObjects de la scene.
void Scene::Render(sf::RenderWindow* _window)
{
	for (Background* const& background : backgrounds)
	{
		sf::Sprite sprite = background->GetSprite();
		_window->draw(sprite);
	}

	for (GameObject* const& game_object : gameObjects)
	{
		game_object->Render(_window);
	}
}

// Methode pour obtenir le nom de la scene.
const std::string& Scene::GetName() const
{
	return name;
}

// Methode pour creer un GameObject dans la scene.
GameObject* Scene::CreateInGameObject(const std::string& _name, const std::string& _type, Maths::Vector2f _position, Maths::Vector2f _size, float _mass, const sf::Color _color)
{
	// Cree le GameObject et le configure.
	GameObject* game_object = CreateGameObject(_name, _type, id);
	game_object->SetPosition(Maths::Vector2f(_position));

	// Si le GameObject est un joueur, cree un composant Physics pour lui.
	if (game_object->GetType() == "Player")
	{
		Physics* physics = game_object->CreateComponent<Physics>();
		physics->SetMass(1.0f);
	}

	if (game_object->GetType() == "Enemy")
	{
		Physics* physics = game_object->CreateComponent<Physics>();
		physics->SetMass(1.0f);
	}

	RectangleShapeRenderer* shape_renderer = game_object->CreateComponent<RectangleShapeRenderer>();
	shape_renderer->SetColor(_color);

	// Cree un SquareCollider et un RectangleShapeRenderer pour le GameObject.
	SquareCollider* square_collider = game_object->CreateComponent<SquareCollider>();
	square_collider->SetHeight(_size.y);

	if (game_object->GetType() == "Player" || game_object->GetType() == "Enemy")
	{
		square_collider->SetWidth(_size.x);
		shape_renderer->SetSize(Maths::Vector2f(_size.x, _size.y));
	}
	else
	{
		square_collider->SetWidth(_size.x);
		shape_renderer->SetSize(_size);
	}



	// Si le GameObject est le joueur, cree un AnimatedSpriteComponent et un Health pour lui.
	if (game_object->GetType() == "Player")
	{
		AnimatedSpriteComponent* animated_sprite = game_object->CreateComponent<AnimatedSpriteComponent>();
		animated_sprite->SetFrameTime(0.1f);
		// Definir le chemin du fichier pour l'etat Idle
		animated_sprite->SetStateFilePath(AnimatedSpriteComponent::PlayerSpriteState::Idle, "Assets/Idle.png");
		// Charger la texture a partir du chemin du fichier pour l'etat Idle
		animated_sprite->LoadTexture(animated_sprite->GetStateFilePath(AnimatedSpriteComponent::PlayerSpriteState::Idle));

		// Definir le nombre de frames pour l'etat Running
		animated_sprite->SetStateFrameCount(AnimatedSpriteComponent::PlayerSpriteState::Running, 8);

		// Definir le nombre de frames pour l'etat Idle
		animated_sprite->SetStateFrameCount(AnimatedSpriteComponent::PlayerSpriteState::Idle, 15);

		// Definir le nombre de frames pour l'etat Running
		animated_sprite->SetStateFrameCount(AnimatedSpriteComponent::PlayerSpriteState::Jump, 8);


		// Definir l'etat actuel a Idle
		animated_sprite->state = AnimatedSpriteComponent::PlayerSpriteState::Idle;

		// Calcule l'echelle en fonction de la taille du gameObject et du sprite (_size *taille du gameObject* / _sprite *taille du sprite*)
		float scaleX = (_size.x / (animated_sprite->GetSprite()->getTextureRect().width / animated_sprite->GetFrameCount()));
		float scaleY = (_size.y / (animated_sprite->GetSprite()->getTextureRect().height));

		// Definis l'echelle du sprite
		animated_sprite->GetSprite()->setScale(scaleX, scaleY);
		animated_sprite->SetDefaultScale(scaleX, scaleY);

		Health* health = game_object->CreateComponent<Health>();
		health->SetMaxHealth(100);
		health->SetHealth(100);

		PlayerAttack* attack = game_object->CreateComponent<PlayerAttack>();
		attack->SetCollisionDamage(100.0f);
	}

	// Si le GameObject est le joueur, cree un AnimatedSpriteComponent et un Health pour lui.
	if (game_object->GetType() == "Enemy")
	{
		AnimatedSpriteComponent* animated_sprite = game_object->CreateComponent<AnimatedSpriteComponent>();
		animated_sprite->SetFrameTime(0.1f);
		// Definir le chemin du fichier pour l'etat Idle
		animated_sprite->SetStateFilePath(AnimatedSpriteComponent::PlayerSpriteState::Idle, "Assets/Idle.png");
		// Charger la texture a partir du chemin du fichier pour l'etat Idle
		animated_sprite->LoadTexture(animated_sprite->GetStateFilePath(AnimatedSpriteComponent::PlayerSpriteState::Idle));

		// Definir le nombre de frames pour l'etat Running
		animated_sprite->SetStateFrameCount(AnimatedSpriteComponent::PlayerSpriteState::Running, 8);

		// Definir le nombre de frames pour l'etat Idle
		animated_sprite->SetStateFrameCount(AnimatedSpriteComponent::PlayerSpriteState::Idle, 15);

		// Definir le nombre de frames pour l'etat Jump
		animated_sprite->SetStateFrameCount(AnimatedSpriteComponent::PlayerSpriteState::Jump, 8);

		// Definir l'etat actuel a Idle
		animated_sprite->state = AnimatedSpriteComponent::PlayerSpriteState::Idle;

		// Calcule l'echelle en fonction de la taille du gameObject et du sprite (_size *taille du gameObject* / _sprite *taille du sprite*)
		float scaleX = (_size.x / (animated_sprite->GetSprite()->getTextureRect().width / animated_sprite->GetFrameCount()));
		float scaleY = (_size.y / (animated_sprite->GetSprite()->getTextureRect().height));

		// Definis l'echelle du sprite
		animated_sprite->GetSprite()->setScale(scaleX, scaleY);
		animated_sprite->SetDefaultScale(scaleX, scaleY);

		Health* health = game_object->CreateComponent<Health>();
		health->SetMaxHealth(50);
		health->SetHealth(50);

		EnemyAttack* attack = game_object->CreateComponent<EnemyAttack>();
		attack->SetCollisionDamage(25.0f);
	}

	// Configure le GameObject pour qu'il appartienne a cette scene et ait les memes arriere-plans que cette scene.
	game_object->SetScene(this);
	game_object->SetBackgrounds(backgrounds);

	id++;

	return game_object;
}

// Methode pour creer un GameObject.
GameObject* Scene::CreateGameObject(const std::string& _name, const std::string& _type, const int id)
{
	GameObject* const game_object = new GameObject();
	game_object->SetName(_name);
	game_object->SetType(_type);
	game_object->SetId(id);
	gameObjects.push_back(game_object);

	return game_object;
}

// Methode pour creer un projectile.
GameObject* Scene::CreateProjectile(const std::string& _name, const std::string& _type, Maths::Vector2f _position, Maths::Vector2f _size, const float _damage, const float _speed, const sf::Color _color)
{
	// Cree le GameObject et le configure.
	GameObject* game_object = CreateGameObject(_name, _type, id);
	game_object->SetPosition(Maths::Vector2f(_position));

	RectangleShapeRenderer* shape_renderer = game_object->CreateComponent<RectangleShapeRenderer>();
	shape_renderer->SetColor(_color);

	// Cree un SquareCollider et un RectangleShapeRenderer pour le GameObject.
	SquareCollider* square_collider = game_object->CreateComponent<SquareCollider>();
	square_collider->SetHeight(_size.y);
	square_collider->SetWidth(_size.x);
	shape_renderer->SetSize(_size);

	AnimatedSpriteComponent* animated_sprite = game_object->CreateComponent<AnimatedSpriteComponent>();
	animated_sprite->SetFrameTime(0.1f);
	// Definir le chemin du fichier pour l'etat Idle
	animated_sprite->SetStateFilePath(AnimatedSpriteComponent::PlayerSpriteState::Idle, "Assets/WaterBall.png");
	// Charger la texture a partir du chemin du fichier pour l'etat Idle
	animated_sprite->LoadTexture(animated_sprite->GetStateFilePath(AnimatedSpriteComponent::PlayerSpriteState::Idle));

	// Definir le nombre de frames pour l'etat Idle
	animated_sprite->SetStateFrameCount(AnimatedSpriteComponent::PlayerSpriteState::Idle, 17);

	// Definir l'etat actuel a Idle
	animated_sprite->state = AnimatedSpriteComponent::PlayerSpriteState::Idle;

	// Calcule l'echelle en fonction de la taille du gameObject et du sprite (_size *taille du gameObject* / _sprite *taille du sprite*)
	float scaleX = (_size.x / (animated_sprite->GetSprite()->getTextureRect().width / animated_sprite->GetFrameCount()));
	float scaleY = (_size.y / (animated_sprite->GetSprite()->getTextureRect().height));

	// Definis l'echelle du sprite
	animated_sprite->GetSprite()->setScale(scaleX, scaleY);
	animated_sprite->SetDefaultScale(scaleX, scaleY);

	if (_speed < 0)
	{
		animated_sprite->SetDirection(AnimatedSpriteComponent::MovementDirection::Left);
	}
	else
	{
		animated_sprite->SetDirection(AnimatedSpriteComponent::MovementDirection::Right);
	}


	// Cree un Projectile pour le GameObject.
	Projectile* projectile = game_object->CreateComponent<Projectile>();
	projectile->SetSpeed(_speed);
	projectile->SetDamage(_damage);

	// Configure le GameObject pour qu'il appartienne a cette scene et ait les memes arriere-plans que cette scene.
	game_object->SetScene(this);
	game_object->SetBackgrounds(backgrounds);

	id++;

	return game_object;
}

// Methode pour creer les arriere-plans de la scene.
void Scene::CreateBackgrounds(const std::string& _path)
{
	for (int i = 0; i < 2; i++)
	{
		Background* const background = new Background();
		background->SetPath(_path, i);
		SetBackgroundSize(background, 1600.0f, 900.0f);

		if (i == 0)
		{
			background->SetPosition(0, 0);
		}
		else
		{
			background->SetPosition(1600.0f, 0);
		}

		backgrounds.push_back(background);
	}
}

// Methode pour definir la taille d'un arriere-plan.
void Scene::SetBackgroundSize(Background* background, float _width, float _height)
{
	background->SetSize(_width, _height);
}

// Methode pour detruire un GameObject de la scene.
void Scene::DestroyGameObject(const GameObject* _game_object)
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (*it == _game_object)
		{
			gameObjects.erase(it);
			delete _game_object;
			return;
		}
	}
}

// Methode pour trouver un GameObject par son nom.
GameObject* Scene::FindGameObject(const std::string& _name) const
{
	for (GameObject* const& game_object : gameObjects)
	{
		if (game_object->GetName() == _name)
		{
			return game_object;
		}
	}
	return nullptr;
}

// Methode pour trouver un GameObject par son type.
GameObject* Scene::FindGameObjectType(const std::string& _type) const
{
	for (GameObject* const& game_object : gameObjects)
	{
		if (game_object->GetType() == _type)
		{
			return game_object;
		}
	}
	return nullptr;
}

// Methode pour obtenir tous les GameObjects de la scene.
const std::vector<GameObject*>& Scene::GetGameObjects() const
{
	return gameObjects;
}
