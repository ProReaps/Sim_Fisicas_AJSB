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
    //  - Static body (box)        - SBOX_Name
    //  - Static body (circle)     - SCI_Name

    //  - Kinematic body (box)     - KBOX_Name
    //  - Kinematic body (circle)  - KCI_Name

    //  - Dynamic body (box)       - DBOX_Name
    //  - Dynamic body (circle)    - DCI_Name

    //Offset to move the entire machine
    float x_offset =200;
    float y_offset =200;

    //We create the static bodies that'll be part of the machine's structure (static walls and floors for the machine)
#pragma region Structure_Static_Body_Creation
    //Start ramp
    Cube SBOX_Stage1(world, x_offset-2.5f, y_offset+205, 300, 10, sf::Color::White, 1,1, 30);
    //Walls
    Cube SBOX_Stage2(world, x_offset+125, y_offset+350, 10, 150, sf::Color::White, 1, 1, 0);
    Cube SBOX_Stage3(world, x_offset+200, y_offset+365, 10, 220, sf::Color::White, 1, 1, 0);
    Cube SBOX_Stage4(world, x_offset+275, y_offset+375, 10, 200, sf::Color::White, 1, 1, 0);
    //Floor for the walls, just so it looks neat
    Cube SBOX_Stage5(world, x_offset+200, y_offset+480, 180, 10, sf::Color::White, 1, 1, 0);
    //Flat runway after pulley joint
    Cube SBOX_Stage6(world, x_offset+375, y_offset+280, 200, 10, sf::Color::White, 1, 1, 0);
    //Wall that will stop the advance of the distance/weld joint
    Cube SBOX_Stage7(world, x_offset+475, y_offset+300, 10, 50, sf::Color::White, 1, 1, 0);
    //Area where the second ball will be propelled
    Cube SBOX_Stage8(world, x_offset+630, y_offset+480, 340, 10, sf::Color::White, 1, 1, 0);
    //Something to make it look like this is on a table
    Cube SBOX_Stage9(world, x_offset+500, y_offset+515, 10000, 60, sf::Color::White, 1, 1, 0);

    //Wall that will stop the advance of the pulley ball
    Cube SBOX_Stage10(world, x_offset+530, y_offset+225, 10, 200, sf::Color::White, 1, 1, 0);

    //Update the static walls and floors for the machine
    //We update all the static bodies declared here to avoid doing so in every cycle
    SBOX_Stage1.update();
    SBOX_Stage2.update();
    SBOX_Stage3.update();
    SBOX_Stage4.update();
    SBOX_Stage5.update();
    SBOX_Stage6.update();
    SBOX_Stage7.update();
    SBOX_Stage8.update();
    SBOX_Stage9.update();
    SBOX_Stage10.update();
#pragma endregion

    //Top shelf area, this part will slide to the right, allowing a box to fall by the end of the contraption
#pragma region Shelf body creation
    //Static body case
    Cube SBOX_ShelfSide1(world, x_offset+709, y_offset-100, 10, 100, sf::Color::White, 1, 1, 0);
    Cube SBOX_ShelfSide2(world, x_offset+761, y_offset-100, 10, 100, sf::Color::White, 1, 1, 0);
    SBOX_ShelfSide1.update();
    SBOX_ShelfSide2.update();

    //Sliding structure
    Cube KBOX_ShelfSlide1(world, x_offset+715, y_offset-50, 40, 10, sf::Color::Cyan, 2, 1, 0);
    Cube KBOX_ShelfSlide2(world, x_offset+755, y_offset-50, 40, 10, sf::Color::Cyan, 2, 1, 0);
    //This is a dynamic body, the slide will take the velocity from it and apply it to itself
    Circle DCI_ShelfActivator(world, x_offset+50, y_offset+480, 10, sf::Color::Cyan, 3, 1);
    //Create the dynamic box that'll get dropped in
    Cube DBOX_Drop(world, x_offset+735, y_offset-80, 40, 40, sf::Color::Red, 3, 0.25f, 0);
#pragma endregion

    //Creation of the objects involved in pulley joint, and pulley joint itself
#pragma region PulleyJoint_Creation
    //Static anchor bodies for the pulley (from cube class)
    Cube SBOX_PulleyAnchor1(world, x_offset+162, y_offset+230, 50, 10, sf::Color::Yellow, 2, 1, 30);
    Cube SBOX_PulleyAnchor2(world, x_offset+237, y_offset+240, 50, 10, sf::Color::Yellow, 2, 1, 330);
    //Update the static objects, so they get drawn every frame in the same position (because they don't move)
    SBOX_PulleyAnchor1.update();
    SBOX_PulleyAnchor2.update();

    //Create the dynamic bodies from cube class, these will be our pulley platforms
    Cube DBOX_PulleyPlatform1(world, x_offset+162, y_offset+300, 60, 10, sf::Color::Yellow, 3, 1, 0);
    Cube DBOX_PulleyPlatform2(world, x_offset+237, y_offset+450, 60, 10, sf::Color::Yellow, 3, 1, 0);
    //Set the rotation to fixed, so they don't start spinning when things hit them
    DBOX_PulleyPlatform1.getBody()->SetFixedRotation(true);
    DBOX_PulleyPlatform2.getBody()->SetFixedRotation(true);

    //Define and create the joint in world space
    b2PulleyJointDef PulleyJointDef;
    PulleyJointDef.Initialize(DBOX_PulleyPlatform1.getBody(), DBOX_PulleyPlatform2.getBody(),   //both platform bodies
                              SBOX_PulleyAnchor1.getBody()->GetWorldCenter(), SBOX_PulleyAnchor2.getBody()->GetWorldCenter(),   //both anchor world centers
                              DBOX_PulleyPlatform1.getBody()->GetWorldCenter(), DBOX_PulleyPlatform2.getBody()->GetWorldCenter(), //both platform world centers
                              1);   //Ratio
    world.CreateJoint(&PulleyJointDef);

    //Create the ball that'll start on the pulley, it'll go up it in when simulated
    Circle DCI_PulleyBall(world, x_offset+245, y_offset+450, 15, sf::Color::Red, 3, 2);
#pragma endregion

    //Creation of the objects involved in revolute joint, and the joint itself
#pragma region Revolute joint creation
    //Revolute joint bodies, we set gravity scale to 0, so they don't instantly fall
    Circle DCI_RevAnchor(world, x_offset+300, y_offset+210, 7.5f, sf::Color::Blue, 3, 20);
    DCI_RevAnchor.getBody()->SetGravityScale(0.0f);
    Cube DBOX_RevArm(world, x_offset+300, y_offset+210, 80, 10, sf::Color::Blue, 3, 20, 1);
    DBOX_RevArm.getBody()->SetGravityScale(0.0f);

    //Declare the joint definition variable (maybe that is what this is, could be wrong)
    b2RevoluteJointDef RevjointDef;
    RevjointDef.Initialize(DCI_RevAnchor.getBody(),DBOX_RevArm.getBody(),DCI_RevAnchor.getBody()->GetWorldCenter());

    //Other important parameters, trying to turn the joint into a motor
    RevjointDef.enableMotor = true;
    RevjointDef.maxMotorTorque = 10.0f*100000;
    RevjointDef.motorSpeed = -2.0f*10000;

    //Actually create the joint in world space
    b2RevoluteJoint* Revjoint = (b2RevoluteJoint*)world.CreateJoint(&RevjointDef);
#pragma endregion

    //Creation of the objects involved in prismatic joint, and the joint itself
#pragma region Prism joint
    //Create the bodies from the cube class
    Cube DBOX_PrismMove(world, x_offset+500, y_offset+375, 50, 10, sf::Color::Cyan, 3, 0.5f, 0);
    Cube DBOX_PrismFixed(world, x_offset+400, y_offset+375, 10, 10, sf::Color::Cyan, 3, 0.5f, 0);
    //Set these so it won't move on its own
    DBOX_PrismMove.getBody()->SetGravityScale(0.0f);
    DBOX_PrismFixed.getBody()->SetGravityScale(0.0f);
    //We don't want this part of the prismatic joint to rotate
    DBOX_PrismMove.getBody()->SetFixedRotation(true);
    DBOX_PrismFixed.getBody()->SetFixedRotation(true);

    //Declare the joint definition variable
    b2PrismaticJointDef PrismjointDef;

    b2Vec2 worldAxis(0.0f, 1.0f);
    //Other necessary variables
    PrismjointDef.Initialize(DBOX_PrismMove.getBody(), DBOX_PrismFixed.getBody(),
                             DBOX_PrismMove.getBody()->GetWorldCenter(), worldAxis);
    PrismjointDef.localAxisA.Set(0,1);
    PrismjointDef.upperTranslation = 50.0f;
    PrismjointDef.lowerTranslation = 50.0f;
    PrismjointDef.enableLimit = true;
    PrismjointDef.collideConnected = false;
    //PrismjointDef.localAnchorA.Set(-(DBOX_PrismMove.getSizeX()/(SCALE*2)), 0.0f);
    //PrismjointDef.localAnchorB.Set(DBOX_PrismFixed.getSizeX()/(SCALE*2)+120.0f, 0.0f);

    b2PrismaticJoint* Prismjoint = (b2PrismaticJoint*)world.CreateJoint(&PrismjointDef);
#pragma endregion

    //Creation of joint between parts of the Prismatic joint and the gear joint
#pragma region Distance joint creation
    b2DistanceJointDef DistjointDef;
    DistjointDef.Initialize(DBOX_PrismMove.getBody(), DBOX_PulleyPlatform2.getBody(), DBOX_PrismMove.getBody()->GetPosition(), DBOX_PulleyPlatform2.getBody()->GetPosition());

    DistjointDef.collideConnected = false;
    DistjointDef.stiffness = 10;

    //Actually create the joint in world space
    b2DistanceJoint* joint = (b2DistanceJoint*)world.CreateJoint(&DistjointDef);

#pragma endregion

    //Create the dynamic circles that are used as the movable objects that interact with others in the machine
    Circle DCI_PelotaInicio(world, x_offset+50, y_offset+50, 20, sf::Color::Red, 3, 4);
    Circle DCI_PelotaInicio2(world, x_offset+50, y_offset+150, 20, sf::Color::Red, 3, 3);
    Circle DCI_PelotaInicio3(world, x_offset+50, y_offset+0, 15, sf::Color::Red, 3, 3);
    Circle DCI_PelotaInicio4(world, x_offset+25, y_offset+175, 10, sf::Color::Green, 3, 1);



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

#pragma region Update dynamic 'and' kinematic shelf components
        //Slide sets the linear velocity from the activator to its own variable
        KBOX_ShelfSlide1.getBody()->SetLinearVelocity(b2Vec2( DCI_ShelfActivator.getBody()->GetLinearVelocity().x*2, 0.0f));
        KBOX_ShelfSlide2.getBody()->SetLinearVelocity(b2Vec2(-DCI_ShelfActivator.getBody()->GetLinearVelocity().x*2, 0.0f));
        //Update all the shelf components
        KBOX_ShelfSlide1.update();
        KBOX_ShelfSlide2.update();

        DCI_ShelfActivator.update();
        DBOX_Drop.update();
#pragma endregion

#pragma region Update all the dynamic bodies associated with the pulley joint
        //Update the pulley joint platforms
        DBOX_PulleyPlatform1.update();
        DBOX_PulleyPlatform2.update();
        DCI_PulleyBall.update();
#pragma endregion

#pragma region Update revolute joint
    DBOX_RevArm.update();
    DCI_RevAnchor.update();
#pragma endregion

#pragma region Update prismatic joint
        DBOX_PrismMove.update();
        DBOX_PrismFixed.update();
#pragma endregion


        //Update the dynamic circles that are used as the movable objects that interact with others in the machine
        DCI_PelotaInicio.update();
        DCI_PelotaInicio2.update();
        DCI_PelotaInicio3.update();
        DCI_PelotaInicio4.update();



        //Clear everything then draw it again
        window.clear();

        //Draw the static objects that form the shape of the machine
# pragma region Draw Stage
        SBOX_Stage1.draw(window);
        SBOX_Stage2.draw(window);
        SBOX_Stage3.draw(window);
        SBOX_Stage4.draw(window);
        SBOX_Stage5.draw(window);
        SBOX_Stage6.draw(window);
        SBOX_Stage7.draw(window);
        SBOX_Stage8.draw(window);
        SBOX_Stage9.draw(window);
        SBOX_Stage10.draw(window);
#pragma endregion

#pragma region Draw shelf components
        //draw the static components of the shelf
        SBOX_ShelfSide1.draw(window);
        SBOX_ShelfSide2.draw(window);
        //Draw the kinematic 'and' dynamic part of the shelf
        KBOX_ShelfSlide1.draw(window);
        KBOX_ShelfSlide2.draw(window);
        DCI_ShelfActivator.draw(window);
        DBOX_Drop.draw(window);
#pragma endregion

#pragma region Draw Pulley joint associated components
        //Anchors
        SBOX_PulleyAnchor1.draw(window);
        SBOX_PulleyAnchor2.draw(window);
        //Platforms
        DBOX_PulleyPlatform1.draw(window);
        DBOX_PulleyPlatform2.draw(window);
        //Ball that starts on it
        DCI_PulleyBall.draw(window);
#pragma endregion

#pragma region Draw revolute joint
        DBOX_RevArm.draw(window);
        DCI_RevAnchor.draw(window);
#pragma endregion

#pragma region Draw prismatic joint
        DBOX_PrismMove.draw(window);
        DBOX_PrismFixed.draw(window);
#pragma endregion



        //Draw the interacting circles
        DCI_PelotaInicio.draw(window);
        DCI_PelotaInicio2.draw(window);
        DCI_PelotaInicio3.draw(window);
        DCI_PelotaInicio4.draw(window);


        window.display();

        //Wait for x milliseconds every time the window is drawn
        std::this_thread::sleep_for(std::chrono::nanoseconds(timeJumps*timeMultiplier));
    }
    return 0;
}
