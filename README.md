What is this?
=============

This program is a simple tool to adjust the brightness/contrast/gamma correction of each RGB color for Xserver with XF86 graphics drivers. 
It provides adjustment features similar to the Intel GMA Driver and other graphics drivers for Windows.

Requirements and Limitations
============================

This tool works with the XF86VM library.
In the many of Linux systems, you will be able to install the library by using the existing package management system.

    ubuntu: sudo apt-get install libxxf86vm-dev
    
    centos: sudo yum install libXxf86vm-devel

We have verified that the run correctly on Ubuntu-12.04LTS.

Building and Installing
=======================

Compile
-------

    cc xrgbgain.c -lXxf86vm -o xrgbgain

You need XF86VM headers and libraries installed.

Usage
=====

    Usage: xrgbgain [OPTIONS]
    
     -1, --rgamma <realnum>           set Red   Gamma value
     -2, --ggamma <realnum>           set Green Gamma value
     -3, --bgamma <realnum>           set Blue  Gamma value
     -R, --rcontrast <parcent>        set Red   Contrast value, range recommended 0-100
     -G, --gcontrast <parcent>        set Green Contrast value, range recommended 0-100
     -B, --bcontrast <parcent>        set Blue  Contrast value, range recommended 0-100
     -r, --rbrightness <parcent>      set Red   Brightness value, range recommended 0-100
     -g, --gbrightness <parcent>      set Green Brightness value, range recommended 0-100
     -b, --bbrightness <parcent>      set Blue  Brightness value, range recommended 0-100

### Gamma value
Set the gamma curve to the value specified at 'realnum'. The default value is 1.0.

### Contrast value
Set the maximum brightness point of the gamma ramp curve to the value specified at integer 'parcent'.
The range recommended is 0-100 and the default value is 100.

### Brightness value
Set the minimum brightness point of the gamma ramp curve to the value specified at integer 'parcent'.
The range recommended is 0-100 and the default value is 0.

Example
-------

    $ xrgbgain --rcontrast 88 --gcontrast 80 --rgamma 0.95 --ggamma 0.92
    or
    $ xrgbgain -B 88 -G 80 -1 0.95 -2 0.92
    
            gamma   bright  contrast
    Red      0.95      0 %      88 %
    Green    0.92      0 %      80 %
    Blue     1.00      0 %     100 %

This example is correct the white point of my laptop the LCD was turned yellow.

License
=======

This tool is licensed under the GNU GPL v2.
