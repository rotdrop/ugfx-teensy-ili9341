Proof-of-concept [ugfx](https://ugfx.io/) project for a [Teensy-3.2](https://www.pjrc.com/store/teensy32.html) and an [Adafruit 3.2" touchscreen TFT breakout board](https://www.adafruit.com/product/1743).

Starting point was to some extent the following thread: 

https://community.ugfx.io/topic/1000-generic-board-file-for-ili9341-and-the-arduino-ide-based-on-adadruits-generic-library-supporting-different-boards/?ct=1563776186

However, except for the uGfx double const issue and a custom uGfx "lld read-rectangle" addition all underlying libaries are unchanged.

*  Libraries are contained in the externals/ folder as Git sub-trees, with is more convenient for download than Git sub-modules; and less convenient for hacking and maybe questionable when it comes to copyright issues. Hope I did it right.

* The project comes as a GNU autotools project (autoconf, automake, libtool etc.)

* The uGfx library files are extracted from the uGfx build-system by running a GNU Makefile fragment an update on the fly if they have changed.

* "Externals" can be synchronized with the script `externals.sh` which parses `EXTERNALS.csv` and incorporates the listed external project as either Git sub-trees or -modules.
