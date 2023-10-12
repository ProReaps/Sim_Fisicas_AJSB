# Formas
## Descripción general del programa
 Este programa almacena código que utiliza PolygonShape de Box2d Crear las siguientes figuras:
* Triangulo 
* Rectángulo
* Cuadrado
* Pentágono
* Hexágono
* Octágono.
No se utilizan en ninguna simulación, ni se agregan a un cuerpo, pero se crean y se podría tomar el código para insertarlo en otro programa.

## Main
Dentro de la sección inicial del código declaramos las variables de formas que vamos a crear:
```cpp
b2PolygonShape Triangulo;  
b2PolygonShape Rectangulo;  
b2PolygonShape Cuadrado;  
b2PolygonShape Pentagono;  
b2PolygonShape Hexagono;  
b2PolygonShape Octagono;
```
## Funciones de creación de formas
#### Triangulo
Función para crear un triangulo
```cpp
// Triangulo (equilatero)  
b2Vec2 Vert_Triangulo[] = {  
        b2Vec2(0.0f, 0.0f),  
        b2Vec2(1.0f, 0.0f),  
        b2Vec2(0.5f, 0.866f)  
};  
Triangulo.Set(Vert_Triangulo, 3);
```
#### Rectángulo
Función para crear el rectángulo
```cpp
// Rectangulo  
b2Vec2 Vert_Rectangulo[] = 
{  
        b2Vec2(-2.0f, 0.0f),  
        b2Vec2(2.0f, 0.0f),  
        b2Vec2(2.0f, 2.0f),  
        b2Vec2(-2.0f, 2.0f)  
};  
Rectangulo.Set(Vert_Rectangulo, 4);
```
#### Cuadrado
Función para crear el cuadrado
```cpp
// Cuadrado  
b2Vec2 Vert_Cuadrado[] =  
{  
        b2Vec2(-1.0f, -1.0f),  
        b2Vec2(1.0f, -1.0f),  
        b2Vec2(1.0f, 1.0f),  
        b2Vec2(-1.0f, 1.0f)  
};  
Cuadrado.Set(Vert_Cuadrado, 4);
```
#### Pentágono
Función para crear el pentágono
```cpp
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
```
#### Hexágono
Función para crear el Hexágono
```cpp
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
```
#### Octágono
Función para crear el octágono
```cpp
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
```
## Notas
* Estas formas serían añadidas a un fixture para su uso dentro de una simulación, posteriormente el fixture se añadiría a un cuerpo.
* Las funciones se pueden mejorar, creando un offset o escala a la que se quiere dibujar la forma. Esto se implementaría con su propia función creada por nosotros que cambia los valores insertados en los vectores de vértices.
* Intenté poner valores que más o menos le darían una forma regular.