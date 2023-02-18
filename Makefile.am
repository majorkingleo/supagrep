bin_PROGRAMS=\
	supagrep
	
if MINGW
bin_PROGRAMS += forward

forward_SOURCES=\
	forward/main.cpp
	
forward_LDADD = cpputils/cpputilsshared/libcpputilsshared.a \
	cpputils/cpputilsshared/cpputilsformat/libcpputilsformat.a


	
bin_PROGRAMS += addpath

addpath_SOURCES=\
	addpath/main.cpp	
	
addpath_LDADD = cpputils/cpputilsshared/libcpputilsshared.a \
	cpputils/cpputilsshared/cpputilsformat/libcpputilsformat.a	
	
endif

supagrep_SOURCES=\
		src/adie.h \
		src/desccombo.h \
		src/desccombo.cpp \
		src/icon_add.h \
		src/icon_close.h \
		src/lang.h \
		src/lang.cpp \
		src/main.h \
		src/main.cpp \
		src/resultwin.h \
		src/resultwin.cpp \
		src/search.h \
		src/search.cpp \
		src/searchwin.h \
		src/searchwin.cpp \
		src/setup.h \
		src/setup.cpp \
		src/tab.h \
		src/tab.cpp \
		tools_config.h

AM_CPPFLAGS = -I$(top_srcdir)/tools \
	-I$(top_srcdir)/cpputils/cpputilsshared  \
	-I$(top_srcdir)/cpputils/cpputilsshared/cpputilsformat \
	-I$(top_srcdir)/cpputils/io \
	-I$(top_srcdir)/cpputils/fox \
	-DNOWAMAS \
	$(LIBFOX_CFLAGS) \
	-std=gnu++20 \
	-D_XOPEN_SOURCE=700 # for cyggwin fileno()

noinst_LIBRARIES=cpputils/io/libcpputilsio.a \
	cpputils/cpputilsshared/libcpputilsshared.a \
	cpputils/cpputilsshared/cpputilsformat/libcpputilsformat.a \
	cpputils/fox/libcpputilsfox.a

cpputils_io_libcpputilsio_a_SOURCES= \
	cpputils/io/ColoredOutput.h \
	cpputils/io/ColoredOutput.cc \
	cpputils/io/debug.cc \
	cpputils/io/debug.h \
	cpputils/io/DetectLocale.h \
	cpputils/io/DetectLocale.cc \
	cpputils/io/OutDebug.h \
	cpputils/io/OutDebug.cc \
	cpputils/io/read_file.h \
	cpputils/io/read_file.cc


cpputils_cpputilsshared_libcpputilsshared_a_SOURCES= \
		cpputils/cpputilsshared/arg.h \
		cpputils/cpputilsshared/arg.cc \
		cpputils/cpputilsshared/backtrace.h \
		cpputils/cpputilsshared/backtrace.cc \
		cpputils/cpputilsshared/cppdir.h \
		cpputils/cpputilsshared/cppdir.cc \
		cpputils/cpputilsshared/file_option.h \
		cpputils/cpputilsshared/file_option.cc \
		cpputils/cpputilsshared/iterator.h \
		cpputils/cpputilsshared/leoini.h \
		cpputils/cpputilsshared/leoini.cc \
		cpputils/cpputilsshared/map_utils.h \
		cpputils/cpputilsshared/pairs.h \
		cpputils/cpputilsshared/pairs.cc \
		cpputils/cpputilsshared/ref.h \
		cpputils/cpputilsshared/stderr_exception.h \
		cpputils/cpputilsshared/string_utils.h \
		cpputils/cpputilsshared/string_utils.cc \
		cpputils/cpputilsshared/utf8.h \
		cpputils/cpputilsshared/utf8/core.h \
		cpputils/cpputilsshared/utf8/checked.h \
		cpputils/cpputilsshared/utf8/unchecked.h \
		cpputils/cpputilsshared/utf8/cpp11.h \
		cpputils/cpputilsshared/utf8/cpp17.h \
		cpputils/cpputilsshared/utf8_util.h \
		cpputils/cpputilsshared/utf8_util.cc \
		cpputils/cpputilsshared/xml.h \
		cpputils/cpputilsshared/xml.cc


cpputils_cpputilsshared_cpputilsformat_libcpputilsformat_a_SOURCES= \
	cpputils/cpputilsshared/cpputilsformat/format1.cc \
	cpputils/cpputilsshared/cpputilsformat/format2.cc \
	cpputils/cpputilsshared/cpputilsformat/format2.h \
	cpputils/cpputilsshared/cpputilsformat/format1.h \
	cpputils/cpputilsshared/cpputilsformat/cformat.h \
	cpputils/cpputilsshared/cpputilsformat/wformat2.cc \
	cpputils/cpputilsshared/cpputilsformat/wformat2.h \
	cpputils/cpputilsshared/cpputilsformat/cwformat.h \
	cpputils/cpputilsshared/cpputilsformat/cwformat.cc
	
	
cpputils_fox_libcpputilsfox_a_SOURCES= \
	cpputils/fox/theme.h \
	cpputils/fox/theme.cpp	

supagrep_LDADD = cpputils/io/libcpputilsio.a \
				 cpputils/cpputilsshared/libcpputilsshared.a \
				 cpputils/cpputilsshared/cpputilsformat/libcpputilsformat.a \
				 cpputils/fox/libcpputilsfox.a
				 
LIBS= \
    $(LIBFOX_LIBS)
    
AM_LDFLAGS=
    
if MINGW
AM_LDFLAGS += -mwindows
LIBS += -liconv
# with -std=c++11 WIN32 is disabled (_WIN32) would be the valid one, so we define it in mingw.
AM_CPPFLAGS += -DWIN32
else
AM_LDFLAGS += -lX11
endif


    

