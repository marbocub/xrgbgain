What is this?
=============

This program is a tool to adjust the brightness/contrast/gamma of each RGB color for XF86 graphics devices.

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
    
     -1, --rgamma <value>             set Red   Gamma value
     -2, --ggamma <value>             set Green Gamma value
     -3, --bgamma <value>             set Blue  Gamma value
     -R, --rcontrast <parcent>        set Red   Contrast value, range 0-100
     -G, --gcontrast <parcent>        set Green Contrast value, range 0-100
     -B, --bcontrast <parcent>        set Blue  Contrast value, range 0-100
     -r, --rbrightness <parcent>      set Red   Brightness value, range 0-100
     -g, --gbrightness <parcent>      set Green Brightness value, range 0-100
     -b, --bbrightness <parcent>      set Blue  Brightness value, range 0-100

### Gamma value
Sets the gamma curve to the value specified at 'value'. The default value is 1.0.

### Contrast value
Set the maximum brightness point of the gamma ramp curve to the value specified in 'parcent'. The value range is 0-100 and the default value is 100.

### Brightness value
Set the minimum brightness point of the gamma ramp curve to the value specified in 'parcent'. The value range is 0-100 and the default value is 0.

Example
-------

    $ xrgbgain -R 88 -G 80 -1 0.95 -2 0.92
    
            gamma   bright  contrast
    Red      0.95      0 %      88 %
    Green    0.92      0 %      80 %
    Blue     1.00      0 %     100 %

This example is correct the white point of a laptop the LCD was turned yellow.

License
=======

This tool is licensed under the GNU GPL v2.
