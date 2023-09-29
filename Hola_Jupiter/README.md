# Hola_Jupiter

## Descripción general del programa
 Este programa simula un cuerpo (con forma de cubo) cayendo en la superficie de júpiter usando la librería Box2D como base.

 Para poder simular las condiciones en la superficie de júpiter, primero hay que saber cuales son necesariamente definidas dentro del programa y conocerlas individualmente. Menciono que solo serían las condiciones necesarias, porque dependiendo de qué estemos simulando, necesitaremos saber que tanto
## Instrucciones
 Este es el primer proyecto en el que fuimos asignados atributos reales a recrear para el cuerpo simulado, fueron las siguientes:
 * Cubo de carburo de tungsteno de dimensiones de 1 metro cuadrado cayendo en la superficie de Júpiter de una altura de 15 metros, simulado por 1 segundo
 * Fricción del aire 'inventada'
## Tipo de problema
Esta simulación se trata de un caso de **caída libre**.
En la caída libre un objeto cae verticalmente desde cierta altura $H$ despreciando cualquier tipo de rozamiento con el aire o cualquier otro obstáculo. Se trata de un movimiento rectilíneo uniformemente acelerado (m.r.u.a.) o movimiento rectilíneo uniformemente variado (m.r.u.v.) en el que la aceleración coincide con el valor de la gravedad.
## Condiciones
### Gravedad
En el primer proyecto (Hola_Mundo) se explicó qué es la gravedad, en el caso de este proyecto, el valor de la gravedad cambió debido a que la masa de Júpiter es mayor. Por eso dentro del código en la línea.
```cpp
	b2Vec2 gravity(0.0f, -24.79f);
```
El valor del componente $y$ del vector '$gravity$' cambia a $-24.79$ metros sobre segundo (no tiene unidades pero eso representa), porque estamos tratando de hacer una simulación de lo que pasaría bajo las condiciones presentes en Júpiter y esa es la gravedad presente.
De nuevo, el valor de gravedad se agrega de forma negativa.
### Altura (punto de inicio del cubo)
Este punto se explica dentro del proyecto (Hola_Mundo). De nuevo, cambia su valor, esta vez con 15.0 metros en lugar de 10.0 metros. Esta es la altura mencionada en las instrucciones del ejercicio.
```cpp
	bodyDef.type = b2_dynamicBody;  
	bodyDef.position.Set(0.0f,15.0f);   //cambia la altura a la dada para el ejercicio
```
### Densidad
La densidad fue explicada dentro del proyecto Hola_Mundo. La densidad del carburo de tungsteno es de  $15600 kg/m^3$. Como este es el material que queremos replicar para el cuerpo dentro de la simulación, es necesario darle este valor al atributo de densidad
```cpp
	//Hacemos una fixture def aparte para cambiar cada característica del cuerpo  
	b2FixtureDef fixtureDef;  
	fixtureDef.shape = &dynamicBox;  
	fixtureDef.density = 15600.0f;  //densidad cambiada a la del carburo de tungsteno
```
### Masa
Mismo caso que el de la densidad. Es explicado dentro del proyecto Hola_Mundo. 
Como cambiamos el valor de densidad por un valor mucho mayor, nuestra caja de 1 metro cuadrado va a tener una masa mayor.
### Fricción
Este punto fue explicado dentro del proyecto de 'Hola_Mundo'.
Las indicaciones del ejercicio nos decían que agregáramos un valor de fricción inventado. por esto yo le di un valor de 0.9 unidades.
```cpp
	//fricción del aire (inventada pero mayor)  
	fixtureDef.friction = 0.9f;
```
### Tiempo de simulación
Mismo caso que el de la densidad, masa y fricción, es explicado en el proyecto 'Hola_Mundo'. 
Las instrucciones nos indican que se va a simular por 1 segundo, no nos dicen nada acerca de cuantos pasos por segundo así que decidí dejar el número de pasos por segundo intacto y enfocarme en cuantos pasos se simularían.
```cpp
	...
	float timeStep = 1.0f/60;
	...
	for(int32 i=0; i<60; ++i){...}
```
Dentro del código $timeStep$ tiene el mismo valor que proyectos pasados, indicando que cada paso de simulación es $1/60$ segundos. La condición del ciclo $for$ nos indica que se realizarán 60 pasos de simulación, por lo que vamos a simular 1 segundo del experimento.
## ¿Qué podemos esperar de la simulación? (a grandes rasgos)
 Podemos esperar que el cubo disminuya su posición en la coordenada y, debido a la fuerza que la gravedad ejerce sobre el. Además, podemos esperar que la posición cambie cada vez más rápido, porque va a lograr tener cierta aceleración y aumentará su velocidad de forma gradual hasta colisionar con otro cuerpo.

 Además, si comparamos los valores que despliega esta simulación con las de los proyectos Hola_Mundo y Hola_Luna podemos esperar que la altura a la que está el cuerpo disminuya considerablemente más rápido en Hola_Jupiter (este proyecto).
## Código
```cpp
#include <iostream>  
#include <Box2d/Box2d.h>  
//Descripcion:  
//Cubo carburo de tungsteno de dimensiones de 1 metro cuadrado cayendo en la 'superficie de jupiter'  
//de una altura de 15 metros cuadrados, simulado por 1 segundo  
  
int main(){  
    //Mundo  
    //Creando el vector de gravedad y el mundo    
    //En este caso la gravedad es -24.79f porque está basado en las condiciones de jupiter    
    b2Vec2 gravity(0.0f, -24.79f);  
    b2World world(gravity);  
  
    //Piso (no cambié nada del código de la clase en la sección de piso)  
    //Características del cuerpo    
    b2BodyDef groundBodyDef;  
    groundBodyDef.position.Set(0.0f, -10.0f);  
  
    //Creación del cuerpo  
    b2Body* groundBody = world.CreateBody(&groundBodyDef);  
  
    //Crear la forma del cuerpo  
    b2PolygonShape groundBox;  
    groundBox.SetAsBox(50,1);  
  
    //Se crea el fixture  
    groundBody->CreateFixture(&groundBox, 0.0f);  
  
    //Cuerpo dinamico (caja)  
    //Características de la caja    
    b2BodyDef bodyDef;  
    bodyDef.type = b2_dynamicBody;  
    bodyDef.position.Set(0.0f,15.0f);   //cambia la altura a la dada para el ejercicio  
  
    //Creacion del cuerpo    
    b2Body* body = world.CreateBody(&bodyDef);  
  
    //Crear la forma del cuerpo  
    b2PolygonShape dynamicBox;  
    dynamicBox.SetAsBox(1.0f,1.0f);  
  
    //Hacemos una fixture def aparte para cambiar cada característica del cuerpo  
    b2FixtureDef fixtureDef;  
    fixtureDef.shape = &dynamicBox;  
    fixtureDef.density = 15600.0f;  //densidad cambiada a la del carburo de tungsteno  
    //fricción del aire (inventada pero mayor)    
    fixtureDef.friction = 0.9f;  
  
    //Cargamos el fixtureDef que creamos  
    body->CreateFixture(&fixtureDef);  
  
    //Caida  
    float timeStep = 1.0f/60;  
  
    int32 velocityIterations = 6;  
    int32 positionIteration = 2;  
  
    //Tiempo ajustado a 1 segundo de simulación  
    for(int32 i=0; i<60; ++i)  
    {  
        world.Step(timeStep, velocityIterations, positionIteration);  
        b2Vec2 position = body->GetPosition();  
        float angle = body->GetAngle();  
        printf("%4.2f %4.2f %4.2f \n",position.x, position.y, angle);  
    }  
}
```
## Condiciones dentro del código
Los cambios realizados a las condiciones tanto del mundo como las del cuerpo dinámico ya fueron mencionados conforme se introducían.