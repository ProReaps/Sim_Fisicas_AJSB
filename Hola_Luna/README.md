# Hola_Luna

## Descripción general del programa
 Este programa simula un cuerpo (con forma de cubo) cayendo en la superficie de la luna usando la librería Box2D como base.

 Para poder simular las condiciones en la superficie de la luna, primero hay que saber cuales son necesariamente definidas dentro del programa y conocerlas individualmente. Menciono que solo serían las condiciones necesarias, porque dependiendo de qué estemos simulando, necesitaremos saber que tanto
## Tipo de problema
Esta simulación se trata de un caso de **caída libre**.
En la caída libre un objeto cae verticalmente desde cierta altura $H$ despreciando cualquier tipo de rozamiento con el aire o cualquier otro obstáculo. Se trata de un movimiento rectilíneo uniformemente acelerado (m.r.u.a.) o movimiento rectilíneo uniformemente variado (m.r.u.v.) en el que la aceleración coincide con el valor de la gravedad.
## Condiciones
### Gravedad
En el primer proyecto (Hola_Mundo) se explicó qué es la gravedad, en el caso de este proyecto, el valor de la gravedad cambió debido a que la masa de la luna es menor. Por eso dentro del código en la línea.

```cpp
	b2Vec2 gravity(0.0f, -1.62f);
```

El valor del componente $y$ del vector '$gravity$' cambia a $-1.62$ metros sobre segundo (no tiene unidades, pero eso representa), porque estamos tratando de hacer una simulación de lo que pasaría bajo las condiciones presentes en la luna y esa es la gravedad presente.
De nuevo, el valor de gravedad se agrega de forma negativa.
### Altura (punto de inicio del cubo)
Este punto se explica dentro del proyecto (Hola_Mundo). De nuevo, cambia su valor, esta vez con 20.0 metros en lugar de 10.0 metros. Quiere decir que nuestro cuerpo caerá el doble de la altura que el proyecto pasado.
```cpp
	bodyDef.type = b2_dynamicBody;  
	bodyDef.position.Set(0.0f,20.0f);
```
### Densidad
La densidad fue explicada dentro del proyecto Hola_Mundo. No hay ningún cambio de ese proyecto a este. Ni en los valores asignados dentro del código.
### Masa
Mismo caso que el de la densidad. No hay ningún cambio del proyecto Hola_Mundo a este.
### Fricción
El concepto de fricción fue explicado dentro del proyecto 'Hola_Mundo'.
En la luna no hay resistencia del aire, por lo que el valor de fricción que le asignamos al cuerpo en este proyecto será de $0$, o no se le asignará, porque ese es el predeterminado. Eso fue lo que hice, simplemente lo comenté para que tomara el valor predeterminado.
```cpp
	//no hay friccion en la luna  
	//fixtureDef.friction = 0.3f;
```
Este cambio se debe a que la luna no cuenta con una atmósfera, y por tanto no hay resistencia del aire. Por esto se cumplen las condiciones para que ante una caída no haya una fuerza de resistencia por parte del aire
### Tiempo de simulación
Mismo caso que el de la densidad y masa. No hay ningún cambio del proyecto Hola_Mundo a este.
## ¿Qué podemos esperar? (a grandes rasgos)
 Podemos esperar que el cubo disminuya su posición en la coordenada y, debido a la fuerza que la gravedad ejerce sobre el. Además, podemos esperar que la posición cambie cada vez más rápido, porque va a lograr tener cierta aceleración y aumentará su velocidad de forma gradual hasta colisionar con otro cuerpo.

 Además, si comparamos los valores de esta simulación con los del proyecto Hola_Mundo podemos esperar que la altura a la que está el cuerpo disminuya considerablemente más lento en Hola_Luna.
## Código
```cpp
#include <iostream>  
#include <Box2d/Box2d.h>  
//Descripcion:  
//Cubo callendo en la superficie de la luna  
  
int main(){  
    //Mundo  
    //Creando el vector de gravedad y el mundo    
    //En este caso la gravedad es -1.62f porque está basado en la superficie de la luna    
    b2Vec2 gravity(0.0f, -1.62f);  
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
  
    //Cuerpo dinamico (caja) (aquí solo cambié la fricción, la comenté porque escencialmente no hay fricción en el espacio [palabras de google])  
    //Características de la caja    
    b2BodyDef bodyDef;  
    bodyDef.type = b2_dynamicBody;  
    bodyDef.position.Set(0.0f,20.0f);  
  
    //Creacion del cuerpo  
    b2Body* body = world.CreateBody(&bodyDef);  
  
    //Crear la forma del cuerpo  
    b2PolygonShape dynamicBox;  
    dynamicBox.SetAsBox(1.0f,1.0f);  
  
    //Hacemos una fixture def aparte para cambiar cada característica del cuerpo  
    b2FixtureDef fixtureDef;  
    fixtureDef.shape = &dynamicBox;  
    fixtureDef.density = 1.0f;  
    //no hay friccion en la luna  
    //fixtureDef.friction = 0.3f;  
    //Cargamos el fixtureDef que creamos   
	body->CreateFixture(&fixtureDef);  
  
  
    //Caida (aquí solo le asigné más tiempo, para que se notara el cambio del inicio hasta el fin de la ejecución)  
    float timeStep = 1.0f/60.0f;  
  
    int32 velocityIterations = 6;  
    int32 positionIteration = 2;  
  
    for(int32 i=0; i<180; ++i)  
    {  
        world.Step(timeStep, velocityIterations, positionIteration);  
        b2Vec2 position = body->GetPosition();  
        float angle = body->GetAngle();  
        printf("%4.2f %4.2f %4.2f \n",position.x, position.y, angle);  
    }  
}
```
## Condiciones dentro del código
Los cambios del proyecto Hola_Mundo a este proyecto (Hola_Luna) con respecto a valores de las condiciones simuladas ya fueron mencionados cuando se mencionaron las nuevas condiciones.