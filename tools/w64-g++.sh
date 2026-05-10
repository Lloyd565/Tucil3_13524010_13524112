#!/usr/bin/env bash

set -e

cmd.exe /C "set PATH=D:\\Language\\raylib\\w64devkit\\bin;%PATH% && g++ $*"
