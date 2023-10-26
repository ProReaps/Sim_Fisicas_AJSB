# Articulaciones
Este programa simula por medio de varios archivos .cpp a algunos tipos de joints de box2D.

 Para poder cambiar cual tipo de joint se está simulando solo es necesario intercambiar el nombre del archivo .cpp dentro de CmakeLists.txt
## Tipo de problema
Cada una de las simulaciones contiene un tipo de joint, se controlan con las teclas:
#### w, a, s, d, r, z, 'espacio'
## Overview de Simulaciones (por nombre)
### DistanceJoint.cpp
Esta simulación contiene una articulación de distancia, dentro de la simulación se puede controlar el movimiento de una caja A usando las teclas **w, a, s, d**, espacio para devolver la caja al punto inicial de aparición y la tecla r para detener la velocidad por completo.

Cuando manipulamos el movimiento de la caja A, podemos notar que el único otro objeto visible, otra caja (B) la sigue, aunque cambia ciertos aspectos, como su velocidad angular con respecto al origen de la caja A.

Esta simulación se controla de la siguiente forma:
* W - Agregar velocidad hacia arriba
* A  - Agregar velocidad hacia la izquierda
* S  - Agregar velocidad hacia abajo
* D - Agregar velocidad hacia la derecha
* R  - Fijar la velocidad en 0
* " " -  Regresar a la posición inicial
* Z  - Salir

Creación de la articulación:
```cpp
//Create the cubes from Cube class
Cube cube(world, 400, 300);
Cube cuboDos(world, 200, 300);

//Declare the joint definition variable (maybe that is what this is, could be wrong)
b2DistanceJointDef jointDef;
jointDef.Initialize(cube.getBody(), cuboDos.getBody(), cube.getBody()->GetPosition(), cuboDos.getBody()->GetPosition());
jointDef.collideConnected = true;

//Actually create the joint in world space
b2DistanceJoint* joint = (b2DistanceJoint*)world.CreateJoint(&jointDef);
```
### RevoluteJoint.cpp
Esta simulación contiene una articulación de rotación, dentro de la simulación se puede controlar la rotación de lo que parece ser un cuerpo compuesto, que realmente es una articulación de rotación con un motor, lo que controlamos es la velocidad que tiene el motor al rotar.
Tenemos dos cuerpos que giran alrededor de un mismo eje. 

Esta simulación se controla de la siguiente forma:
* D  - Hace que gire en la dirección de las manecillas del reloj
* A  - Hace que gire en la dirección contraria a las manecillas del reloj
* " " -  Regresar a la velocidad inicial
* Z  - Salir

Creación de la articulación:
```cpp
//Create the cubes from Cube class  
Cube cube(world, 400, 300, 10.0f, 10.0f, sf::Color::Red);  
Cube cuboDos(world, 400, 300, 40.0f, 10.0f, sf::Color::Blue);  
  
//Declare the joint definition variable (maybe that is what this is, could be wrong)  
b2RevoluteJointDef jointDef;  
  
//Define main parameters  
jointDef.bodyA = cuboDos.getBody();  
jointDef.bodyB = cube.getBody();  
  
//Set the anchor for bodyA, this will be where it rotates around  
jointDef.localAnchorA.Set(0, 0);  
  
//Don't want it to collide with itself (in this case)  
jointDef.collideConnected = false;  
  
//Other important parameters, trying to turn the joint into a motor  
jointDef.maxMotorTorque = 10.0f;  
jointDef.motorSpeed = 0.1f*b2_pi;  
jointDef.enableMotor = true;  
  
//Actually create the joint in world space  
b2RevoluteJoint* joint = (b2RevoluteJoint*)world.CreateJoint(&jointDef);
```
### PrismaticJoint.cpp
Esta simulación contiene una articulación prismática, dentro de la simulación se puede controlar el movimiento de una caja A y una caja B.
Es posible mover la caja A de arriba hacia abajo y la caja B de izquierda a derecha.

Esta simulación implementa un suelo, que es otra caja y gravedad, curiosamente, la gravedad solo comienza a afectar a las primeras dos cajas (A y B) cuando se mueven inicialmente.
El cuerpo A tiene límites de movimiento con respecto al B.

Esta simulación se controla de la siguiente forma:
* W - Mover el cuerpo A hacia arriba
* A  - Agregar velocidad al cuerpo B hacia la izquierda
* S  - Mover el cuerpo A hacia abajo
* D - Agregar velocidad al cuerpo B hacia la derecha
* R  - Fijar la velocidad del cuerpo A en 0
* " " -  Fijar la velocidad del cuerpo B en 0
* Z  - Salir

Creación de la articulación:
```cpp
//Create the cubes from Cube class  
Cube cube(world, 350, 290, 10.0f, 50.0f, sf::Color::Red);  
Cube cube2(world, 200, 300, 60.0f, 40.0f, sf::Color::Blue);  
  
//This is the floor, we set its type as a static body  
Cube floor(world, -50000, 750, 500000, 150, sf::Color::Green);  
floor.getBody()->SetType(b2_staticBody);  
  
//Declare the joint definition variable (maybe that is what this is, could be wrong)  
b2PrismaticJointDef jointDef;  
  
b2Vec2 worldAxis(0.0f, 1.0f);  
  
jointDef.Initialize(cube.getBody(), cube2.getBody(), cube.getBody()->GetWorldCenter(), worldAxis);  
  
//Define main parameters  
jointDef.localAxisA.Set(0,1);  
jointDef.upperTranslation = 5.0f;  
jointDef.lowerTranslation = 1.25f;  
jointDef.enableLimit = true;  
jointDef.collideConnected = false;  
jointDef.localAnchorA.Set(-(cube.getSizeX()/(SCALE*2)), -2.3f);  
jointDef.localAnchorB.Set(cube2.getSizeX()/(SCALE*2)+1.0f, -1.9f);  
  
//Other important parameters, trying to turn the joint into a motor  
jointDef.maxMotorForce = 1.0f;  
jointDef.motorSpeed = 0.0f;  
jointDef.enableMotor = true;  
  
//Actually create the joint in world space  
b2PrismaticJoint* joint = (b2PrismaticJoint*)world.CreateJoint(&jointDef);
```
### WeldJoint.cpp
Esta simulación contiene una articulación de soldadura, dentro de la simulación se puede controlar el movimiento de una caja A, parecido a la forma en la que fue descrito en la explicación de DistanceJoint.cpp. Este archivo implementa un suelo y una articulación diferente, pero utiliza los mismos controles.

Aquí, podemos simular dos cuerpos unidos en un punto central de uno de sus lados, la forma principal en la que difiere de una articulación de distancia sería que, aunque la distancia sigue siendo la misma, el punto y trayectoria por la que pasa esta distancia no cambia. Las dos cajas siempre estarán unidas, conectadas en el mismo punto.

Esta simulación se controla de la siguiente forma:
* W - Agregar velocidad hacia arriba
* A  - Agregar velocidad hacia la izquierda
* S  - Agregar velocidad hacia abajo
* D - Agregar velocidad hacia la derecha
* R  - Fijar la velocidad en 0
* " " -  Regresar a la posición inicial
* Z  - Salir

Creación de la articulación:
```cpp
//Create the cubes from Cube class, here they're touching and given the different sizes and masses they should  
//fall slightly sideways, giving ample opportunity to see the weld joint in effect  
Cube cube(world, 300, 300, 20.0f, 50.0f, sf::Color::Red);  
Cube cube2(world, 310, 300, 50.0f, 40.0f, sf::Color::Blue);  
  
//This is the floor, we set the type as a static body  
Cube floor(world, -50000, 750, 500000, 150, sf::Color::Green);  
floor.getBody()->SetType(b2_staticBody);  
  
//Declare the joint definition variable (I am now almost sure of this, could this be character development?)  
b2WeldJointDef jointDef;  
  
//Initialize the joint, then set the anchors for both bodies, should be at about the midway point in y and  
//right up against each other in x  
jointDef.Initialize(cube.getBody(), cube2.getBody(), b2Vec2(cube.getBody()->GetWorldCenter().x+(cube.getSizeX()/2),cube.getBody()->GetWorldCenter().x));  
jointDef.localAnchorA.Set(cube.getSizeX()/(SCALE*2), 0);  
jointDef.localAnchorB.Set(-cube2.getSizeX()/(SCALE*2), 0);  
  
//Actually create the joint in world space  
b2WeldJoint* joint = (b2WeldJoint*)world.CreateJoint(&jointDef);
```
### WheelJoint.cpp
Esta simulación contiene una articulación de rueda, dentro de la simulación se puede controlar el movimiento la rueda A, parecido en parte a los controles de RevoluteJoint.cpp.

Aquí, podemos simular dos cuerpos unidos de cierta forma, realmente la caja (cuerpo B) es nuestra carroza simulada. Utilicé un constraint de rotación fija para hacer que el cuerpo B no pudiese rotar y desbalancearse.

La rueda es controlada como motor, realmente solo cambiamos los valores de la velocidad con los que gira, pero en lugar de solo estar rotando un cuerpo, estamos generando torque, lo cual nos deja hacer que el objeto compuesto (rueda A y circulo B) avance o retroceda.

Esta simulación se controla de la siguiente forma:
* D  - Hace que gire en la dirección de las manecillas del reloj, avanzando (derecha)
* A  - Hace que gire en la dirección contraria a las manecillas del reloj, retrocediendo (izquierda)
* R -  Regresar a la posición inicial del círculo.
* Z  - Salir

**Nota:**
Este código incluye su propio creador de objetos círculo.

Creación de la articulación:
```cpp
//Create the cubes from Cube class  
Circle circle(world, 375, 350, 12.5f, sf::Color::Red); //This one is the wheel  
Cube cube2(world, 400, 325, 40.0f, 25.0f, sf::Color::Blue); //This one is NOT a wheel  
  
//This is the floor, we set the type as a static body  
Cube floor(world, -50000, 750, 500000, 150, sf::Color::Green);  
floor.getBody()->SetType(b2_staticBody);  
  
//Set this one as a bit of a static one? We're just restricting rotation, not movement  
cube2.getBody()->SetFixedRotation(true);  
  
//Declare the joint definition variable (maybe that is what this is, could be wrong)  
b2WheelJointDef jointDef;  
  
//Define main parameters  
jointDef.bodyA = circle.getBody();  
jointDef.bodyB = cube2.getBody();  
  
//Set the anchor for bodyA, this will be where it rotates around  
jointDef.localAnchorA.Set(0, 0);  
//since it locally rotates around (0, 0), it'll rotate around its own center  
  
//Set the anchor for bodyB, this will be where the wheel/bodyA is at (I think)  
jointDef.localAnchorB.Set(-circle.getRadius()/(SCALE*2), (circle.getRadius()*2.5f)/(SCALE));  
  
//The movement axis for bodyA, it gets constrained to this spot with an upper and lower range  
jointDef.localAxisA.Set(0.0f, 1.0f);  
  
//It's a motor! maybe...  
jointDef.maxMotorTorque = 10.0f;  
jointDef.motorSpeed = 0.0f;  
jointDef.enableMotor = true;  
  
//This is for the wheel, the upper and lower range for its movement axis restrict.  
//It can move from -1 * localAxisA.y (lower) to  1 * localAxisA.y (upper)   
jointDef.lowerTranslation = 0;  
jointDef.upperTranslation = 0;  
jointDef.enableLimit = true;  
  
//I do not want to activate collisions for the connected bodies, since their shapes are squares there  
//will be points where they overlap (mostly while rotating)  
jointDef.collideConnected = false;  
jointDef.stiffness = 10.0f;  
  
//Actually create the joint in world space  
b2WheelJoint* joint = (b2WheelJoint*)world.CreateJoint(&jointDef);
```

Clase círculo:
```cpp
class Circle {  
public:  
    Circle(b2World &world, float x, float y, float Radius, sf::Color color){  
        radius = Radius;  
  
        b2BodyDef bodydef;  
        bodydef.position.Set(x / SCALE, y / SCALE);  
        bodydef.type = b2_dynamicBody;  
        body = world.CreateBody(&bodydef);  
  
        b2CircleShape shape;  
        shape.m_radius = Radius/SCALE*2;  
        shape.m_p.Set(0,0);  
  
        b2FixtureDef fixturedef;  
        fixturedef.shape = &shape;  
        fixturedef.density = 1.0f;  
        fixturedef.friction = 3.0f;  
        body->CreateFixture(&fixturedef);  
  
        circ.setRadius(Radius);  
        circ.setFillColor(color);  
    }  
    void update(){  
        circ.setPosition(SCALE*body->GetPosition().x, SCALE*body->GetPosition().y);  
    }  
    void draw(sf::RenderWindow &win) const{  
        win.draw(circ);  
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
    sf::CircleShape circ;  
    b2Body *body;  
    float size_x;  
    float size_y;  
    float radius;  
};
```

### Conclusiones
Increíblemente extenso este trabajo, aunque me parece que plantearlo con programación orientada a objetos es la clave de crear algo más completo.

Realmente tiene sentido, porque de esta forma nos es posible conectar múltiples funciones de librerías diferentes, como lo fue dentro de este trabajo.

Serían posibles cosas como implementar funciones que creen los polígonos hechos en la tarea anterior de forma automática, simplemente teniendo una escala.
