# Descramble

## Introduction

Solver for "scramble", a facebook game or something from ~2009, I don't quite
remember. The game presents a 4x4 grid of characters and the player must find
words contained within (words can be made up of any connecting characters).

I created this to teach myself some aspects of C and to beat my wife at the
game. This is the first full program I've written in C, but should be good
code, at least it does what it set out to.

## Wordlists

The program requires wordlists to run. There should be one named words.NUM
where NUM is a length from 3 though 8.

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
