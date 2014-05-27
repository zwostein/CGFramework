## Compiling the project

### Unix like operating systems

Change to the project's root directory.

Create and change into the build directory:

	mkdir build
	cd build

#### Using the system installation of GLFW/GLM

Generate Makefiles and compile the project:

	cmake ..
	make

#### Using an internal copy of GLFW and/or GLM

First clone the library sources into the the project directory:

	git submodule init
	git submodule update

Then generate the Makefiles and compile the project:

	cmake -DUSE_INTERNAL_GLFW=TRUE -DUSE_INTERNAL_GLM=TRUE ..
	make


### Compiling on Windows using Visual Studio and Git Bash

Open a Bash terminal in the project's root directory.

Create and change into the build directory:

	mkdir build
	cd build

Clone the library sources into the the project directory:

	git submodule init
	git submodule update

Then generate the project files either using cmake-gui or manually:

	cmake -G "Visual Studio 12" -DUSE_INTERNAL_GLFW=TRUE -DUSE_INTERNAL_GLM=TRUE ..

Open the generated "Solution" file.
In the "Solution Explorer" right-click on the "Solution" and select "properties".
Select "cgframework" as the "Single startup project".
