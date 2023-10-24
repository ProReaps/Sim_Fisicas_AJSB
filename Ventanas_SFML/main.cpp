#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

constexpr float SCALE = 30.0f;
constexpr float OFFS = SCALE*2;

class Cube {
public:
    Cube(b2World &world, float x, float y) {
        b2BodyDef bodydef;
        bodydef.position.Set(x / SCALE, y / SCALE);
        bodydef.type = b2_dynamicBody;
        body = world.CreateBody(&bodydef);

        b2PolygonShape shape;
        shape.SetAsBox((10.f / 2) / SCALE, (10.f / 2) / SCALE);

        b2FixtureDef fixturedef;
        fixturedef.shape = &shape;
        fixturedef.density = 1.0f;
        body->CreateFixture(&fixturedef);

        box.setSize(sf::Vector2f(10.f, 10.f));
        box.setFillColor(sf::Color::Red);
        box.setOrigin(5.f, 5.f);
    }

    void update() {
        box.setPosition(SCALE * body->GetPosition().x, SCALE * body->GetPosition().y);
        box.setRotation(body->GetAngle() * 180 / b2_pi);
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

private:
    sf::RectangleShape box;
    b2Body *body;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 900), "SFML works!");

    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);

    Cube cube(world, 400, 300);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                cube.getBody()->ApplyForceToCenter(b2Vec2(0.1f, 0.0f), true);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                cube.getBody()->ApplyForceToCenter(b2Vec2(-0.1f, 0.0f), true);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                cube.getBody()->ApplyForceToCenter(b2Vec2(0.0f, -0.1f), true);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                cube.getBody()->ApplyForceToCenter(b2Vec2(0.0f, 0.1f), true);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                cube.resetPosition();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                cube.stop();
            }

            // The Z key event to close the window
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                window.close();
                return 0; // return from the main function, effectively ending the program
            }
        }

        world.Step(1 / 60.f, 8, 3);
        cube.update();

        window.clear();
        cube.draw(window);

        window.display();
    }
    return 0;
}