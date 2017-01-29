# Testing with GNU Check.

`libsr` uses [GNU Check](http://check.sourceforge.net/doc/check_html/index.html) to run unit tests.
See the documentation for specifics.
One nice trick I've found to debugging tests is to run:
```sh
$ CK_FORK=no gdb <check_binary>
```
This disables check's forking, thereby simplifying debugging, breakpoint setting, and stack traces.
