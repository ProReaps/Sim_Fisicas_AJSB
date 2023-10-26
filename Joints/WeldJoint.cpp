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


int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 900), "SFML works!");

    b2Vec2 gravity(0.0f, 0.05f);
    b2World world(gravity);

    //Floor body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 75.0f);
    //Create the floor body
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    //Create the shape
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50000,15.0f);
    //Create the fixture, so it is finally in the simulation world
    groundBody->CreateFixture(&groundBox, 0.0f);

    //Create the cubes from Cube class, here they're touching and given the different sizes and masses they should
    //fall slightly sideways, giving ample opportunity to see the weld joint in effect
    Cube cube(world, 300, 300, 20.0f, 50.0f, sf::Color::Red);
    Cube cube2(world, 310, 300, 50.0f, 40.0f, sf::Color::Blue);

    //Declare the joint definition variable (I am now almost sure of this, could this be character development?)
    b2WeldJointDef jointDef;

    //Initialize the joint, then set the anchors for both bodies, should be at about the midway point in y and
    //right up against each other in x
    jointDef.Initialize(cube.getBody(), cube2.getBody(), b2Vec2(cube.getBody()->GetWorldCenter().x+(cube.getSizeX()/2),cube.getBody()->GetWorldCenter().x));
    jointDef.localAnchorA.Set(cube.getSizeX()/(SCALE*2), 0);
    jointDef.localAnchorB.Set(-cube2.getSizeX()/(SCALE*2), 0);

    //Actually create the joint in world space
    b2WeldJoint* joint = (b2WeldJoint*)world.CreateJoint(&jointDef);

    //Window loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            //Move body B right
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                cube2.getBody()->SetLinearVelocity(b2Vec2(0.5f,0.0f));
            }
            //Move body B left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                cube2.getBody()->SetLinearVelocity(b2Vec2(-0.5f, 0.0f));
            }
            //Move body A up
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                cube.getBody()->SetLinearVelocity(b2Vec2(0.0f,-0.5f));
            }
            //Move body A down
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                cube2.getBody()->SetLinearVelocity(b2Vec2(0.0f,0.5f));
            }
            //Set body A speed to 0, 0
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                cube.getBody()->SetLinearVelocity(b2Vec2(0.0f,0.0f));
            }
            //Set motor speed to 0
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                cube.resetPosition();
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
        cube2.update();
        cube.update();

        //Clear everything then draw it again
        window.clear();
        cube2.draw(window);
        cube.draw(window);
        
        window.display();
    }
    world.DestroyJoint(joint);
    joint = nullptr;

    return 0;
}