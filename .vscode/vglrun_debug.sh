#!/bin/bash
if [ ! -f /opt/VirtualGL/bin/vglrun ]; then
    gdb "$@"
else
    vglrun gdb "$@"
fi

