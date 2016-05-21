all:
	$(MAKE) $(MFLAGS) -C src/driver
	$(MAKE) $(MFLAGS) -C src/userapp/libusb_app
	$(MAKE) $(MFLAGS) -C src/userapp/driver_app

clean:
	$(MAKE) $(MFLAGS) -C src/driver clean
	$(MAKE) $(MFLAGS) -C src/userapp/libusb_app clean
	$(MAKE) $(MFLAGS) -C src/userapp/driver_app clean
