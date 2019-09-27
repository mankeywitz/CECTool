#!/bin/bash -ex

dkp-pacman -Syyu --noconfirm 3ds-dev
source /etc/profile.d/devkit-env.sh
cd /CECTool
make