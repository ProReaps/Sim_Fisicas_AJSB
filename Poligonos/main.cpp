#include <iostream>
#include <Box2D/Box2D.h>
#include <iostream>
#include <Box2D/Box2D.h>

int main() {
    b2PolygonShape Triangulo;
    b2PolygonShape Rectangulo;
    b2PolygonShape Cuadrado;
    b2PolygonShape Pentagono;
    b2PolygonShape Hexagono;
    b2PolygonShape Octagono;

    // Triangulo (equilatero)
    b2Vec2 Vert_Triangulo[] =
    {
            b2Vec2(0.0f, 0.0f),
            b2Vec2(1.0f, 0.0f),
            b2Vec2(0.5f, 0.866f)
    };
    Triangulo.Set(Vert_Triangulo, 3);

    // Rectangulo
    b2Vec2 Vert_Rectangulo[] =
    {
            b2Vec2(-2.0f, 0.0f),
            b2Vec2(2.0f, 0.0f),
            b2Vec2(2.0f, 2.0f),
            b2Vec2(-2.0f, 2.0f)
    };
    Rectangulo.Set(Vert_Rectangulo, 4);

    // Cuadrado
    b2Vec2 Vert_Cuadrado[] =
    {
            b2Vec2(-1.0f, -1.0f),
            b2Vec2(1.0f, -1.0f),
            b2Vec2(1.0f, 1.0f),
            b2Vec2(-1.0f, 1.0f)
    };
    Cuadrado.Set(Vert_Cuadrado, 4);

    //Pentagono
    b2Vec2 Vert_Pentagono[] =
    {
            b2Vec2(0.0f, 0.0f),
            b2Vec2(1.5f, 0.0f),
            b2Vec2(2.5f, 1.5f),
            b2Vec2(0.75f, 2.75f),
            b2Vec2(-1.0, 1.5f),
    };
    Pentagono.Set(Vert_Pentagono, 5);

    //Hexagono
    b2Vec2 Vert_Hexagono[] =
    {
            b2Vec2(0.0f, 0.0f),
            b2Vec2(2.0f, 0.0f),
            b2Vec2(3.5f, 1.5f),
            b2Vec2(2.0f, 3.0f),
            b2Vec2(0.0, 3.0f),
            b2Vec2(-1.5, 1.5f)
    };
    Hexagono.Set(Vert_Hexagono, 6);

    //Octagono
    b2Vec2 Vert_Octagono[] =
    {
            b2Vec2(0.0f, 0.0f),
            b2Vec2(2.0f, 0.0f),
            b2Vec2(3.0f, 1.0f),
            b2Vec2(3.0f, 3.0f),
            b2Vec2(2.0f, 4.0f),
            b2Vec2(0.0f, 4.0f),
            b2Vec2(-1.0f, 3.0f),
            b2Vec2(-1.0f, 1.0f)
    };
    Octagono.Set(Vert_Octagono, 8);

    //Insertar simulación
    return 0;
}
