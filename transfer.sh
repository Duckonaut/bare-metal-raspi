#!/bin/sh

MNT_POINT=/run/media/szagorow/boot

if [ -d "$MNT_POINT" ]
then
    echo "$MNT_POINT exists, mounting..."
else
    mkdir $MNT_POINT 
    echo "$MNT_POINT created, mounting..."
fi

mount /dev/sdb1 $MNT_POINT

if [ "$?" = 0 ]
then
    echo "boot mounted to $MNT_POINT"
    cp kernel.img "$MNT_POINT/kernel.img" &
    prid=$!
    wait $prid
    echo "copied to $MNT_POINT/kernel.img"

else 
    echo "mount error, trying to umount..."
    
fi
umount $MNT_POINT
if [ "$?" = 0 ]
then
    echo "successfully umounted"
else
    echo "umount error"
fi
echo "finished"