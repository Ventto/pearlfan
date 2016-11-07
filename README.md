

PearlFan
=========

[![Build Status](https://travis-ci.org/Ventto/pearlfan.svg?branch=master)](https://travis-ci.org/Ventto/pearlfan)
[![License](https://img.shields.io/badge/license-GPLv3-blue.svg?style=flat)](https://github.com/Ventto/pearlfan/blob/master/LICENSE)
[![Status](https://img.shields.io/badge/status-Release_v1.0-blue.svg?style=flat)](https://github.com/Ventto/pearlfan/releases)

*PearlFan provides a GNU/Linux Kernel driver and a libusb application for the "[PEARL.fr](https://www.pearl.fr/article/PX5939/ventilateur-usb-programmable-avec-message-defilant
)" USB LED fan.*

## Dependencies

Archlinux:

* *linux-headers* - Header files and scripts for building modules for Linux kernel
* *libusb-1.0* - Library that provides generic access to USB devices
* *netpbm* - Toolkit for manipulation of graphic images (with libraries and header files)

Ubuntu:

* *linux-headers-$(uname -r)*
* *libnetpbm10-dev* - Netpbm libraries and header files
* *libusb-1.0-dev* - Library that provides generic access to USB devices

## Install

### Package (AUR)

```
$ yaourt -S pearlfan (or)
$ pacaur -S pearlfan
```

### Manually

* Kernel module and its user application:

```
$ make -f Makefile.module
$ sudo make -f Makefile.module install
```

* Libusb application:

```
$ make -f Makefile.libusb
```

## Execute

### Synopsis

```
pearlfan [-f] [-c FILE]
pearlfan [-f] [-d DIRECTORY]
```

### Options

```
Setting:
  -c:  Displays at most eight images with transition effects described in FILE.
  -d:  Displays at most eight .PBM images (156x11) in DIRECTORY.

Mode:
  -f:  Enables fast-mode. Disables all others effect transitions.

Miscellaneous:
  -h:  Prints this help and exits.
  -v:  Prints version info and exits.
```

## Configuration File

### Example

This an example of a pfan's configuration file

```
pacman.pbm  +2-2-0
mario.pbm   +1-1-6
circle.pbm  +0-0-0
cols.pbm    +3-3-6
full.pbm    +2-5-0
```

### Specifications

Each configuration file consists of the following:

* line pattern: `{imagename}.pbm+{effects}`
* Whitespace (blanks and TABs) between `{imagename}.pbm` and `+` are ignored (cf. example above)
* The image must be in the same directory than the configuration file
* The image must be [.PBM](http://netpbm.sourceforge.net/doc/pbm.html) image
* The image size must be: width=11px, height=156px
* {effects}: the fan provides transition effects when displaying and switching images
* Effect's pattern: `{open}-{close}-{beforeclose}`
* Open, close and beforeclose's exclusive values are described in the table below
* The fan can display at most 8 images

### Effect values

| Effects/Value | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|---|
| **open** | right-left | left-right | 2-side | red-carpet | top-bottom | bottom-top | fast-mode |
| **close** | left-right | right-left | 2-side | red-carpet | top-bottom | bottom-top | x |
| **before-close** | none | x | x | x | turn right-left | x | turn left-right |

* x: unused and considered as invalid value
* none: disable
* fast-mode: skip the 'open' and 'before-close' transition effects
