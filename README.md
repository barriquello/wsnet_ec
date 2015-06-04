# wsnet_ec
Wsnet port for Eclipse + mingW

This is a port of WSNET simulator available at http://wsnet.gforge.inria.fr/index.html.

The code has been modified in order to compile and link the project using Eclipse and MingW GCC.

How to use it:

1 - Install Eclipse CDT (e.g: Luna) : https://eclipse.org/cdt/downloads.php

2 - Install MingW: http://www.mingw.org/

3 - Install the folowing software libraries to MingW (it is required for compiling the simulator, but nbot the models)

    libglib2.0-0, libxml2, libgsl0, libiconv
    
4 -  The project is provided as an Eclipse workspace. To use it, clone the project and, when Eclipse starts, point it to project's workspace.


Organizaton:

-  The project is comprised by several eclipse projects (one is the simulator and the others are the models used by the simulator)
-  
-  Wsnet is a modular simulator, which loads the models at runtime. The models are compiled as shared libraris (*.dll)

License
Wsnet is licensed under the CeCILL Free Software License. Available at http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt






