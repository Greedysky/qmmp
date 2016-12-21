#!/bin/sh

######################################
# Removes svn tags from qmmp package #
######################################

find ../ -type d -name ".svn" | xargs rm -rf
