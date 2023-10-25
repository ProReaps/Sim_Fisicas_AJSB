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
        //this used to be divided by two, this makes more sense to me as we are translating the size the box will have
        //in the window directly into box2D units
        shape.SetAsBox((sizeX) / SCALE, (sizeY) / SCALE);

        b2FixtureDef fixturedef;
        fixturedef.shape = &shape;
        fixturedef.density = 1.0f;
        body->CreateFixture(&fixturedef);

        //size depends on size, obviously
        box.setSize(sf::Vector2f(sizeX, sizeY));
        box.setFillColor(color);
        //point of origin depends on size
        box.setOrigin(sizeX/2, sizeY/2);
    }
    void update() {
        box.setPosition(SCALE * body->GetPosition().x, SCALE * body->GetPosition().y);
        box.setRotation(body->GetAngle() * 180 / b2_pi);

        /* This part kind of makes it break
        //  Make it loop (higher threshold)
        //      x value of position vector is more than window width + an offset value, so it resets to 0
        if((SCALE*body->GetPosition().x)>(1000+OFFS))
        {
            body->SetTransform(b2Vec2(0, body->GetPosition().y),body->GetAngle());
        }
        //      y value of position vector is more than window height + an offset value, so it resets to 0
        if((SCALE*body->GetPosition().y)>(900+OFFS))
        {
            body->SetTransform(b2Vec2(body->GetPosition().x, 0),body->GetAngle());
        }

        //  Make it loop (lower threshold)
        //      x value of position vector is less than 0 - an offset value, so it resets to window width
        if((SCALE*body->GetPosition().x)<(0-OFFS))
        {
            body->SetTransform(b2Vec2(1000/SCALE, body->GetPosition().y),body->GetAngle());
        }
        //      y value of position vector is less than 0 - an offset value, so it resets to window height
        if((SCALE*body->GetPosition().y)<(0-OFFS))
        {
            body->SetTransform(b2Vec2(body->GetPosition().x, 900/SCALE),body->GetAngle());
        }
        //  In both cases, the offset value helps to only reset it once, since the 'bounds' are farther away
        //  than the points at which it reappears
         */
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

    //Visual representation of the center of rotation, very purple, much center
    sf::RectangleShape aa;
    //size depends on size, obviously
    aa.setSize(sf::Vector2f(5, 5));
    aa.setFillColor(sf::Color::Magenta);
    //point of origin depends on size
    aa.setOrigin(2.5f, 2.5f);
    aa.setPosition(400,300);

    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);

    //Create the cubes from Cube class
    Cube cube(world, 400, 300, 10.0f, 10.0f, sf::Color::Red);
    Cube cuboDos(world, 400, 300, 40.0f, 10.0f, sf::Color::Blue);

    //Declare the joint definition variable (maybe that is what this is, could be wrong)
    b2RevoluteJointDef jointDef;

    //Define main parameters
    jointDef.bodyA = cuboDos.getBody();
    jointDef.bodyB = cube.getBody();

    //Set the anchor for bodyA, this will be where it rotates around
    jointDef.localAnchorA.Set(0, 0);

    //Don't want it to collide with itself (in this case)
    jointDef.collideConnected = false;

    //Other important parameters, trying to turn the joint into a motor
    jointDef.maxMotorTorque = 10.0f;
    jointDef.motorSpeed = 0.1f*b2_pi;
    jointDef.enableMotor = true;

    //Actually create the joint in world space
    b2RevoluteJoint* joint = (b2RevoluteJoint*)world.CreateJoint(&jointDef);

    //Window loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                joint->SetMotorSpeed(joint->GetMotorSpeed()-(0.1f*b2_pi));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                joint->SetMotorSpeed(joint->GetMotorSpeed()+(0.1f*b2_pi));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                joint->SetMotorSpeed(0.1f*b2_pi);
                joint->EnableMotor(true);

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                joint->SetMotorSpeed(0.0f*b2_pi);
                joint->EnableMotor(false);
            }
            // The Z key event to close the window
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                window.close();
                return 0; // return from the main function, effectively ending the program
            }
        }
        world.Step(1 / 60.f, 8, 3);
        cuboDos.update();
        cube.update();

        //Clear everything then draw it again
        window.clear();
        cuboDos.draw(window);
        cube.draw(window);

        //Drawable for the center of rotation
        window.draw(aa);
        
        window.display();
    }
    world.DestroyJoint(joint);
    joint = nullptr;

    return 0;
}