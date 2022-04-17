PLATOTerm99 Coleco Adam
=======================

PLATOTerm99 is a terminal emulator to access CYBIS services now available
on the Internet utilizing a FujiNet.

For the purposes of this documentation. PLATO and CYBIS are interchangeable
names for the same platform.

Requirements
------------
* Coleco Adam
* FujiNet

--------------------------------------------------------------
What services are currently available to access via PLATOTerm?
--------------------------------------------------------------
As of writing this preliminary documentation (April 2022), there are
two major CYBIS systems running. CYBER1.ORG and IRATA.ONLINE.

---------------------------
WHAT IS PLATO? (aka CYBIS?)
---------------------------
(from the PLATO wikipedia page:)

PLATO (Programmed Logic for Automatic Teaching Operations), was the first
generalized computer-asisted instruction system. Starting in 1960, it ran
on the University of Illinois ILLIAC I computer. By the late 1970s, it
supported several thousand graphics terminals distributed worldwide, running
on nearly a dozen different networked mainframe computers. Many modern
concepts in multi-user computing were originally developed on PLATO, including
forums, message boards, online testing, e-mail, chat rooms, picture languages,
instant messaging, remote screen sharing, and multi-player games.

-------------------
What is CYBER1.ORG?
-------------------

CYBER1.ORG is a CYBIS system initially set up in 2004, as a haven for
ex-PLATO users to experience a classic PLATO author experience.

CYBER1.ORG is home to many thousands of classic PLATO lessons and
notesfiles which have been restored from various sources, and have
been made available in the interests of preserving PLATO.

---------------------
What is IRATA.ONLINE?
---------------------

IRATA.ONLINE is a CYBIS system that has been set up for the benefit of
the greater vintage computing community, in the interest to provide
a unique experience that can be accessed on a wide variety of
vintage computers with a bitmapped graphics display. To this end,
IRATA.ONLINE develops PLATOTerm for dozens of platforms, so that they
can access CYBIS systems, as well as provide a community and learning
infrastructure for vintage computing users of all types, in the hopes
that as a cohesive community, something unique can hopefully
emerge.

-----------------------------------------------------------
What is the connection between IRATA.ONLINE and CYBER1.ORG?
-----------------------------------------------------------

CYBER1.ORG and IRATA.ONLINE are independent of one another. With that said,
the reason IRATA.ONLINE and PLATOTerm exist, are because of the efforts of
CYBER1.ORG to not only preserve a running PLATO system, and provide the
necessary information for interested parties to write terminal software
to access CYBIS systems, but also in their effort to produce a publically
available distribution of CYBIS that others who are interested may also
run their own CYBIS installation. IRATA.ONLINE is a direct result of the
public release of this distribution.

Loading PLATOTERM
-----------------
PLATOTERM can be loaded via FujiNet by attaching platoterm.dsk from adam-apps.irata.online in the Comms/ folder, and booting.

PLATO Keyboard
-------------------
The PLATO keyboard is mapped to the Adam keys, like so:

| PLATO KEY  	| Adam Key  	|
|---	|---	|
| ANS  	| CTRL A  	|
| BACK  	| CTRL B  	|
| SHIFT-BACK | HOME + LEFT ARROW |
| COPY | COPY |
| SHIFT-COPY | SHIFT COPY  |
| DATA | CTRL D |
| SHIFT-DATA | SHIFT UNDO |
| EDIT | CTRL E |
| SHIFT-EDIT | SHIFT INSERT |
| ERASE | BACK-SPACE |
| SHIFT-ERASE | SHIFT DELETE |
| FONT | CTRL F |
| &#247; | II | 
| HELP | CLEAR |
| SHIFT-HELP | SHIFT CLEAR |
| LAB | GET |
| SHIFT-LAB | STORE |
| MICRO | HOME + UP-ARROW |
| NEXT | ENTER |
| SHIFT-NEXT | HOME + RIGHT ARROW |
| SUPER | CTRL-P |
| SHIFT-SUPER | SHIFT I |
| SQUARE | SHIFT WILDCARD | 
| ACCESS | SHIFT WILDCARD |
| STOP | CTRL S |
| SHIFT-STOP | HOME | 
| TERM | CTRL T |
| &#215; | I |
| SUB | CTRL Y |
| SHIFT-SUB | SHIFT II |
| SIGMA | III |
| DELTA | IV |
| ASSIGN | ESC |
| UNION | VI |
| INTERSECT | V |

Credits
=======

* '''Thomas Cherryhomes''' - Terminal coding, sleepless nights.
* '''Steve Peltz''' - original PAD protocol decoder from MacPAD.
