# Examen parcial 2
 Este programa fue creado usando box2D para simular una máquina de Rube Goldberg, haciendo uso de SFML para la creación de una interfaz gráfica.

 Dentro del mismo se encuentran cuerpos estáticos, cinemáticos y dinámicos, joints, uso de eventos, clases y objetos, además de threads (ligeramente) para controlar la velocidad en la que se está visualizando/simulando la máquina.

## Descripción del problema
Una máquina de Rube Goldberg es un aparato excesivamente sofisticado que realiza una tarea muy simple de una manera deliberadamente indirecta y elaborada, normalmente haciendo uso de una reacción en cadena.

# Solución (en partes)
Dentro del código se hace uso de clases para facilitar la creación de cuerpos customizables de box2D con funciones de SFML integradas.

## Planteamiento inicial de máquina
Como primer paso, pensé en la forma de incluir las joints y número mínimo de cuerpos a forma de crear una máquina algo divertida.
El concepto en sí cambió bastante al llegar a la build final, pero incluí el archivo de diseño inicial dentro del proyecto en GitHub.

El cambio más grande sería que al final decidí buscar que la máquina se destruyera en varias partes, a forma de no poder ser reutilizable en su estado final.

Esta decision llegó gracias a dificultades implementando algunos elementos de la simulación. Pero fue divertido implementarlo un poco más de lleno en el funcionamiento, la pelota final por ejemplo, no puede regresar a su posición inicial, las pelotas de contrapeso tampoco, algunas plataformas simplemente se desaparecen, etc.
## Clases
#### Clase Cube
Cambios notables son la adición de variables de entrada para el constructor de la clase cubo (anteriormente explicada en 'Ventanas_SFML'), en específico las variables de densidad y ángulo de rotación y tipo de cuerpo.
Agregué estas variables de entrada para facilitar el crear cuerpos estáticos con rotación variada, cuerpos dinámicos con densidades diferentes y con el propósito de poder escoger qué tipo de cuerpo crear dentro del constructor.

Escogiendo qué tipo de cuerpo asignar:
```cpp
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
```

Esos fueron los únicos cambios implementados para la clase cubo desde la asignación de ventanas SFML.

#### Clase Circle
La clase circulo fue anteriormente mencionada en la asignación de 'Joints'.
La clase circulo es en extremo parecido a la clase cubo, solo que crea una forma de círculo en lugar de una caja de la siguiente forma:
```cpp
b2CircleShape shape;  
shape.m_radius = Radius/SCALE;  
shape.m_p.Set(0,0);
```

#### Nomenclatura en la creación de objetos
Se menciona dentro del código:
```cpp
//Create the bodies using the cube and circle classes, syntax being:  
//  - Static body (box)        - SBOX_Name  
//  - Static body (circle)     - SCI_Name  
  
//  - Kinematic body (box)     - KBOX_Name  
//  - Kinematic body (circle)  - KCI_Name  
  
//  - Dynamic body (box)       - DBOX_Name  
//  - Dynamic body (circle)    - DCI_Name
```

#### Offset para la máquina completa
Las variables de offset se llaman cada que se crea un objeto, para tener la capacidad de cambiar la posición en la que se crean las formas.
```cpp
//Offset to move the entire machine  
float x_offset =200;  
float y_offset =200;
```

Ejemplo de la creación de elementos usando la clase de cubo.
Para los valores de x, y se suman los offsets.
```cpp
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
```
#### Cada vez que se crea un objeto dentro del código
```cpp
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
    //We update all the static bodies declared here to avoid doing so in every cycle    SBOX_Stage1.update();  
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
      
    //Create the dynamic circles that are used as the movable objects that interact with others in the machine  
    Circle DCI_PelotaInicio(world, x_offset+50, y_offset+50, 20, sf::Color::Red, 3, 4);  
    Circle DCI_PelotaInicio2(world, x_offset+50, y_offset+150, 20, sf::Color::Red, 3, 3);  
    Circle DCI_PelotaInicio3(world, x_offset+50, y_offset+0, 15, sf::Color::Red, 3, 3);  
    Circle DCI_PelotaInicio4(world, x_offset+25, y_offset+175, 10, sf::Color::Green, 3, 1);
```
#### Cada vez que se crea un objeto que va dentro de un joint y los joints en sí en el código
##### Polea
```cpp
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
```
##### Revolución
```cpp
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
```
##### Prismática
```cpp
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
```
##### Distancia
```cpp
//Creation of joint between parts of the Prismatic joint and the gear joint  
#pragma region Distance joint creation  
    b2DistanceJointDef DistjointDef;  
    DistjointDef.Initialize(DBOX_PrismMove.getBody(), DBOX_PulleyPlatform2.getBody(), DBOX_PrismMove.getBody()->GetPosition(), DBOX_PulleyPlatform2.getBody()->GetPosition());  
  
    DistjointDef.collideConnected = false;  
    DistjointDef.stiffness = 10;  
  
    //Actually create the joint in world space  
    b2DistanceJoint* joint = (b2DistanceJoint*)world.CreateJoint(&DistjointDef);  
  
#pragma endregion
```
## Ciclo de dibujo
Tomando el ciclo de dibujo de las asignaciones de ventanas SFML y joints, realicé algunos cambios pequeños.
### Tiempo de espera manipulable
Dentro del ciclo de dibujado, como última instrucción implementé:
```cpp
//Wait for x milliseconds every time the window is drawn  
std::this_thread::sleep_for(std::chrono::nanoseconds(timeJumps*timeMultiplier));
```
Esta instrucción es controlada con dos variables, una que no varía (time jumps, o qué tan grandes son los saltos de tiempo) y cuantos saltos de tiempo se usan.

Estas variables son creadas como variables globales, que se actualizan con funciones void:
```cpp
//These two variables are used to change the framerate the window is drawn in  
//particularly the time it waits after drawing the window  
//Base time  
int timeJumps = 160;  
//Multiplier for base time, we control this in the window drawing cycle  
int timeMultiplier = 5;

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
```
Esto para como mínimo esperar 160 nanosegundos, con la capacidad de crecer el valor que se espera cada ciclo.
### Llamada a eventos update para cuerpos estáticos
Para los cuerpos estáticos, como no cambian de posición o rotación realmente, no es necesario actualizarlos cada vez que la ventana se dibuja. No es necesario estar llamando a la función de update para estos objetos.

Por esta razón, los incluí una única vez después de crear un objeto estático (ejemplo de cuando se crean las partes fijas de la máquina):
```cpp
    //We create the static bodies that'll be part of the machine's structure (static walls and floors for the machine)  
#pragma region Structure_Static_Body_Creation  
    //Start ramp  
    Cube SBOX_Stage1(world, x_offset-2.5f, y_offset+205, 300, 10, sf::Color::White, 1,1, 30);  
    
    //...
    
    //Wall that will stop the advance of the pulley ball  
    Cube SBOX_Stage10(world, x_offset+530, y_offset+225, 10, 200, sf::Color::White, 1, 1, 0);  
  
    //Update the static walls and floors for the machine  
    //We update all the static bodies declared here to avoid doing so in every cycle    SBOX_Stage1.update();  
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
```
### Cosas que no cambian
Las funciones de update y draw se llaman para el resto de objetos, updates antes del window clear y draws después del window clear y antes del window display (pseudocódigo):
```cpp
Cube.update(); //para todos los cubos
Circle.update(); //para todos los circulos

window.clear();

Cube.draw(window); //para todos los cubos
Circle.draw(window); //para todos los circulos

window.display();

sleep_for(nanoseconds(x))
```
## Otros
### Puerta que se abre al empujar una bola
Dentro de la simulación tenemos una puerta doble que toma la velocidad de un objeto activador y la adapta, la primera parte (lado derecho de la puerta) toma el doble de la velocidad y la segunda parte (lado izquierdo) toma el doble negativo de la velocidad del activador.

Como son objetos cinemáticos se deben actualizar, estos si pueden cambiar de posición.
```cpp
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
```