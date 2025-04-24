#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicialización de la ventana y configuración de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuración de la vista del juego
    InitPhysics(); // Inicialización del motor de física
}

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        CheckCollitions(); // Comprobar colisiones
        UpdatePhysics(); // Actualizar la simulación física
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }
}

// Actualización de la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo físico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo físico para depuración
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
    // Dibujar el suelo
    sf::RectangleShape suelo(sf::Vector2f(500, 5));
    suelo.setFillColor(sf::Color::Red);
    suelo.setPosition(0, 95);
    wnd->draw(suelo);

    //Dibujar el techo
    sf::RectangleShape techo(sf::Vector2f(500, 5));
    techo.setFillColor(sf::Color::Red);
    techo.setPosition(0, 0);
    wnd->draw(techo);

    //Dibujar pared izquierda
    sf::RectangleShape paredIzq(sf::Vector2f(500, 5));
    paredIzq.setFillColor(sf::Color::Red);
    paredIzq.setRotation(90);
    paredIzq.setPosition(5, 0);
    wnd->draw(paredIzq);
    //Dibujar pared izquierda
    sf::RectangleShape paredDer(sf::Vector2f(500, 5));
    paredDer.setFillColor(sf::Color::Red);
    paredDer.setRotation(90);
    paredDer.setPosition(100, 0);
    wnd->draw(paredDer);

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
            wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
            break;
        }
    }
}

// Comprobación de colisiones (a implementar más adelante)
void Game::CheckCollitions()
{
    // Implementación de la comprobación de colisiones
}

// Configuración de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tamaño de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics()
{
    // Inicializar el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo 
    b2Body* sueloBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    sueloBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);


    // Crear el techo
    b2Body* techoBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    techoBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);
    
    // Crear paredIzq
    b2Body* paredIzqBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    paredIzqBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    // Crear paredDer
    b2Body* paredDerBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    paredDerBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);


    // Crear una pelota
    pelotaBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5.0f, 0.2f, 0.1, 1.5);
    pelotaBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);
}

// Destructor de la clase

Game::~Game(void)
{ }