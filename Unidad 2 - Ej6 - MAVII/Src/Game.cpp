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
    suelo.setFillColor(sf::Color::Blue);
    suelo.setPosition(0, 95);
    wnd->draw(suelo);

    //Dibujar el techo
    sf::RectangleShape techo(sf::Vector2f(500, 5));
    techo.setFillColor(sf::Color::Blue);
    techo.setPosition(0, 0);
    wnd->draw(techo);

    //Dibujar pared izquierda
    sf::RectangleShape paredIzq(sf::Vector2f(500, 5));
    paredIzq.setFillColor(sf::Color::Blue);
    paredIzq.setRotation(90);
    paredIzq.setPosition(5, 0);
    wnd->draw(paredIzq);

    //Dibujar pared izquierda
    sf::RectangleShape paredDer(sf::Vector2f(500, 5));
    paredDer.setFillColor(sf::Color::Blue);
    paredDer.setRotation(90);
    paredDer.setPosition(100, 0);
    wnd->draw(paredDer);

    //Dibujar base
    base = RectangleShape(Vector2f(2, 2));
    base.setFillColor(Color(128, 128, 128));
    base.setPosition(7.0f, 50.0f);
    wnd->draw(base);

}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
    sf::Vector2i mousePosPixel = sf::Mouse::getPosition(*wnd);
    sf::Vector2f mouseWorld = wnd->mapPixelToCoords(mousePosPixel);

    b2Vec2 pivotPos(base.getPosition().x + base.getSize().x / 2.f, base.getPosition().y + base.getSize().x / 2.f);

    float dx = mouseWorld.x - pivotPos.x;
    float dy = mouseWorld.y - pivotPos.y;
    float angle = std::atan2(dy, dx);

    b2Vec2 offsetCanon(cos(angle) * 8.f, sin(angle) * 8.0f);
    b2Vec2 posCanon = pivotPos + offsetCanon;
    canonBody->SetTransform(posCanon, angle);

    b2Vec2 offset(cos(angle) * 11.0f, sin(angle) * 11.0f);
    Vector2f respawnPos = Vector2f(canonBody->GetPosition().x + offset.x, canonBody->GetPosition().y + offset.y);

    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close();
            break;
        case Event::MouseButtonPressed:
            if (evt.mouseButton.button == sf::Mouse::Left) {
                b2Vec2 spawnPos(respawnPos.x , respawnPos.y );

                balasBody = Box2DHelper::Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 2, 1.0f, 0.3f, 0.5f);
                balasBody->SetTransform(spawnPos, angle);

                // Velocidad en la dirección del ángulo
                float fuerza = 50.0f;
                b2Vec2 direccion(std::cos(angle) * fuerza, std::sin(angle) * fuerza);
                balasBody->SetLinearVelocity(direccion);
            }

        }

    }    
}

void Game::CheckCollitions(){}

// Configuración de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tamaño de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

void Game::InitPhysics()
{
    // Inicializar el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo 
    sueloBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    sueloBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    // Crear el techo
    techoBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    techoBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);
    
    // Crear paredIzq
    paredIzqBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    paredIzqBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    // Crear paredDer
    paredDerBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    paredDerBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    // Crear canon
    canonBody = Box2DHelper::Box2DHelper::CreateRectangularDynamicBody(phyWorld, 20, 4,0.0,0.0,0.0);
    canonBody->SetTransform(b2Vec2(5.0f, 50.0f), 0.0f);

}

// Destructor de la clase

Game::~Game(void)
{ }