#!/bin/sh

MNT_POINT=/mnt/sd_card

if [ -d "$MNT_POINT" ]
then
    echo "$MNT_POINT exists, mounting..."
else
    sudo mkdir $MNT_POINT 
    echo "$MNT_POINT created, mounting..."
fi

sudo mount /dev/sdb1 $MNT_POINT

if [ "$?" = 0 ]
then
    echo "boot mounted to $MNT_POINT"
    sudo cp kernel.img "$MNT_POINT/kernel.img" &
    prid=$!
    wait $prid
    echo "copied to $MNT_POINT/kernel.img"

else 
    echo "mount error, trying to umount..."
    
fi
sudo umount $MNT_POINT
if [ "$?" = 0 ]
then
    echo "successfully umounted"
else
    echo "umount error"
fi

sudo rmdir $MNT_POINT
if [ "$?" = 0 ]
then
	echo "successfully removed $MNT_POINT"
else
	echo "failed to remove $MNT_POINT"
fi
echo "finished"
