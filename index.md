---
layout: index
---

# LibSR: a Sketch Recognition library.

Docs: http://www.jamoozy.com/libsr-docs/index.html

## Overview ####

`libsr` is a sketch recognition library.  The purpose of this library is to
recognize semi-technical sketches; its purpose is *not* recognizing your child's
drawing of a meadow with a house and a smiley face sun.  This section talks
about `libsr` as though it were already complete.

The core of `libsr` consists of 5 modules:

1. `common` contains common utilities and "base classes" for strokes and points.
2. `paleo` contains an implementation of PaleoSketch by Brandon Paulson.
3. `nea` extends PaleoSketch; it allows application programmers to extend
   PaleoSketch with their own domain-specific shapes that build on top of
   PaleoSketch.
4. `ouyang` contains an implementation of Tom Ouyang's symbol recognizer.
5. `dollarp` contains the simple $P implementation.

There are a further 3 modules that add some functionality to `libsr`:

6. `swig` contains the SWIG interface file and other-language bindings.
   Supported languages include Ruby, Python, Java, Go, ...

### Schedule & Progress ######

Going forward, 

## Contributing ####

To contribute:

1. Fork on github
2. Do this (or similar):
```sh
  $ git clone https://github.com/jamoozy/libsr.git
  $ cd libsr
  $ git remote add github git@github.com:<my-username>/libsr.git
  $ git checkout -b my-feature

  # edit and commit a bunch of clever code ...

  $ git push -u github my-feature
```
3. Open a pull request on github!
