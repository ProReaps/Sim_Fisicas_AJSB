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
        size_y = sizeY;

        b2BodyDef bodydef;
        bodydef.position.Set(x / SCALE, y / SCALE);
        bodydef.type = b2_dynamicBody;
        body = world.CreateBody(&bodydef);

        b2PolygonShape shape;
        //this used to be divided by two, this makes more sense to me as we are translating the size the box will have
        //in the window directly into box2D units
        shape.SetAsBox((sizeX/2) / SCALE, (sizeY/2) / SCALE);

        b2FixtureDef fixturedef;
        fixturedef.shape = &shape;
        fixturedef.density = 1.0f;
        fixturedef.friction = 3.0f;
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
    float getSizeX() const{
        return size_x;
    }
    float getSizeY() const{
        return size_y;
    }
private:
    sf::RectangleShape box;
    b2Body *body;
    float size_x;
    float size_y;
};
class Circle {
public:
    Circle(b2World &world, float x, float y, float Radius, sf::Color color){
        radius = Radius;

        b2BodyDef bodydef;
        bodydef.position.Set(x / SCALE, y / SCALE);
        bodydef.type = b2_dynamicBody;
        body = world.CreateBody(&bodydef);

        b2CircleShape shape;
        shape.m_radius = Radius/SCALE*2;
        shape.m_p.Set(0,0);

        b2FixtureDef fixturedef;
        fixturedef.shape = &shape;
        fixturedef.density = 1.0f;
        fixturedef.friction = 3.0f;
        body->CreateFixture(&fixturedef);

        circ.setRadius(Radius);
        circ.setFillColor(color);
    }
    void update(){
        circ.setPosition(SCALE*body->GetPosition().x, SCALE*body->GetPosition().y);
    }
    void draw(sf::RenderWindow &win) const{
        win.draw(circ);
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
    float getRadius() const{
        return radius;
    }
private:
    sf::CircleShape circ;
    b2Body *body;
    float size_x;
    float size_y;
    float radius;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 900), "SFML works!");

    b2Vec2 gravity(0.0f, 0.1f);
    b2World world(gravity);

    //Create the cubes from Cube class
    Circle circle(world, 375, 350, 12.5f, sf::Color::Red); //This one is the wheel
    Cube cube2(world, 400, 325, 40.0f, 25.0f, sf::Color::Blue); //This one is NOT a wheel

    //This is the floor, we set the type as a static body
    Cube floor(world, -50000, 750, 500000, 150, sf::Color::Green);
    floor.getBody()->SetType(b2_staticBody);

    //Set this one as a bit of a static one? We're just restricting rotation, not movement
    cube2.getBody()->SetFixedRotation(true);

    //Declare the joint definition variable (maybe that is what this is, could be wrong)
    b2WheelJointDef jointDef;

    //Define main parameters
    jointDef.bodyA = circle.getBody();
    jointDef.bodyB = cube2.getBody();

    //Set the anchor for bodyA, this will be where it rotates around
    jointDef.localAnchorA.Set(0, 0);
    //since it locally rotates around (0, 0), it'll rotate around its own center

    //Set the anchor for bodyB, this will be where the wheel/bodyA is at (I think)
    jointDef.localAnchorB.Set(-circle.getRadius()/(SCALE*2), (circle.getRadius()*2.5f)/(SCALE));

    //The movement axis for bodyA, it gets constrained to this spot with an upper and lower range
    jointDef.localAxisA.Set(0.0f, 1.0f);

    //It's a motor! maybe...
    jointDef.maxMotorTorque = 3.0f;
    jointDef.motorSpeed = 0.0f;
    jointDef.enableMotor = true;

    //This is for the wheel, the upper and lower range for its movement axis restrict.
    //It can move from -1 * localAxisA.y (lower) to  1 * localAxisA.y (upper)
    jointDef.lowerTranslation = 0;
    jointDef.upperTranslation = 0;
    jointDef.enableLimit = true;

    //I do not want to activate collisions for the connected bodies, since their shapes are squares there
    //will be points where they overlap (mostly while rotating)
    jointDef.collideConnected = false;
    jointDef.stiffness = 10.0f;

    //Actually create the joint in world space
    b2WheelJoint* joint = (b2WheelJoint*)world.CreateJoint(&jointDef);

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

            }
            //Reset position to 0,0
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                circle.resetPosition();
                joint->SetMotorSpeed(0.0f*b2_pi);
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
        circle.update();

        //Clear everything then draw it again
        window.clear();

        cube2.draw(window);
        circle.draw(window);

        window.display();
    }
    world.DestroyJoint(joint);
    joint = nullptr;

    return 0;
}