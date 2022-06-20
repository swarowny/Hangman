cmake_minimum_required(VERSION 3.22)
project(Assignment2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Assignment2
        Gamer.cpp
        Gamer.h
        Hangman.cpp
        Hangman.h
        Main.cpp
        Manager.cpp
        Manager.h
        Person.cpp
        Person.h
        ReadWords.cpp
        ReadWords.h
        WordProcessor.cpp
        WordProcessor.h)
