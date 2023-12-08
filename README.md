<div align="center">
 <h1>Torres de Hanoy</h1>
  <p>Ramirez Garcia Diego Andres</p>
</div>

> ## (release 1.0v)

<h2>Descripción</h2>
<p>Este simulador consta de tres torres y tres toroides (discos). Inicialmente, 
  los tres toroides están en la primera torre, ordenados de mayor a menor, de abajo hacia arriba.
  
  Al presionar la tecla SPACE, se inicia una animación que traslada los toroides uno a uno hacia 
  la torre opuesta (torre 3). Al concluir la animación, los tres toroides quedan en la torre número 3. 
  Esta animación es paso a paso y a una velocidad constante para una apreciación óptima.
  
  La secuencia se repite al presionar SPACE nuevamente, devolviendo los tres discos a la torre 1 
  para comenzar una nueva simulación.</p>
<div align="center">
  <img src="https://github.com/DiegoAndresRa/Torres_de_Hanoy/blob/main/torres_hanoy.gif" alt="gif of simulation" width=400>
</div>  
<h2>Bibliotecas y Dependencias</h2>
<div align="center">

  | Ejecutable                              | Entorno                                               |
  |------------------------------------------|-------------------------------------------------------|
  | glew32.dll                               | iostream                                            |
  | assimp-vc140-mt.dll                      | cmath                                               |
  | Carpeta con los shader utilizados        | GL/glew.h                                           |
  | Carpeta con los modelos (obj)             | GLFW/glfw3.h                                        |
  |                                          | glm/glm.hpp, glm/gtc/matrix_transform.hpp, glm/gtc/type_ptr.hpp |
  |                                          | "stb_image.h                                       |
  |                                          | "SOIL2/SOIL2.h                                     |
  |                                          | Shader.h                                           |
  |                                          | Camera.h                                           |
  |                                          | Model.h                                            |

</div>


<div style="display: flex; justify-content: space-between; align-items: center;">
  <img align="left" src="https://github.com/devicons/devicon/blob/master/icons/visualstudio/visualstudio-plain.svg" title="visualstudio" alt="visualstudio" width="40" height="40"/>
  <h2>Configuración de entorno</h2>
</div>

<p>El proyecto fue desarrollado mediante el IDE Visual Studio, ello implica requerir una omputadora
    con sistema operativo Windows. A continuación se muestra la configuración de la solución para poder 
    manipular el código fuente.</p>

* Paso 1: En el apartado de propiedades, nos dirigimos a *C/C++ > General > Additional Include Directories*. 
En esta opción añadimos las direcciones donde están nuestras bibliotecas y dependencias adicionales.
```
$(SolutionDir)/External Libraries/GLEW/include
$(SolutionDir)/External Libraries/GLFW/include
$(SolutionDir)/External Libraries/glm
$(SolutionDir)/External Libraries/assimp/include
```
* Paso 2: Nos dirigimos a *Linker > General > Additional Library Directories*. En esta opción añadimos las 
direcciones donde están nuestras bibliotecas.

```
$(SolutionDir)/External Libraries/GLEW/lib/Release/Win32
$(SolutionDir)/External Libraries/GLFW/lib-vc2015
$(SolutionDir)/External Libraries/glm
$(SolutionDir)/External Libraries/SOIL2/lib
$(SolutionDir)/External Libraries/assimp/lib

```
* Paso 3: Y finalmente en Properties > Linker > Input > Aditional Dependencies colocamos las últimas dependencias. 
Si en el apartado ya tenemos algunas dependencias, adicionamos estas antes o después pero separadas por punto y coma (;).
```
soil2-debug.lib;assimp-vc140 mt.lib;opengl32.lib;glew32.lib;glfw3.lib
```

<h2>Instalación</h2>
<p> En una PowerShell ejecute las siguientes sentencias</p>

```PowerShell
$ git clone https://github.com/DiegoAndresRa/Tower_of_Hanoy.git
$  cd '.\Tower_of_Hanoy\Executable Program\'
$ .\ProyectoFinal.exe
```
<h2>Manual de usuario</h2>
<p>A continuación se muestra una tabla con las teclas y su correspondiente función dentro del programa; 
  la tecla space es propia de la animación y  las demás son adicionales para una mejor interacción con el usuario.</p>

<div align="center">
    <table>
    <tr>
      <th>Teclas</th>
      <th>Acción</th>
    </tr>
    <tr>
      <td>Space</td>
      <td>Inicio de la simulación</td>
    </tr>
    <tr>
      <td>Space (de nuevo)</td>
      <td>Reinicia la simulación</td>
    </tr>
    <tr>
      <td>A</td>
      <td>Mueve la cámara a la izquierda</td>
    </tr>
    <tr>
      <td>D</td>
      <td>Mueve la cámara a la derecha</td>
    </tr>
    <tr>
      <td>W</td>
      <td>Mueve la cámara hacia abajo</td>
    </tr>
    <tr>
      <td>S</td>
      <td>Mueve la cámara hacia arriba</td>
    </tr>
  </table>
</div>


  
</div>
