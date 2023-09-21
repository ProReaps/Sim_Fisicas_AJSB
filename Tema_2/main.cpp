#include <iostream>
#include <Box2d/Box2d.h>
//Descripcion:
//Tiro parabólico

int main(){
    //Mundo
    //Creando el vector de gravedad y el mundo
    //En este caso la gravedad es -24.79f porque está basado en las condiciones de jupiter
    b2Vec2 gravity(0.0f, -9.81f);
    b2World world(gravity);


    //Piso (no cambié nada del código de la clase en la sección de piso)
    //Características del cuerpo
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    //Creación del cuerpo
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    //Crear la forma del cuerpo
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50,1);

    //Se crea el fixture
    groundBody->CreateFixture(&groundBox, 0.0f);


    //Cuerpo dinamico (caja)
    //Características de la caja
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f,15.0f);   //cambia la altura a la dada para el ejercicio

    //Creacion del cuerpo
    b2Body* body = world.CreateBody(&bodyDef);

    //Crear la forma del cuerpo
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f,1.0f);

    //Hacemos una fixture def aparte para cambiar cada característica del cuerpo
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 15600.0f;  //densidad cambiada a la del carburo de tungsteno
    //fricción del aire (inventada pero mayor)
    fixtureDef.friction = 0.9f;

    //Cargamos el fixtureDef que creamos
    body->CreateFixture(&fixtureDef);


    //Caida
    float timeStep = 1.0f/60;

    int32 velocityIterations = 6;
    int32 positionIteration = 2;

    //Tiempo ajustado a 1 segundo de simulación
    for(int32 i=0; i<60; ++i)
    {
        world.Step(timeStep, velocityIterations, positionIteration);
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f \n",position.x, position.y, angle);
    }
}
