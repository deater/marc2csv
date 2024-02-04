#!/bin/sh

for i in ./records/* ; do ./dump_record < $i; done
