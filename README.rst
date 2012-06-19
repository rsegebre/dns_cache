DNSCACHE:
#########

A DNS Cache in C++ using a simple vector where every IP look-up is stored. If
the IP is not in the cache then a lookup to an outside DNS  is performed. It also calculates
some basic statistics on every look-up and displays them. 


To Compile:
===========
To compile using g++:

::
	g++ -o dnscache -c dnscache.cc

This will generate a file called 'dnscache', but it is not executable because it doesn't have a main.


TODO:
=====

- Add a basic test to show what the code does.
- Add an SCONSTRUCT file to make compilation easier.
 
