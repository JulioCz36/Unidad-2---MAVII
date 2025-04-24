#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicializaci�n de la ventana y configuraci�n de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuraci�n de la vista del juego
    InitPhysics(); // Inicializaci�n del motor de f�sica
}

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        CheckCollitions(); // Comprobar colisiones
        UpdatePhysics(); // Actualizar la simulaci�n f�sica
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }
}

// Actualizaci�n de la simulaci�n f�sica
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo f�sico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo f�sico para depuraci�n
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
    // Dibujar el plano
    sf::RectangleShape plano(sf::Vector2f(100, 10));
    plano.setFillColor(sf::Color::Blue);
    plano.setOrigin(plano.getSize().x / 2, plano.getSize().y / 2);
    plano.setPosition(planoBody->GetPosition().x, planoBody->GetPosition().y);
    wnd->draw(plano);

}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close(); // Cerrar la ventana si se presiona el bot�n de cerrar
            break;
        }
    }

    float fuerza = 1000.0f;
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        cajaBody->ApplyForceToCenter(b2Vec2(-fuerza, 0.0f), true);
    }

    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        cajaBody->ApplyForceToCenter(b2Vec2(fuerza, 0.0f), true);
    }
}

// Comprobaci�n de colisiones (a implementar m�s adelante)
void Game::CheckCollitions()
{
    // Implementaci�n de la comprobaci�n de colisiones
}

// Configuraci�n de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tama�o de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicializaci�n del motor de f�sica y los cuerpos del mundo f�sico
void Game::InitPhysics()
{
    // Inicializar el mundo f�sico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo f�sico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // |Superficies en contacto  |  Coeficiente de rozamiento|
   // |   acero sobre acero     |           0,6             |
   
    // Crear plano
    planoBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 100, 10, 0.0f, friccion, 0.0);
    planoBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    
    // Crear una caja
    cajaBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 15.0f, 10.0f, 0.3f, 0.6f, 0.0);
    cajaBody->SetTransform(b2Vec2(50.0f, 90.0f), 0.0f);
}

// Destructor de la clase

Game::~Game(void)
{ }