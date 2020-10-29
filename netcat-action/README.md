netcat-action
=============

Example of writing a netcat application in ACTION!

It is distributed as an ATR image which should be mounted as:

* **D1** - Your favorite DOS. I use DOS XL 2.30p
* **D2** - netcat-action.atr

Files
=====

* NIO.ACT - The SIO bindings for FujiNet network I/O
* NC.ACT - The NetCat main program

What works
==========

* nopen() - works but something wrong with interrupt.
* nclose() - these send their commands.

What doesn't
============

* nwrite() - I think it's sending length byte on accident?
* nread()
* Interrupt handler may need tweaking.

Really need some help from an ACTION! coder which can work out what are probably fundamental misunderstandings coming from C.

Once ready, I'll put the source code files directly into github.