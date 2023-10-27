# Ventanas SFML
## Descripción general del programa
 Este programa crea una ventana que funciona como viewport de dos dimensiones para box2D, dejándonos ver la forma en la que estamos manipulando una caja de box2D.
 Dentro del mismo podemos hacer que una caja de box2D se mueva gracias a que le estamos aplicando fuerza al centro de su cuerpo.

 Con estos dos aspectos, el programa nos deja visualizar una caja de box2D que controlamos con las teclas 'wasdrz' y barra espaciadora del teclado.
## Clase Cube
Esta es la clase que hace posible el funcionamiento del programa en sí.
La utilizamos para tener un control de dónde exactamente se encuentra el cuerpo de box2D y la forma de traducir la información de ese cuerpo a información dibujable dentro del viewport, esta traducción se hace más que nada a partir de una variable de escala, declarada al inicio del código.
```cpp
#include <Box2D/Box2D.h>  
  
constexpr float SCALE = 30.0f;        // Esta es la variable de la que hablo 
constexpr float OFFS = SCALE*2;
  
class Cube {  
public:  
    Cube(b2World &world, float x, float y) {
```
Esta variable se usaría dentro de la clase cubo y otras clases que tengamos que buscan tener funcionalidades parecidas.

#### Funciones
Dentro de la clase cubo, declaramos algunas funciones que nos dan el resto de la funcionalidad, tenemos las siguientes funciones:

##### Constructor
El constructor nos ayuda a inicializar el objeto de la clase cubo cuando lo creamos.
```cpp
Cube(b2World &world, float x, float y) 
{  
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
```
* La función en solo pide 3 variables de entrada:
	* El el espacio en memoria del mundo en el que queremos crear la caja, ya que es necesario para crear la caja en sí dentro de ese mundo.
	* El punto x donde se encuentra nuestro cubo, en contexto de la ventana. No en unidades de box2D.
	* El punto y donde se encuentra nuestro cubo, en contexto de la ventana. No en unidades de box2D.
Esta función se encarga de crearla caja dentro del mundo de box2D especificado y asignar algunas variables dentro de valores del cuadrado de SFML.
##### Update
La función update se encarga principalmente de actualizar los valores de posición de la variable privada box de SMFL.

Además, yo le agregué la funcionalidad de hacer que cuando pasa por algún extremo de la ventana, la posición de la caja (box2D) se cambie al otro extremo, implementando un valor de offset para evitar un bucle infinito.
```cpp
void update() {  
    box.setPosition(SCALE * body->GetPosition().x, SCALE * body->GetPosition().y);  
    box.setRotation(body->GetAngle() * 180 / b2_pi);  
    //  Make it loop (higher threshold)  
    //      x value of position vector is more than window width + an offset value, so it resets to 0    if((SCALE*body->GetPosition().x)>(1000+OFFS))  
    {  
        body->SetTransform(b2Vec2(0, body->GetPosition().y),body->GetAngle());  
    }  
    //      y value of position vector is more than window height + an offset value, so it resets to 0  
    if((SCALE*body->GetPosition().y)>(900+OFFS))  
    {  
        body->SetTransform(b2Vec2(body->GetPosition().x, 0),body->GetAngle());  
    }  
  
    //  Make it loop (lower threshold)  
    //      x value of position vector is less than 0 - an offset value, so it resets to window width    if((SCALE*body->GetPosition().x)<(0-OFFS))  
    {  
        body->SetTransform(b2Vec2(1000/SCALE, body->GetPosition().y),body->GetAngle());  
    }  
    //      y value of position vector is less than 0 - an offset value, so it resets to window height  
    if((SCALE*body->GetPosition().y)<(0-OFFS))  
    {  
        body->SetTransform(b2Vec2(body->GetPosition().x, 900/SCALE),body->GetAngle());  
    }  
    //  In both cases, the offset value helps to only reset it once, since the 'bounds' are farther away  
    //  than the points at which it reappears}
```
Esto lo hice a partir de ifs, que no es la mejor estructura posible, pero es una que me deja modificar valores de forma individual.
##### Draw
Esta función solo se encarga de dibujar el cuadrado que tenemos de SFML.
Tiene como valor de entrada un puntero al espacio de memoria de la ventana de SFML en la cual se va a dibujar.
```cpp
void draw(sf::RenderWindow &win) const {  
    win.draw(box);  
}
```
##### ResetPosition
Esta función nos ayuda a mover el cuerpo de box2D de regreso a su punto inicial, con el ángulo que tenga en ese momento.
```cpp
void resetPosition() {  
    body->SetTransform(b2Vec2(0, 0), body->GetAngle());  
}
```
##### Stop
Esta función le asigna al cuerpo una velocidad lineal de 0 en x y 0 en y.
```cpp
void stop() {  
    body->SetLinearVelocity(b2Vec2(0, 0));  
}
```
##### GetBody
Es una función que retorna el valor de la variable de body.
```cpp
b2Body *getBody() {  
    return body;  
}
```
El asterisco indica que está retornando un puntero de memoria.
Nos puede ayudar dentro del código a cambiar valores de esa variable y se utiliza de esta forma, porque si tener variables públicas de body, siendo 1 por cubo, no es lo más óptimo y si creamos una variable global de cuerpo para el programa, no es escalable a forma de crear más objetos de la clase.
#### Variables privadas
Tenemos dos variables privadas dentro de la clase cubo:
##### box
Guarda la caja que vamos a estar dibujando.
##### body
Guarda el cuerpo de el objeto.
## Main
Dentro del main de nuestro programa, creamos la ventana, el mundo de box2D y un objeto de la clase cubo.
Como nuestra ventana es de 1000 x 900, le damos valores de x-y de 400-300 al cubo dentro de su constructor.
### Ciclo de dibujado
Este es un concepto que hemos utilizado antes con páginas web que simulaban un juego simple. Básicamente, tenemos un ciclo en el que cada 'frame' analiza si hubo una entrada por parte del jugador y genera otros eventos a partir de la entrada. Luego, sin importar la entrada, borra el contenido de la ventana y dibuja su contenido nuevamente.
```cpp
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
```

### Inputs
Dentro del ciclo tenemos esta línea de código que solo busca si existió un input, 
```cpp
sf::Event event;  
    while (window.pollEvent(event)) {  
    ...
    }
```

En los casos de W, A, S y D le aplica fuerza al cuerpo desde su centro.
```cpp
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
```

En el caso de que presione la barra espaciadora, el cubo regresa a su posición incial.
```cpp
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {  
            cube.resetPosition();  
        }  
```

En el caso de que presione la tecla R, el cubo para completamente su velocidad haciendo uso de la función 'stop()' de la clase Cube.
```cpp
if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {  
            cube.stop();  
        }  
```

Sigue dentro de este ciclo hasta que el usuario presiona la tecla z, esto lo hace retornando un 0, porque ese sería el resultado de la función 'int main()' en la que estamos.
```cpp
// The Z key event to close the window  
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {  
            window.close();  
            return 0; // return from the main function, effectively ending the program  
        }  
```

### Limpiar y dibujar de nuevo
Cercas del final del ciclo, el mundo da un paso, limpiamos el contenido de la ventana, utilizamos las funciones de actualizar el cubo (update() y draw()) y mostramos el contenido nuevo de la ventana.
```cpp
world.Step(1 / 60.f, 8, 3);  
    cube.update();  
  
    window.clear();  
    cube.draw(window);  
  
    window.display();  
```

En caso de que no incluyamos la línea de código:
```cpp
window.clear();  
```
El contenido se dibujaría sobre el que ya está presente.


## General
Este programa nos da las bases para crear un viewport en el que integremos el dibujado de cajas haciendo uso de clases y objetos que engloben nuestros elementos simulados.
Se puede mejorar con más clases de diferentes formas o clases que creen diferentes tipos de cuerpos.

