# Tema_2
## Descripción general del programa
 Este programa está creando una simulación 2D donde generamos un disparo de cañón en la luna (con un cuerpo con forma de caja) usando la librería Box2D como base.
 En este caso le asignamos cierta velocidad al objeto en el primer 'frame' de la simulación.

 Para poder simular las condiciones en la superficie de la luna, primero hay que saber cuales son necesariamente definidas dentro del programa y conocerlas individualmente. Menciono que solo serían las condiciones necesarias, porque dependiendo de qué estemos simulando, necesitaremos saber que tanto
## Instrucciones
Tiro de una bala de cañon, la densidad la ponemos nosotros, a una velocidad de 900 hm/h a un ángulo de 60 grados, de un tamaño promedio
## Tipo de problema
Esta simulación se trata de un caso de **tiro parabólico**.
El tiro parabólico, se aplica a un objeto, al que se le da una velocidad inicial con un ángulo de elevación y a continuación sigue una trayectoria determinada por la fuerza gravitacional y la resistencia de la atmósfera. La trayectoria que sigue es parabólica.
## Condiciones
Dentro de este proyecto decidí simular el Hierro como material para la bala de cañón.
### Gravedad
El concepto de gravedad fue explicado dentro del proyecto 'Hola_Mundo'.
La forma en que asignamos la gravedad y su valor se explica dentro del proyecto 'Hola_Luna'
El resultado es que dentro del código tenga este valor:
```cpp
	// Definimos la gravedad con el valor que tendría en la luna  
	b2Vec2 gravity(0.0f, -1.62f);  
	  
	// Construct a world object, which will hold and simulate the rigid bodies  
	b2World world(gravity);
```
### Posición inicial (punto de inicio del cubo)
Un punto muy parecido fue explicado dentro del proyecto 'Hola_Mundo', Altura (punto de inicio del cubo).
La posición inicial es simplemente el punto (con coordenadas $x,y$) en la que se encuentra un cuerpo al inicio de un experimento.
Dentro de este proyecto colocamos el cuerpo dinámico en el origen ($0, 0$)
```cpp
	//Creación de un cuerpo dinámico  
	b2BodyDef bodyDef;  
	bodyDef.type = b2_dynamicBody;  
	bodyDef.position.Set(0.0f, 0.0f);  
	b2Body* body = world.CreateBody(&bodyDef);
```
### Dimensiones de la forma
Las instrucciones del ejercicio indicaban que era necesario investigar acerca de las dimensiones comunes de balas de cañón. Encontré que el diámetro era comúnmente de 30cm.
```cpp
	//Definimos una forma para la bola de cañon, en este caso de 30cm (o 0.3m) que fue  
	//el diametro que encontré en google.  
	b2PolygonShape dynamicBox;  
	dynamicBox.SetAsBox(0.3f, 0.3f);
```
### Densidad
La densidad fue explicada dentro del proyecto Hola_Mundo. 
La densidad del hierro es de  $7874 kg/m^3$. Como este es el material que decidí replicar para el cuerpo dentro de la simulación, es necesario darle este valor al atributo de densidad.
```cpp
	// Define the dynamic body fixture  
	b2FixtureDef fixtureDef;  
	fixtureDef.shape = &dynamicBox;  
	//asignando la densidad del hierro al cuerpo  
	fixtureDef.density = 7874.0f;  
	//Sin friccion porque estamos en la luna  
	//fixtureDef.friction = 0.0f;
```
### Masa
Mismo caso que el de la densidad. Es explicado dentro del proyecto Hola_Mundo. 
Como cambiamos el valor de densidad por un valor mayor y las dimensiones de la caja, tendrá una masa diferente a las de ejercicios pasados. De nuevo, el valor de masa es calculado por la librería.
### Fricción
El concepto de fricción fue explicado dentro del proyecto 'Hola_Mundo'.
La forma en que asignamos la fricción y su valor se explica dentro del proyecto 'Hola_Luna'
El resultado es que dentro del código tenga esta forma:
```cpp
	//Sin friccion porque estamos en la luna  
	//fixtureDef.friction = 0.0f;
```
### Velocidad
La velocidad es una magnitud física que expresa la relación entre el espacio recorrido por un objeto, el tiempo empleado para ello y su dirección.
Debido a que la velocidad también considera la dirección en que se produce el desplazamiento de un objeto, es considerada una magnitud de carácter vectorial.
Así, la velocidad implica el cambio de posición de un objeto en el espacio dentro de determinada cantidad de tiempo, es decir, la rapidez, más la dirección en que se produce dicho movimiento. De allí que velocidad y rapidez no sean lo mismo.
Su unidad en el Sistema Internacional de Unidades es el metro por segundo (m/s), e incluye la dirección del desplazamiento.
### Tiempo de simulación
El tiempo de simulación fue explicado dentro del proyecto 'Hola_Mundo'.
Dentro de este proyecto cambié los valores de $timeStep$ y cuantos pasos se dan dentro del ciclo for que maneja la simulación.
```cpp
	float timeStep = 1.0f/60.0f;
	for(int32 i=0; i<180; ++i)
```
## ¿Qué podemos esperar? (a grandes rasgos)
 Podemos esperar que el cubo aumente su posición en los componentes $x, y$ de su vector de posición gracias a la velocidad que le asignamos al inicio de la simulación. Además podemos esperar que su posición en $y$ comience a disminuir después de cierto punto en la simulación debido a que la gravedad va a ir disminuyendo su velocidad (en el componente $y$)
## Código
```cpp
#include <Box2D/Box2D.h>  
#include <cmath>  
#include <iostream>  
  
#ifndef M_PI  
#define M_PI 3.14159265358979323846  
#endif  
//Descripción:  
//Tiro de una bala de cañon, la densidad la ponemos nosotros, a una velocidad de 900 hm/h  
//a un ángulo de 60 grados, de un tamaño promedio  
int main() {  
    // Definimos la gravedad con el valor que tendría en la luna  
    b2Vec2 gravity(0.0f, -1.62f);  
  
    // Construct a world object, which will hold and simulate the rigid bodies  
    b2World world(gravity);  
  
    //Creación de un cuerpo dinámico  
    b2BodyDef bodyDef;  
    bodyDef.type = b2_dynamicBody;  
    bodyDef.position.Set(0.0f, 0.0f);  
    b2Body* body = world.CreateBody(&bodyDef);  
  
    //Definimos una forma para la bola de cañon, en este caso de 30cm (o 0.3m) que fue  
    //el diametro que encontré en google.    b2PolygonShape dynamicBox;  
    dynamicBox.SetAsBox(0.3f, 0.3f);  
  
    // Define the dynamic body fixture  
    b2FixtureDef fixtureDef;  
    fixtureDef.shape = &dynamicBox;  
    //asignando la densidad del hierro al cuerpo  
    fixtureDef.density = 7874.0f;  
    //Sin friccion porque estamos en la luna  
    //fixtureDef.friction = 0.0f;  
    // Add the shape to the body    body->CreateFixture(&fixtureDef);  
  
    // Set the initial velocity of the bullet  
    float angle = 60.0f; // Angle in degrees  
    float speed = 250.0f; // Equivalente de 900km/h en m/s  
    float vx = speed * std::cos(angle * M_PI / 180.0f);  
    float vy = speed * std::sin(angle * M_PI / 180.0f);  
  
    body->SetLinearVelocity(b2Vec2(vx, vy));  
  
    // Simulate the world  
    float timeStep = 1.0f / 60.0f;  
    int32 velocityIterations = 6;  
    int32 positionIterations = 2;  
  
    //Nota: la velocidad se aplica en el centro de masas del cuerpo  
    for (int i = 0; i < 300; ++i)  
    {  
        world.Step(timeStep, velocityIterations, positionIterations);  
        std::cout << body->GetPosition().x << " " << body->GetPosition().y << std::endl;  
    }  
  
    // Remember to clean up  
    world.DestroyBody(body);  
  
    return 0;  
}
## Condiciones dentro del código
### Gravedad
Dentro del código, creamos un vector que va a guardar el valor de la gravedad que queremos simular. Este valor va a ser guardado en el componente $y$ del mismo y será negativo.
Es el parámetro con el que creamos el mundo en la segunda línea.
```cpp
    //Mundo    //Creando el vector de gravedad y el mundo    
    b2Vec2 gravity(0.0f, -9.81f);  
    b2World world(gravity);
```
## Condiciones dentro del código
Los cambios realizados a las condiciones tanto del mundo como las del cuerpo dinámico ya fueron mencionados conforme se introducían. Exceptuando la velocidad.
### Velocidad
Box2D admite la velocidad (para poder asignarla) como un vector de dos dimensiones, para poder construirlo es necesario realizar los siguientes cálculos utilizando trigonometría para descomponer el vector de 900km/h (equivalente a 250m/s) a sus componentes de x y y.
Internamente también es necesario convertir el ángulo de grados a radianes para poder utilizar las funciones trigonométricas.
```cpp
	// Set the initial velocity of the bullet  
	float angle = 60.0f; // Angle in degrees  
	float speed = 250.0f; // Equivalente de 900km/h en m/s  
	float vx = speed * std::cos(angle * M_PI / 180.0f);  
	float vy = speed * std::sin(angle * M_PI / 180.0f);  
	  
	body->SetLinearVelocity(b2Vec2(vx, vy));
```
