# Process this file with automake to produce Makefile.in.
# Makefile.am for scoreboard/test.
#
# Copyright 2014 scoreboard (scoreboard@naver.com)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

check_PROGRAMS = test_scoreboard
test_scoreboard_SOURCES = main.cpp \
	GameConfigTest.cpp GameTimeTest.cpp TeamScoreTest.cpp
test_scoreboard_CPPFLAGS = @CPPFLAGS@
test_scoreboard_CXXFLAGS = @CXXFLAGS@
test_scoreboard_LDFLAGS = @LDFLAGS@
test_scoreboard_LDADD = @LIBS@ \
	../CppUnitLite/libCppUnitLite.a ../model/libscoreboardmodel.a
