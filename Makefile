all:
	$(MAKE) $(MFLAGS) -C src/driver
	$(MAKE) $(MFLAGS) -C src/userapp/libusb_app
	$(MAKE) $(MFLAGS) -C src/userapp/driver_app
	$(MAKE) $(MFLAGS) -C tests/test_config
	$(MAKE) $(MFLAGS) -C tests/test_letters
	$(MAKE) $(MFLAGS) -C tests/test_libusb
	$(MAKE) $(MFLAGS) -C tests/test_pbm

clean:
	$(MAKE) $(MFLAGS) -C src/driver clean
	$(MAKE) $(MFLAGS) -C src/userapp/libusb_app clean
	$(MAKE) $(MFLAGS) -C src/userapp/driver_app clean
	$(MAKE) $(MFLAGS) -C tests/test_config clean
	$(MAKE) $(MFLAGS) -C tests/test_letters clean
	$(MAKE) $(MFLAGS) -C tests/test_libusb clean
	$(MAKE) $(MFLAGS) -C tests/test_pbm clean
