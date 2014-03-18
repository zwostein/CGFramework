## Compiling the project

### Unix like operating systems

Change to the project's root directory.

Create and change into the build directory:

	mkdir build
	cd build

#### Using the system installation of GLFW

Generate Makefiles and compile the project:

	cmake ..
	make

#### Using an internal copy of GLFW

First clone the GLFW sources to the glfw directory of the project:

	git submodule init
	git submodule update

Then generate the Makefiles and compile the project:

	cmake -DUSE_INTERNAL_GLFW=TRUE ..
	make


### Compiling on Windows using Visual Studio

Change to the project's root directory.

Create and change into the build directory:

	mkdir build
	cd build

First clone the GLFW sources to the glfw directory of the project:

	git submodule init
	git submodule update

Then generate the project files:

	cmake -G "Visual Studio 12" -DUSE_INTERNAL_GLFW=TRUE ..

Open the generated "Solution" file.
In the "Solution Explorer" right-click on the "Solution" and select "properties".
Select "cgframework" as the "Single startup project".
