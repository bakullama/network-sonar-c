# Network sonar thingy

Not anything good or impressive, but a totally useless program using a really old library, designed to look like those SONAR screens you see on submarines in the movies.
I wanted to get a bit more experience in C so I wrote this little network visualisation tool, it uses the libgraphC library ([https://github.com/Prashant47/libgraph](https://github.com/Prashant47/libgraph)) and a little bit of bash to do the network scanning.

## Installation + usage
Once you've cloned the repository, cd into it and run these commands
`qmake -o network-sonar-c`
`make`
`./network-sonar-c`

## Todo:
 - [x] Add the ability to specify an IP range
 - [x] Add command line support
 - [ ] Fix flickering (possibly with double buffer?)
 - [ ] Fix occasional generation of point outside of circle
 - [ ]  Add more information about the network, ie hostname or OS
 - [ ] Stop "libgraphC: xcb_io.c:260: poll_for_event: Assertion `!xcb_xlib_threads_sequence_lost' failed." error message
