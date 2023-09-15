#!/bin/bash

pkgbuild --root "$1" --identifier io.github.KWYTHUB --scripts "installer/mac/Scripts" --install-location "/tmp/sapphire-install" $2
