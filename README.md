# DreamyFan

DreamyFan is a GNU/Linux driver for an USB LED fan by PEARL.

![test](./fan.jpg)

## Dependencies

* libusb - Library that provides generic access to USB devices
* netpbm - A toolkit for manipulation of graphic images

## Compilation

Run `make`.

## Execution

Two different approaches: 

* `./src/userapp/libusb_app/`- runs the user app with libusb.
* `./src/userapp/driver_app/`- uses the kernel driver.

To execute one of user apps, get into one of the directories above.
And run the executable file with the config file path as argument.<br>

For instance:
```bash
$ cd ./src/userapp/libusb_app/
$ ./test config
```

## Load the driver

Load the driver by running the following shell script `./src/driver/script.sh`.

## Configuration file

The latter contains at most 8 lines. Each one for a fan display.

Line format: `+path+x/y/z`

in which `path` is a valid .pbm file path `.pbm`,
and `x`, `y` and `z` are different displaying effect options for the following actions: open, close and display.

## Authors

* Thomas Venries
* Franklin Mathieu
