Introduction {#mainpage}
========================

The MathLibrary offers functionality useful for active safety feature development.

Integration
-----------

### Plastic xLink

In order to use the MathLibrary as part of a bigger (plastic-) project do not copy paste the code from the MathLibrary repository.
Instead use a plastic xLink. It's best to use a partial xLink so that all the unit tests do not show up in the using repository.
In order to generate a partial xLink for the MathLibrary open a command window. Make sure the folder where the sub directory
of the MathLibrary shall be stored is active. Run the following command

~~~cmd
cm xlink Shared_Toolbox /Shared_Toolbox/Source br:/main/Release@10027594_09_Shared_Toolbox@plasticscm_prod1.delphiauto.net:8087
~~~

This will create a folder called 'Shared_Toolbox' that contains nothing but the actual production code of the MathLibrary.

### git

Use a submodule

### CMake

Do an add_subdirectory() into the folder holding this README.md file.

The Shared-Toolbox as well as its users needs to be able to include the reuse.h file matching the system.
When integrating the MathLibrary please add all include folders needed to the build using the CMake command

~~~cmake
include_directories()
~~~

CMake creates its own directory structure with each call to add_subdirectory().
The command include_directories() adds the given include directory to the current and all directories below of this
CMake directory structure.
include_directories() only has an effect after its been called. Targets created before the
call will not get the include directories.
Since many targets need the reuse.h included it might be good to do include_directories() in the root CMakeLists.txt

#### CMake options

See the file fast_math.md in the same folder as this README.md for more information on the CMake options the shared
toolbox offers.

### Make

The MathLibrary offers two .mak files to help with integration into a [gnu-make](https:\\de.wikipedia.org/wiki/GNU_Make) like system.

* shared_toolbox_generic.mak
* shared_toolbox_generic_legacy.mak

Both of these files define two variables:

* SHARED_TOOLBOX_INCLUDE_DIR
  * Shared-Toolbox directories to be added to the include directories
  * These folders get a previously defined SHARED_TOOLBOX_INCLUDE_DIR_PREPEND prepended
* SHARED_TOOLBOX_SUBDIRS
  * Shared-Toolbox directories containing Shared-Toolbox source files
  * These folders get a previously defined SHARED_TOOLBOX_SUBDIRS_PREPEND prepended

shared_toolbox_generic_legacy.mak  includes the legacy header folder

shared_toolbox_generic.mak  Does not include the legacy header folder

Thus the integration is as follows:

* Define SHARED_TOOLBOX_INCLUDE_DIR_PREPEND to be prepended to the SHARED_TOOLBOX_INCLUDE_DIR variable
* Define SHARED_TOOLBOX_SUBDIRS_PREPEND to be prepended to the SHARED_TOOLBOX_SUBDIRS variable
* Note: Depending on your build system you may want to add a '-I' to this path
* Include either shared_toolbox_generic_legacy.mak or shared_toolbox_generic.mak
  * This depends on the ability of the Shared-Toolbox users
* Append SHARED_TOOLBOX_INCLUDE_DIR to the build systems include directories
* Append SHARED_TOOLBOX_SUBDIRS_PREPEND to the build systems source directories

See Shared_Toolbox.mak for a sample.

Fast Math tables
----------------

The MathLibrary offers to speed up trigonometric functions (with some loss of precision)
by using precomputed tables.
While speeding up the computation they also use a substantial amount of memory, both in ROM and RAM.
Therefore the user of the MathLibrary can choose if these tables shall be present in the build.
Since this is a decision that needs to be made consistent in software in the loop and embedded builds
the MathLibrary enforces the decision to be made.
See [Fast Math](fast_math.md) For more information.

External links
--------------

[Jira](http:\\jiraprod.aptiv.com:8080/projects/AIG/summary)

[SRD](https:\\polarionprod1.aptiv.com/polarion/#/project/4118_6945_CustomerFunctionEngineeringComponents/wiki/SW_Requirements/SRD_Shared-Toolbox)

[Project Page](http:\\[pep.usinkok.northamerica.delphiauto.net](http://pep.aptiv.com)/projectdb/public/?page=project-tasks&pid=2850.6859)
