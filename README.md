# Descramble

## Introduction

Solver for "scramble", a facebook game or something from ~2009, I don't quite
remember. The game presents a 4x4 grid of characters and the player must find
words contained within that are made up of any connecting characters.

I created this to teach myself C and to beat my wife at the game. This is the
first full program I've written in C, but should be good code, at least it does
what it set out to do.

## Wordlists

The program requires wordlists. They should be named words.NUM where NUM is a
length from 3 though 8.

You can generate the necessary wordlists from your local dictionary using
something like:

```
% for i in $(seq 3 8); do awk "length(\$0) == $i {print}" /usr/share/dict/words > words.$i; done
```

## Usage

Assuming the game provides the grid:

```
a  b  c  d
e  f  g  h
i  j  k  l
m  n  o  p
```

Type out the grid as four blocks of four letters:

```
% ./descramble abcd efgh ijkl mnop
      fab      fie      fin     fink     fino     glop      ink      jin
     jink    knife     knop     koji      kop      lop     mink      nim
      pol    plonk
Found 18 words
```

## Installation

To compile the program:

```
% cd /path/to/descramble.c
% make
```

Copy the file to somewhere like /usr/local/bin or just leave it where it is.
