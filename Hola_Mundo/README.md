# Hola mundo

## Descripción general del programa
 Este programa simula un cuerpo (con forma de cubo) cayendo en la superficie de la tierra usando la librería Box2D como base.

 Para poder simular las condiciones en la superficie de la tierra, primero hay que saber cuales son necesariamente definidas dentro del programa y conocerlas individualmente. Menciono que solo serían las condiciones necesarias, porque dependiendo de qué estemos simulando, necesitaremos saber que tanto
## Condiciones
### Gravedad
##### ¿Qué es la gravedad?
La gravedad o fuerza de gravedad es un fenómeno de la naturaleza por el cual los cuerpos que poseen masa se atraen entre sí de manera recíproca, con mayor intensidad conforme más masivos sean dichos cuerpos. Se trata de una de las cuatro interacciones fundamentales de la materia, y se la conoce también como “gravitación” o “interacción gravitatoria”.
Esta atracción es la que ejerce el planeta Tierra sobre todos los objetos en él, y hace que las cosas caigan.
##### ¿Cómo se mide la gravedad?
La fuerza de gravedad se mide (en el caso de este proyecto) en relación a la aceleración que imprime en los objetos sobre los cuales actúa, siempre que no intervengan otras fuerzas. Esta aceleración se ha calculado, en la superficie terrestre, en unos 9.80665 m/s2.

Por otro lado, la fuerza gravitacional puede medirse a través de distintas fórmulas, dependiendo del enfoque físico específico (mecánica clásica o relativista), y usualmente se representa, como otras fuerzas, en kilogramos de fuerza o en Newtons (N).
##### Fórmula para calcular la aceleración que 
$g = G m_2/r^2$
dónde:
* $g$ es la Gravedad en $m/s^2$.
* $G$ es la constante de gravitación universal
* $m$ es la masa de la tierra.
* $r$ es el radio de la tierra.
### Altura (punto de inicio del cubo)
Dentro de la geometría, definimos a la altura como la longitud o distancia de una dimensión geométrica, usualmente vertical o en la dirección de la gravedad. Aunque vamos a crear dos cuerpos que tienen una altura (geométrica) determinada dentro de la simulación, también vamos a asignarles una altitud determinada.
La altitud es distancia vertical a un origen determinado considerado como nivel cero, para el que se suele tomar el nivel medio del mar.

Dentro de Box2D, tenemos un punto inicial que directamente tiene las coordenadas (0, 0). Cuando creemos una simulación donde un objeto 'cae desde una altura de 10 metros', vamos a estar dándole ese valor de altitud.
### Densidad
La densidad es una  magnitud escalar referida a la cantidad de masa en un determinado volumen de una sustancia o un objeto sólido. Dentro de Box2D se usa la unidad de medida de densidad en $kg/m^3$.
### Masa
La masa es una magnitud escalar y de uso común en la física y la química, que expresa la cantidad de materia que hay en un objeto o un cuerpo. No es necesario declarar su valor dentro del código, debido a que Box2D hace el cálculo de forma directa, le asignamos una forma de cierto tamaño al cuerpo y le damos una densidad, la misma librería calcula la masa y hace nuestro trabajo menos pesado.
### Tiempo de caída
El tiempo de caída es algo curioso, básicamente, nosotros controlamos la simulación de la caída libre, también controlamos por cuanto tiempo la simulamos.
Dentro del código tenemos estas 2 líneas:
```cpp
	float timeStep = 1.0f/60.0f;
	for(int32 i=0; i<180; ++i)
```
La primera está dividiendo 1 segundo en 60 partes, el paso de tiempo individual de la simulación es equivalente a 1/60 segundos. Entre más pasos tengamos por segundo, más instantes podremos analizar, pero más pesado será.
La segunda línea es un ciclo for, o mínimo sus condiciones, la condición $i<180$ nos indica cuantos pasos individuales de tiempo vamos a ver.
Es decir, aquí estamos viendo 3 segundos de la simulación.
## ¿Qué podemos esperar? (a grandes rasgos)
 Podemos esperar que el cubo disminuya su posición en la coordenada y, debido a la fuerza que la gravedad ejerce sobre el. Además, podemos esperar que la posición cambie cada vez más rápido, porque va a lograr tener cierta aceleración y aumentará su velocidad de forma gradual hasta colisionar con otro cuerpo.
## Código
```cpp
#include <iostream>  
#include <Box2d/Box2d.h>  
  
int main(){ //cubo callendo en la superficie de la luna  
    //Mundo    //Creando el vector de gravedad y el mundo    
    b2Vec2 gravity(0.0f, -9.81f);  
    b2World world(gravity);  
  
    //Piso  
    //Características del cuerpo    b2BodyDef groundBodyDef;  
    groundBodyDef.position.Set(0.0f, -10.0f);  
  
    //Creación del cuerpo  
    b2Body* groundBody = world.CreateBody(&groundBodyDef);  
  
    //Crear la forma del cuerpo  
    b2PolygonShape groundBox;  
    groundBox.SetAsBox(50,1);  
  
    //  
    groundBody->CreateFixture(&groundBox, 0.0f);  
  
    //Cuerpo dinamico (caja)  
    //Características de la caja    b2BodyDef bodyDef;  
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
    fixtureDef.friction = 0.3f;  
  
    //Cargamos el fixtureDef que creamos  
    body->CreateFixture(&fixtureDef);  
  
  
    //Caida  
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
### Gravedad
Dentro del código, creamos un vector que va a guardar el valor de la gravedad que queremos simular. Este valor va a ser guardado en el componente $y$ del mismo y será negativo.
Es el parámetro con el que creamos el mundo en la segunda línea.
```cpp
    //Mundo    //Creando el vector de gravedad y el mundo    
    b2Vec2 gravity(0.0f, -9.81f);  
    b2World world(gravity);
```
### Altura (punto inicial del cubo)
Dentro del código creamos 2 cuerpos, uno es un simple cuerpo estático al que le dimos una altura de -10m, este funcionará como piso.
```cpp
	//Piso  
    //Características del cuerpo    b2BodyDef groundBodyDef;  
    groundBodyDef.position.Set(0.0f, -10.0f); 
```

El otro es un cuerpo dinámico, la caja que vamos a analizar. Esta es creada en el punto (0.0f,20.0f). A 20 metros de altura del origen.
```cpp
	//Cuerpo dinamico (caja)  
	//Características de la caja  
	b2BodyDef bodyDef;  
	bodyDef.type = b2_dynamicBody;  
	bodyDef.position.Set(0.0f,20.0f);
```
### Densidad
La densidad es cargada como atributo dentro de un fixtureDef. Que guarda los atributos y forma para posteriormente ser asignado al cuerpo.
Para este ejercicio le asignamos una densidad arbitraria de $1kg/m^3$.
```cpp
	//Hacemos una fixture def aparte para cambiar cada característica del cuerpo  
    b2FixtureDef fixtureDef;  
    fixtureDef.shape = &dynamicBox;  
    fixtureDef.density = 1.0f;  
    fixtureDef.friction = 0.3f;  
  
    //Cargamos el fixtureDef que creamos  
    body->CreateFixture(&fixtureDef);  
```
### Masa
Como fue mencionado, la masa es calculada dentro de la misma librería, nosotros no la asignamos, pero es necesario asignar un valor de densidad y una forma para que nuestro cuerpo tenga masa.
### Tiempo de caída
El tiempo de caída es asignado en la siguiente parte del código, podemos verlo como tiempo de simulación, instantes de simulación u otras formas, pero lo importante es poder controlarlo.

- $timeStep$ indica cuantos pasos de simulación se dan por segundo (en cuantos instantes se divide un segundo real).
- $i<180$ el 180 indica cuantos instantes de la simulación vamos a observar.
```cpp
	//Caida  
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
```