NeoSaveyBot
===========

This is my attempt to clone (and enhance!) SaveyBot.

[Here](https://github.com/Always8bit/SaveyBot/) is the original SaveyBot.


Note that this uses the POSIX C API for networking, so it won't work in
Windows out of the box.  You'll need something like Cygwin (or maybe
msys2), I think, if you want to run this in Windows.



Required libraries:  jsoncpp, gmp, gmpxx

Also, liborangepower (my own library) is required, but that's taken care of
with git subrepo.

I highly recommend git subrepo, by the way.
