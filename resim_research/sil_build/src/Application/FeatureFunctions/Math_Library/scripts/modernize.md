Modernize a code base from using Shared Toolbox to be using Ml MathLibrary
==========================================================================

The python script can be started in a root directory of a project. It will
recursively find all .c, .h, .cpp and .hpp files.
In each of these files the following modernizations will be done:

* Find function calls and macro calls that have been renamed and apply the renaming
* Remove all includes to deprecated headers (old names starting with st_ or even older ones without a prefix)
* Based on the called functions and macros derive the needed headers
* Add includes for the needed headers at the end of the already included headers

Since the Ml_MathLibrary are an external dependency for a user the

~~~.h
#include <header.h>
~~~

style is used. Tools like TiCS that compute a fan out metric will produce more meaningful
numbers if they can distinguish between internal (quoted) and external (pointy brackets)
includes.

The python script does not implement a C/C++ language parser, therefore do not expect the
results to be perfect. It should still do a big portion of the work automatically for you.

How to use the script
---------------------

Make sure you have no important changes in your workspace, stash or check in all changes.
By doing so you ensure that you can easily revert using git/plastic if the script produces
bad results for you.
In a terminal move your current directory to a suitable root directory. This directory should
be suited to be recursively searched for .c, .h, .cpp and .hpp files.
If such a directory does not exist you may start the script several times from different folders.

The script will put the headers after the copyright comment if it does not find a good place.
Therefore it might be a good idea to use ST_SharedTools'add_copyright_comment.py' first
to ensure that all source files do have such a comment.

Run the script:

~~~cmd
python <path-to-Ml_MathLibrary-workspace>/scripts/modernize.py
~~~

Review the results, try to build, run your unit tests.
Fix any issues caused by the script, if there are improvements needed to the script raise an
issue in [AIG](https://jiraprod.aptiv.com/projects/AIG)
