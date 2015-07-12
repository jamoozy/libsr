---
layout: index
---

# LibSR: a Sketch Recognition library.

Docs: <http://www.jamoozy.com/libsr/index.html>

Github page: <https://github.com/jamoozy/libsr>

## Overview ####

`libsr` is a sketch recognition library.  The purpose of this library is to
recognize semi-technical sketches; its purpose is *not* recognizing your child's
drawing of a meadow with a house and a smiley face sun.  This section talks
about `libsr` as though it were already complete.

The core of `libsr` consists of 5 modules, all contained in the `src/` directory:

1. `common` contains common utilities and "base classes" for strokes and points.
2. `paleo` contains an implementation of PaleoSketch by Brandon Paulson.
3. `nea` extends PaleoSketch; it allows application programmers to extend
   PaleoSketch with their own domain-specific shapes that build on top of
   PaleoSketch.
4. `ouyang` contains an implementation of Tom Ouyang's symbol recognizer.
5. `dollarp` contains the simple $P implementation.

There are a further 4 modules that add some functionality to `libsr`:

1. `tests` contains automated tests for the entire system.
2. `swig` contains the SWIG interface file and other-language bindings.
   Supported languages include Ruby, Python, Java, Go, ...
3. `tools` contains several tools to help display and collect strokes, test the
   system, and view the recognizers in real time.
4. `docs` contains various bits of documentation, including PDFs of the harrier
   math involved in implementing PaleoSketch and Ouyang's symbol recognizer.

### Schedule & Progress ######

Progress so far (as of 2015-07-12):

1. Rudimentary `common` implementation.
  * `point_t` et al.
  * `stroke_t`

2. Broad strokes of `paleo` in place.
3. Several tests for `common` (not nearly complete)
4. Beginnings of 

Going forward, this is what's left to do:

  1. Finish stroke collector in `tools/collector.py`
  2. Write integration tests with data collected from collector.
  3. 

## Contributing ####

To contribute:

  1. Fork on github
  2. Do this (or similar):

    ```sh
      $ git clone git@github.com:<my-username>/libsr.git
      $ cd libsr
      $ git checkout -b my-feature

      # edit and commit a bunch of clever code ...

      $ git push -u github my-feature
    ```

  3. Open a pull request on github.
  4. Profit!


