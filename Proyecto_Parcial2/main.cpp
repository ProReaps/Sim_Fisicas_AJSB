#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//Scale set to 10, the window renders things at a 1:10 scale
constexpr float SCALE = 1.0f;

//These two variables are used to change the framerate the window is drawn in
//particularly the time it waits after drawing the window
//Base time
int timeJumps = 160;
//Multiplier for base time, we control this in the window drawing cycle
int timeMultiplier = 5;

//Class used to create bodies with square shapes
class Cube {
public:
    //x, y used for starting position
    //Size and color parameters to further customize each cube
    //'sizeX' and 'sizeY' is given in the context of the window, just as x and y are. We use the variable 'SCALE'
    //to translate it to box2D units
    //'bodyType' stores either a 0,1 or 2.
    Cube(b2World &world, float x, float y, float sizeX, float sizeY, sf::Color color, int bodyType, float density, float RotAngle) {
        //assign variables
        size_x = sizeX;
        size_y = sizeY;
        b2BodyDef bodydef;
        bodydef.position.Set(x / SCALE, y / SCALE);

        //Switch on bodytype
        switch(bodyType)
        {
            case 1:
                bodydef.type = b2_staticBody;
                //it's supposed to be a static body
                break;
            case 2:
                bodydef.type = b2_kinematicBody;
                //it's supposed to be a kinematic body
                break;
            case 3:
                bodydef.type = b2_dynamicBody;
                //it's supposed to be a dynamic body
                break;
            default:
                bodydef.type = b2_staticBody;
                //Default case
                break;
        }
        bodydef.angle = RotAngle * M_PI/180;
        body = world.CreateBody(&bodydef);

        b2PolygonShape shape;
        //this used to be divided by two, this makes more sense to me as we are translating the size the box will have
        //in the window directly into box2D units
        shape.SetAsBox((sizeX/2) / SCALE, (sizeY/2) / SCALE);

        b2FixtureDef fixturedef;
        fixturedef.shape = &shape;

        if (bodyType != 1) {
            fixturedef.density = density;
        }

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

//Class used to create bodies with circular shapes
class Circle {
public:
    //x, y used for starting position
    //Radius and color parameters to further customize each circle
    //'Radius' is given in the context of the window, just as x and y are. We use the variable 'SCALE'
    //to translate it to box2D units
    //'is_Static' is used to create static bodies instead of dynamic ones
    Circle(b2World &world, float x, float y, float Radius, sf::Color color, int bodyType, float density){
        radius = Radius;

        b2BodyDef bodydef;
        bodydef.position.Set(x / SCALE, y / SCALE);

        //Switch on bodytype
        switch(bodyType)
        {
            case 1:
                bodydef.type = b2_staticBody;
                //it's supposed to be a static body
                break;
            case 2:
                bodydef.type = b2_kinematicBody;
                //it's supposed to be a kinematic body
                break;
            case 3:
                bodydef.type = b2_dynamicBody;
                //it's supposed to be a dynamic body
                break;
            default:
                bodydef.type = b2_staticBody;
                //Default case
                break;
        }
        body = world.CreateBody(&bodydef);

        b2CircleShape shape;
        shape.m_radius = Radius/SCALE;
        shape.m_p.Set(0,0);

        b2FixtureDef fixturedef;
        fixturedef.shape = &shape;
        fixturedef.density = density;
        fixturedef.friction = 3.0f;
        body->CreateFixture(&fixturedef);

        ball.setRadius(Radius);
        ball.setOrigin(Radius, Radius);
        ball.setFillColor(color);
    }
    void update(){
        ball.setPosition(SCALE*this->body->GetPosition().x, SCALE*this->body->GetPosition().y);
    }
    void draw(sf::RenderWindow &win) const{
        win.draw(ball);
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
    sf::CircleShape ball;
    b2Body *body;
    float radius;
};

//For later, when we want to add one to timeMults
void add_1_TimeMults(){
    timeMultiplier = timeMultiplier+1;
}

//For later, when we want to subtract one from timeMults, ONLY IF it's greater than one
void sub_1_TimeMults(){
    if (timeMultiplier>1)
    {
        timeMultiplier = timeMultiplier-1;
    }
}

int main() {
    //Window and world creation
    sf::RenderWindow window(sf::VideoMode(1000, 900), "SFML works!");
    b2Vec2 gravity(0.0f, 9.8f);
    b2World world(gravity);

    //Create the bodies using the cube and circle classes, syntax being:
    //  - Static body (cube)       - SCU_Name
    //  - Static body (circle)     - SCI_Name

    //  - Kinematic body (cube)    - KCU_Name
    //  - Kinematic body (circle)  - KCI_Name

    //  - Dynamic body (cube)      - DCU_Name
    //  - Dynamic body (circle)    - DCI_Name

    //Offset to move the entire machine
    float x_offset =0;
    float y_offset =150;

    //We create the static bodies that'll be part of the machine's structure (static walls and floors for the machine)
    #pragma region Static_Body_Creation //////
    //Start ramp
    Cube SCU_Stage1(world, x_offset-2.5f, y_offset+205, 300, 10, sf::Color::White, 2,1, 30);
    //Walls
    Cube SCU_Stage2(world, x_offset+125, y_offset+375, 10, 200, sf::Color::White, 2, 1, 0);
    Cube SCU_Stage3(world, x_offset+200, y_offset+375, 10, 200, sf::Color::White, 2, 1, 0);
    Cube SCU_Stage4(world, x_offset+275, y_offset+375, 10, 200, sf::Color::White, 2, 1, 0);
    //Floor for the walls, just so it looks neat
    Cube SCU_Stage5(world, x_offset+200, y_offset+480, 180, 10, sf::Color::White, 2, 1, 0);
    //Flat runway after pulley joint
    Cube SCU_Stage6(world, x_offset+375, y_offset+280, 200, 10, sf::Color::White, 2, 1, 0);
    //Wall that will stop the advance of the distance/weld joint
    Cube SCU_Stage7(world, x_offset+475, y_offset+375, 10, 200, sf::Color::White, 2, 1, 0);
    //Area where the second ball will be propelled
    Cube SCU_Stage8(world, x_offset+630, y_offset+480, 340, 10, sf::Color::White, 2, 1, 0);
    //Final ramp
    Cube SCU_Stage9(world, x_offset+915, y_offset+405, 300, 10, sf::Color::White, 2,1, 330);
    //Something to make it look like this is on a table
    Cube SCU_Stage10(world, x_offset+500, y_offset+515, 1000, 60, sf::Color::White, 2, 1, 0);

    //Update the static walls and floors for the machine
    //We update all the static bodies declared here to avoid doing so in every cycle
    SCU_Stage1.update();
    SCU_Stage2.update();
    SCU_Stage3.update();
    SCU_Stage4.update();
    SCU_Stage5.update();
    SCU_Stage6.update();
    SCU_Stage7.update();
    SCU_Stage8.update();
    SCU_Stage9.update();
    SCU_Stage10.update();
#pragma endregion

    //Create dynamic box
    Cube DCU_Try(world, x_offset+10, y_offset+0, 20, 20, sf::Color::White, 3, 2, 0);

    //Create the dynamic circles that are used as the movable objects that interact with others in the machine
    Circle DCI_PelotaInicio(world, x_offset+50, y_offset+100, 15, sf::Color::Red, 3, 4);
    Circle DCI_PelotaInicio2(world, x_offset+50, y_offset-100, 10, sf::Color::Red, 3, 2);

    //Window loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            //This makes our window wait less
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                sub_1_TimeMults();
            }
            //This makes our window wait for longer
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                add_1_TimeMults();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {

            }
            // The Z key event to close the window
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                window.close();
                return 0; // return from the main function, effectively ending the program
            }
        }
        world.Step(1 / 60.f, 8, 3);

        DCU_Try.update();

        //Update the dynamic circles that are used as the movable objects that interact with others in the machine
        DCI_PelotaInicio.update();
        DCI_PelotaInicio2.update();

        //Clear everything then draw it again
        window.clear();

        DCU_Try.draw(window);

        //Draw the interacting circles
        DCI_PelotaInicio.draw(window);
        DCI_PelotaInicio2.draw(window);

        window.display();

        //Wait for 50 milliseconds every time the window is drawn to
        std::this_thread::sleep_for(std::chrono::nanoseconds(timeJumps*timeMultiplier));
    }
    return 0;
}
