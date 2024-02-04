#!/bin/sh

./print_header
for i in ./records/* ; do ./marc2csv < $i; done
