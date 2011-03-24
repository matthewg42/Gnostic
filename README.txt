
GNOSTIC
M N Gates, 2011


What it is
==========

It's a remote process montoring visualization tool.

Gnostic connects to remote servers, executes a command there, and plots
pretty graphs from the output of those remote programs.


Build Dependencies
------------------

- A recent Qt.  4.x should work, I used 4.7
- Qwt.  I use Qwt-6.0.0-rc5 on win32, and SVN export on Ubuntu.
- gcc goodness.  On win32 this is bundled with Qt.


Runtime Dependencies
====================

for OpenSshTransport: ssh + ssh-askpass
for PlinkTransport:   plink (shipped with win32 installer)


Concepts
========

Transports
----------
A Transport is a pipe to a remote server which can be used to get output
from some remote command which is running there.  Think a rexec or ssh
session.

Remote Commands
---------------
A remote command is just a command on a system which spits out data in a
format Gnostic can understand.  That essentially means adhereing to the
Gnostic Protocol, which is a simple text-based format.  See below.

Remote Commands can be written in any language, so long as they spit out
intelligible data.  A Remote Command is started when Gnostic connects to
a machine, and keeps running until Gnostic disconnects (i.e. stops
plotting data for a given remote command.

Each Remote Command is associated with a Transport.  A Transport can have
any number of Remote Commands associated with it.

Data Displays
-------------
A Data Display is some method to visualize the output of a Remote Command.
Currently, three types of Data Display are implemented:

* Counter:   simply counts the number of lines of input which have arrived
             from a Remote Command.
* Tailer:    like the UNIX tail command
* TimeGraph: plots a graph of each incoming data item from a Remote Command.


Remote Monitors
---------------
A Remote Monitor connects one or more Remote Commands to one or more Data
Displays.  A simple example would be for a Remote Command to print new
lines appearing in a log file, and connect this to a Tailer Data Display.

A more complicated example might be for a Remote Command to tail a log and
print some statistics (e.g. lines per second) according to the Gnostic
Protocol.  This command might be executed on several different machines
(via different Transports), but have all of them connected to a single
Time Graph display.


The Gnostic Data Protocol
=========================

An example is probably better than an explanation:

GNOSTIC-DATA-PROTOCOL-VERSION=1.0
HOST=the.fq.hostname
INVOCATION-PATH=/whatever/the/program/was
INVOCATION-ARGS="list" "of" "quoted arguments"
MONITOR-NAME=Some monitor
UPDATE-DELAY=1.0
DELIMITER=:
END-HEADER
timstamp:value:label
timstamp:value:label
timstamp:value:label
GNOSTIC-WARNING=System going down in 3 mins
timstamp:value:label
timstamp:value:label
timstamp:value:label
GNOSTIC-ERROR=Monitor sent TERM signal, BYE

Notes
-----
* Timestamps are in number of milli-seconds since epoch.
* GNOSTIC-ERRORs and GNOSTIC-WARNINGs are not currently supported
* Most header values don't do very much
* It is nice, but not totally necessary that lines arrive in
  chronological order.
* new labels will be dynamically added to a graph as they appear


TODO for 1st release
--------------------

- better connection status notification
  - robust notification of connection failures
  - not locking up while connecting
  - propagate connection failure to RemoteMonitor
- some sort of transport status indicator
- reconnect on transport failure
- chase down TODO tags in code


Future Goals
============

- Make a "Configurable Item" class & derive stuff from it
- Have a "Configurable Item Config Widget" parner class
- Automatic discovery of "used child items"
- export / import settings
- copy a bunch of RemoteCommands from one Transport to another...
- filtering on command in monitor config


Musings
=======

Windows remote exec
-------------------
Well here's a thing, remotely executing a command on a windows server and
getting a bunch of text back is a pain in the ass.  Looks like the only
native method is rexec, presumably from their POSIX layer, which makes me
shudder.

So the options are:  install ssh servers on remote hosts, or maybe a light
weight custom process launcher, and make a transport class for that...

The problem is authentication.  Don't want to send passwords in the clear
but would like to use windows authentication if possible... which means
having a public key system to exchange a session key for a stream cipher,
which is essentially re-implementing a part of the SSH protocol.  Ho hum.

Need to have a think on that.


Backronyms - no Free Software project should be without them
============================================================

Gosh, Network Operation Seems Terrifically Interesting. Champion!
Gates' Nearly Omnipotent Software That Is Cromulent.
Get New Operating System. This Is Crap" (choose your target platform)
Gnostic's not overly stupid, technically it's complicated
Graphic Notifying Omni-Platform Software Tool Inspecting Clusters
Gnostic Notifies On Servers Taking Inconvenient Craps
Great, No One Saw That It Crashed
