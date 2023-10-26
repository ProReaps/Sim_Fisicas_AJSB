#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

constexpr float SCALE = 10.0f;

class Cube {
public:
    //added size and color parameters to be able to create bodies that look different, and so I don't get an aneurysm
    //trying to figure out what happens with the simulation, also helpful for you!

    //'sizeX' and 'sizeY' is given in the context of the window, just as x and y are. We use the variable 'SCALE'
    //to translate it to box2D units
    Cube(b2World &world, float x, float y, float sizeX, float sizeY, sf::Color color) {
        size_x = sizeX;

        b2BodyDef bodydef;
        bodydef.position.Set(x / SCALE, y / SCALE);
        bodydef.type = b2_dynamicBody;
        body = world.CreateBody(&bodydef);

        b2PolygonShape shape;
        //this used to be divided by two, however, this way makes more sense to me as we are scaling the size
        //variables into their equivalent box2D units
        shape.SetAsBox((sizeX/2) / SCALE, (sizeY/2) / SCALE);

        b2FixtureDef fixturedef;
        fixturedef.shape = &shape;
        fixturedef.density = 1.0f;
        body->CreateFixture(&fixturedef);

        //size depends on size obviously, this is very direct, no need to scale it since it's in the
        //context of the window already
        box.setSize(sf::Vector2f(sizeX, sizeY));
        box.setFillColor(color);
        //point of origin depends on size
        box.setOrigin(sizeX/2, sizeY/2);
    }
    void update() {
        box.setPosition(SCALE * body->GetPosition().x, SCALE * body->GetPosition().y);
        box.setRotation(body->GetAngle() * 180 / b2_pi);

    }
    void draw(sf::RenderWindow &win) const {
        win.draw(box);
    }
    void resetPosition() {
        body->SetTransform(b2Vec2(0, 0), body->GetAngle());
    }
    void stop() {
        body->SetLinearVelocity(b2Vec2(0, 0));
    }
    b2Body *getBody() {
        return body;
    }
    float getSizeX(){
        return size_x;
    }
private:
    sf::RectangleShape box;
    b2Body *body;
    float size_x;
};


int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 900), "SFML works!");

    b2Vec2 gravity(0.0f, 0.01f);
    b2World world(gravity);

    //Create the cubes from Cube class
    Cube cube(world, 350, 290, 10.0f, 50.0f, sf::Color::Red);
    Cube cube2(world, 200, 300, 60.0f, 40.0f, sf::Color::Blue);

    //This is the floor, we set its type as a static body
    Cube floor(world, -50000, 750, 500000, 150, sf::Color::Green);
    floor.getBody()->SetType(b2_staticBody);

    //Declare the joint definition variable (maybe that is what this is, could be wrong)
    b2PrismaticJointDef jointDef;

    b2Vec2 worldAxis(0.0f, 1.0f);

    jointDef.Initialize(cube.getBody(), cube2.getBody(), cube.getBody()->GetWorldCenter(), worldAxis);

    //Define main parameters
    jointDef.localAxisA.Set(0,1);
    jointDef.upperTranslation = 5.0f;
    jointDef.lowerTranslation = 1.25f;
    jointDef.enableLimit = true;
    jointDef.collideConnected = false;
    jointDef.localAnchorA.Set(-(cube.getSizeX()/(SCALE*2)), -2.3f);
    jointDef.localAnchorB.Set(cube2.getSizeX()/(SCALE*2)+1.0f, -1.9f);

    //Other important parameters, trying to turn the joint into a motor
    jointDef.maxMotorForce = 1.0f;
    jointDef.motorSpeed = 0.0f;
    jointDef.enableMotor = true;

    //Actually create the joint in world space
    b2PrismaticJoint* joint = (b2PrismaticJoint*)world.CreateJoint(&jointDef);

    //Window loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            //Move body B right
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                //cube2.getBody()->ApplyForceToCenter(b2Vec2(5.0f, 0.0f), true);
                cube2.getBody()->SetLinearVelocity(b2Vec2(0.5f,0.0f));
            }
            //Move body B left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                //cube2.getBody()->ApplyForceToCenter(b2Vec2(-5.0f, 0.0f), true);
                cube2.getBody()->SetLinearVelocity(b2Vec2(-0.5f,0.0f));
            }
            //Move body A up
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                joint->SetMotorSpeed(0.25f);
            }
            //Move body A down
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                joint->SetMotorSpeed(-0.25f);
            }
            //Set body A speed to 0, 0
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                cube2.getBody()->SetLinearVelocity(b2Vec2(0.0f,0.0f));
            }
            //Set motor speed to 0
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                joint->SetMotorSpeed(0.0f);
            }
            // The Z key event to close the window
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                window.close();
                world.DestroyJoint(joint);
                joint = nullptr;
                return 0; // return from the main function, effectively ending the program
            }
        }
        world.Step(1 / 60.f, 8, 3);
        floor.update();
        cube2.update();
        cube.update();

        //Clear everything then draw it again
        window.clear();
        //Draw the floor first
        floor.draw(window);
        cube2.draw(window);
        cube.draw(window);
        
        window.display();
    }
    world.DestroyJoint(joint);
    joint = nullptr;

    return 0;
}