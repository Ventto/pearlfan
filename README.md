
PearlFan
=========

[![Build Status](https://travis-ci.org/Ventto/bpep.svg?branch=master)](https://travis-ci.org/Ventto/pearlfan)
[![License](https://img.shields.io/badge/license-GPLv3-blue.svg?style=flat)](https://github.com/Ventto/pearlfan/blob/master/LICENSE)
[![Status](https://img.shields.io/badge/status-experimental-orange.svg?style=flat)](https://github.com/Ventto/pearlfan/)

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

## Build

* Kernel module and its user application:

```
$ make -f Makefile.module
```

* Libusb application:

```
$ make -f Makefile.libusb
```

## Install the kernel module


* Install the module and its udev rule file:

```
$ sudo make -f Makefile.module install
```

*  Reload udev rules and add the module with *insmod*:

```
$ sudo make -f Makefile.module load
```

## Execute

```
$ ./pfan <config_file>
```

## Configuration File

### Example

This an example of a pfan's configuration file

```
+images/pacman.pbm+2/2/6
+images/mario.pbm+1/1/6
+images/circle.pbm+0/0/0
+images/cols.pbm+3/3/6
+images/full.pbm+2/5/0
```

### How to write it ?

#### What about the pattern ?
The config's pattern is: `+{relative_img_path}+{effects}`
* relative\_img\_path: the relative image's path according to the config file's path
* effects: the fan provides transition effects when displaying and switching images
	* pattern: `{open}/{close}/{before_close}`
	* open: opening transition effect when the fan is beggining displaying an image
	* close: closing transition effect when the fan is ending displaying an image
	* before\_close: before displaying the next image the fan can make the image turn and more

#### Images

* Image format: [.PBM](http://netpbm.sourceforge.net/doc/pbm.html) image
* Image size: 11 x 156

**The fan can display at most 8 images. So there is at most 8 lines in the config file.**

#### Effects

| Effects/Value | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|---|
| open | right-left | left-right | ecrase | appear | top-bottom | bottom-top | fast-mode |
| close | left-right | right-left | ecrase | appar | top-bottom | bottom-top | fast-mode |
| before\_close | none | x | x | turn right-left | x | x | turn left-right |

* fast-mode: skip the opening and before_close transition
