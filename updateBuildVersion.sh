#!/usr/bin/env bash

let oldversion=`curl http://s3.amazonaws.com/feelflight/firmware/blanket.version`
let newversion=oldversion+1

echo "============="
echo "New Version:"
echo $newversion
echo "============="

sed -i "s/REPLACE_WITH_CURRENT_VERSION/$newversion/g" src/firmware.h

echo $newversion > upload/blanket.version
