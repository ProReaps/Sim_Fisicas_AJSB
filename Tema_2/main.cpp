#include <Box2D/Box2D.h>
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//Descripción:
//Tiro de una bala de cañon, la densidad la ponemos nosotros, a una velocidad de 900 hm/h
//a un ángulo de 60 grados, de un tamaño promedio
int main() {
    // Definimos la gravedad con el valor que tendría en la luna
    b2Vec2 gravity(0.0f, -1.62f);

    // Construct a world object, which will hold and simulate the rigid bodies
    b2World world(gravity);

    //Creación de un cuerpo dinámico
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 0.0f);
    b2Body* body = world.CreateBody(&bodyDef);

    //Definimos una forma para la bola de cañon, en este caso de 30cm (o 0.3m) que fue
    //el diametro que encontré en google.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.3f, 0.3f);

    // Define the dynamic body fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    //asignando la densidad del hierro al cuerpo
    fixtureDef.density = 7874.0f;
    //Sin friccion porque estamos en la luna
    //fixtureDef.friction = 0.0f;

    // Add the shape to the body
    body->CreateFixture(&fixtureDef);

    // Set the initial velocity of the bullet
    float angle = 60.0f; // Angle in degrees
    float speed = 250.0f; // Equivalente de 900km/h en m/s
    float vx = speed * std::cos(angle * M_PI / 180.0f);
    float vy = speed * std::sin(angle * M_PI / 180.0f);

    body->SetLinearVelocity(b2Vec2(vx, vy));

    // Simulate the world
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    for (int i = 0; i < 300; ++i)
    {
        world.Step(timeStep, velocityIterations, positionIterations);
        std::cout << body->GetPosition().x << " " << body->GetPosition().y << std::endl;
    }

    // Remember to clean up
    world.DestroyBody(body);

    return 0;
}