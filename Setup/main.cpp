#include <iostream>
#include <Box2d/Box2d.h>

int main(){ //cubo callendo en la superficie de la luna
    //Mundo
    //Creando el vector de gravedad y el mundo
    b2Vec2 gravity(0.0f, -9.81f);
    b2World world(gravity);


    //Piso
    //Características del cuerpo
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    //Creación del cuerpo
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    //Crear la forma del cuerpo
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50,1);

    //
    groundBody->CreateFixture(&groundBox, 0.0f);


    //Cuerpo dinamico (caja)
    //Características de la caja
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f,20.0f);

    //Creacion del cuerpo
    b2Body* body = world.CreateBody(&bodyDef);

    //Crear la forma del cuerpo
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f,1.0f);

    //Hacemos una fixture def aparte para cambiar cada característica del cuerpo
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    //Cargamos el fixtureDef que creamos
    body->CreateFixture(&fixtureDef);


    //Caida
    float timeStep = 1.0f/60.0f;

    int32 velocityIterations = 6;
    int32 positionIteration = 2;

    for(int32 i=0; i<180; ++i)
    {
        world.Step(timeStep, velocityIterations, positionIteration);
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f \n",position.x, position.y, angle);
    }
}
