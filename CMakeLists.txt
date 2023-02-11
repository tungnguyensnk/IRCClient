cmake_minimum_required(VERSION 3.22)
project(IRCClient C)

set(CMAKE_C_STANDARD 99)

add_executable(IRCClient main.c irc.c irc.h recv_thread.c recv_thread.h)
