#!/usr/bin/env bash
scalac test/Test.scala -d test/bin
rm bin/loader
gcc runtime/src/loader.c -o bin/loader
bin/loader