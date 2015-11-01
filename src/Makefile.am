SUBDIRS = common dollarp paleo

lib_LTLIBRARIES = libsr.la
libsr_la_SOURCES = main.c
libsr_la_LIBADD = paleo/libpaleo.la common/libcommon.la
libsr_la_LDFLAGS = --whole-archive
