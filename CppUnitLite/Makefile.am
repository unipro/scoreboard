# Process this file with automake to produce Makefile.in.
# This is a simple C++ TestingFramework developed by Michael Feathers,
# who wrote the original CppUnit

noinst_LIBRARIES = libCppUnitLite.a
libCppUnitLite_a_SOURCES = Failure.cpp SimpleString.cpp Test.cpp \
	TestRegistry.cpp TestResult.cpp
noinst_HEADERS = Failure.h SimpleString.h Test.h TestHarness.h \
	TestRegistry.h TestResult.h
libCppUnitLite_a_CPPFLAGS = @CPPFLAGS@
libCppUnitLite_a_CXXFLAGS = @CXXFLAGS@
