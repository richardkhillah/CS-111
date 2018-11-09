# CS111 Discussion Week 6
## Friday November 6, 2018
### Lab 4b
Using two sensors: temp and button  

#### Prereqs:
* poll(2)
	* Button
	* Temperature
* MRAA
	* to link with MRAA add `-lmraa` to search list **Q: search list = ??**
		* have been issues with lmraa.
		* Go to TA if I've got issues with lmraa and beaglebone
* Reference drawing about which ports to use
* Deliverables - standard
	* [ ] C source: runs on Beaglebone
		* supports commandline options:
			* `--period=#` - optional parameter that specifies sampling interval in seconds. default = 1sample/sec
			* `--scale=[C, F]` - optional parameter that specifies temperature scale reporting. default = degrees Fahrenheit
			* `--log=*filename*` - optional parameter that enables logging to filename.
	* [ ] Makefile:
		* [ ] default
		* [ ] check
		* [ ] clean
		* [ ] dist
	* [ ] README

#### lab4b details
* lab4b
	* build & run on BB
	* Interact w/ sensors
		* I/O functionsfrom MRAA
	* Read temp
		* default sampling time = 1 sec
		* default temp units = farenheight
	* Log option that saves result in file
		* interacts with button
		* when button pressed, end what's going on:
			* shutdown program
			* log one more line (of temperature)
			* log "SHUTDOWN"
		* commands: - Must be logged (if logging is enabled). 
			* `PERIOD=#` - number of seconds between sampling
			* `SCALE=[F,C]` - change tamperature reported
			* `STOP` - stop generating temperature reports (like putting a "pause")
			* `START` - start generating temperature reports
			* `LOG[text]`
				* ex: `LOG abcdefg` logs abcdefg to log file
			* `OFF` - should be timestamped
	* Using `AIO commands` for temperature sensor interactions
		* `mraa_aio_init` 
			* takes int as param.
			* input = I/O pin, i.e., AIO:1
		* `mraa_aio_read`
		* `mraa_aio_close`
		* `mraa_aio_context` to relay information between functions
	* Using `GPIO commands` for button interactions
		* `mraa_gpio_init` - similar to `mraa_aio_init`
			*  GPIO:60
		*  `mraa_gpio_read`
		*  `mraa_gpio_close`
		*  `mraa_gpio_context`
		* look into `mraa_gio_dir`
	* 
	
	should use short sampling time for button or use some sort of interrupt handler  
	going to be using  
	grading on this lab is strict