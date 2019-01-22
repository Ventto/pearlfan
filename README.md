
PearlFan
=========

[![Build Status](https://travis-ci.org/Ventto/pearlfan.svg?branch=master)](https://travis-ci.org/Ventto/pearlfan)
[![License](https://img.shields.io/badge/license-GPLv3-blue.svg?style=flat)](https://github.com/Ventto/pearlfan/blob/master/LICENSE)

*PearlFan is a tool to draw on a [PEARL](https://www.pearl.fr/article/PX5939/ventilateur-usb-programmable-avec-message-defilant
)'s USB LED fan.*

[![](doc/pearlfan.gif)](https://www.pearl.fr/article/PX5939/ventilateur-usb-programmable-avec-message-defilant)

# Installation

## Package

```bash
$ pacaur -S pearlfan
```

## Manually

* Dependencies:

```bash
$ pacman -S libusb-1.0 netpbm                         (archlinux)
$ apt-get install libusb-1.0-0-dev libnetpbm10-dev    (ubuntu)
```

* Build:

```
$ make
```

# Usage

```
pearlfan -c FILE  [-f]
pearlfan -d DIRECTORY [-f]
pearlfan -t TEXT [-f]

Setting:
  -c:  Displays at most eight images with transition effects described
       in the given config FILE.
  -d:  Displays at most eight .PBM images (156x11) in DIRECTORY.
  -t:  Draws TEXT (standard ASCII table). Words are not split, only huge words.

Mode:
  -f:  Enables fast-mode. Disables all others effect transitions.

Miscellaneous:
  -h:  Prints this help and exits.
  -v:  Prints version info and exits.
```

# Examples

```bash
$ ls images/
pacman.pbm
mario.pbm
file.config

$ cat file.config
pacman.pbm  +2-2-0
mario.pbm   +1-1-6

$ pearlfan -c images/file.config    (or)
$ pearlfan -d images/             (without config file)
```

# Configuration File

## Sample

This an example of a pfan's configuration file

```
pacman.pbm  +2-2-0
mario.pbm   +1-1-6
circle.pbm  +0-0-0
cols.pbm    +3-3-6
full.pbm    +2-5-0
```

## Specifications

Each configuration file consists of the following:

* line pattern: `{image_basename}.pbm+{effects}`
* Whitespace (blanks and TABs) between `{image_basename}.pbm` and `+` are ignored (cf. example above)
* The image must be in the same directory than the configuration file
* The image must be [.PBM](http://netpbm.sourceforge.net/doc/pbm.html) image
* The image size must be: width=11px, height=156px
* {effects}: the fan provides transition effects when displaying and switching images
* Effect's pattern: `{open}-{close}-{beforeclose}`
* Open, close and beforeclose's exclusive values are described in the table below
* The fan can display at most 8 images

## Effect values

| Effects/Value | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| **opening** | right-left | left-right | 2-side | red-carpet | top-bottom | bottom-top | fast-mode |
| **closing** | left-right | right-left | 2-side | red-carpet | top-bottom | bottom-top | n/a |
| **before-closing** | do nothing | n/a | turn left-right | n/a | n/a | n/a | turn right-left |

* *opening/right-left*: means appearing from right to left
* *closing/right-left*: means disappearing from right to left
* *fast-mode*: skip the 'open' and 'before-closing' transition effects

# TODO

- [ ] Turn the project into a cross-platform lib to draw on USB LED fans
- [ ] Rename the project
