SaveyBot
========

This is my attempt to clone (and enhance!) SaveyBot.

[Here](https://github.com/Always8bit/SaveyBot/) is the original SaveyBot.


Some Commands
-------------

### .load
* .load &lt;username>
  * This command tells you how many savestates &lt;username> owns!
* .load &lt;slot\_number>
  * This command shows savestate #&lt;slot\_number>\!


### .save
* .save &lt;message>
  * This commands saves &lt;message> to the lowest (positive) available
  slot!
* .save &lt;slot\_number> &lt;message>
  * This command saves &lt;message> to savestate #&lt;slot\_number>, which
  can overwrite an existing savestate **only** if the user owns that
  savestate!


### .road
* .road
  * This command loads a random savestate!
* .road &lt;username>
  * This command loads a random savestate owned by &lt;username>\!


### .remove
* .remove &lt;slot\_number>
  * If the user owns savestate #&lt;slot\_number>, then this command will
  delete that savestate!


### .whois
* .whois &lt;slot\_number>
  * This command tells you who owns savestate #&lt;slot\_number>\!


### .date
* .date &lt;slot\_number>
  * This command tells you when savestate #&lt;slot\_number> was saved, if
  it is known.  Savestates made using original SaveyBot do not have a known
  date saved!


Building Notes
--------------

Note that this implementation uses the POSIX C API for networking, so it
won't work in Windows out of the box.  You'll need something like Cygwin
(or maybe msys2), I think, if you want to run this in Windows.


Required libraries:  jsoncpp, gmp, gmpxx

Also, liborangepower (my own library) is required, but that's taken care of
with git subrepo.

I highly recommend git subrepo, by the way.
