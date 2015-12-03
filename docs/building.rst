=========
Building
=========

Dependencies
==============================================

PROJECTNAME depends on the following Ubuntu packages:

* one
* two
* three


PROJECTNAME depends on the following external libraries:

* one
* two
* three

Example Instructions
==============================================

The following instructions assume that your current working directory starts
out as the ``PROJECTNAME`` directory within the project.

1. Create a new directory for building.
  
  .. code-block:: bash  

    mkdir ../build

2. Change into the new directory.
  
  .. code-block:: bash  

    cd ../build

3. Run CMake with the path to the project source.
  
  .. code-block:: bash  

    cmake ../PROJECTNAME

4. Run your desired build tool inside the build directory.
  
  .. code-block:: bash  

    make

4. Use ``make install`` to install the project.
  
  .. code-block:: bash  

    make install



