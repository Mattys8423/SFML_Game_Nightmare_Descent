// MainMenuScene.h
#pragma once
#include "Scene.h"

class MainMenuScene : public Scene
{
public:
    MainMenuScene() : Scene("MainMenuScene")
    {
        // Cr�ez ici les �l�ments de votre menu principal.
        // Par exemple, vous pouvez cr�er des GameObjects pour les boutons "Jouer", "Options", "Quitter", etc.
        // Vous pouvez �galement ajouter des �couteurs d'�v�nements � ces boutons pour changer de sc�ne lorsque l'utilisateur clique sur eux.
    }
};

