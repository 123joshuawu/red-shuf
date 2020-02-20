# red-shuf

```
SHUF(1)                     General Commands Manual                    SHUF(1)

NAME
     shuf – randomly permute input lines

SYNOPSIS
     shuf [-hv] [-n count] [-o outfile] [-rz] [file]
     shuf [-hv] -e [-n count] [-o outfile] [-rz] [args ...]
     shuf [-hv] -i lo-hi [-n count] [-o outfile] [-rz]

DESCRIPTION
     shuf is a utility that outputs a random permutation of its input lines.
     By default, shuf reads from stdin and outputs to stdout.

     The options are as follows:

     -e      Use a space-separated list of command-line args as input.

             Cannot be combined with -i.

     -h      Print usage information and exit.

     -i lo-hi
             Use the range of positive integers from lo to hi as input.

             Cannot be combined with -e.

     -n count
             Print at most count lines.

     -o outfile
             Write output to outfile instead of stdout.

     -r      Do not permute.  Instead, choose lines at random, with
             replacement.  When used without -n, shuf repeats indefinitely.

     -v      Print version information and exit.

     -z      Delineate lines with the NUL character instead of the newline
             character.

EXIT STATUS
     The shuf utility exits 0 on success, and >0 if an error occurs.

EXAMPLES
     Flip a coin ten times:

           shuf -e -n 10 -r Heads Tails

     Select five numbers from 10 to 20:

           shuf -i 10-20 -n 5

     Set a random desktop background image from the user's collection:

           ls ~/wallpaper | shuf -n 1 | feh --bg-fill

     Read a shuffled version of this manual page:

           shuf shuf.1 | mandoc -mdoc | less

AUTHORS
     shuf was written by Brian Callahan <bcallah@openbsd.org>.

CAVEATS
     This implementation of shuf intentionally omits all long options.

     The maximum range for lo-hi is INT_MAX-1.

Mac OS X 10.13                   July 13, 2017                  Mac OS X 10.13
```
