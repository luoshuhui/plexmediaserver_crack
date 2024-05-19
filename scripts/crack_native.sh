#!/bin/sh

# The script assumes Plex Media Server is currently running.

PLEX_MEDIA_SERVER_DIR=$(ps aux | grep 'Plex Media Server' | grep -v grep | awk '{print $11}' | xargs dirname)

if [ `id -u` -ne 0 ]; then
    echo "Run this script as root or with sudo. Script aborting."
    exit 1
fi

if [ -z "$PLEX_MEDIA_SERVER_DIR" ]; then
    echo "Plex Media Server is not running, unable to determine its directory. Script aborting."
    exit 1
fi

rm -rf /opt/plexmediaserver_crack
mkdir /opt/plexmediaserver_crack
cd /opt/plexmediaserver_crack
wget https://github.com/yuv420p10le/plexmediaserver_crack/releases/latest/download/plexmediaserver_crack.so
rm $PLEX_MEDIA_SERVER_DIR/lib/plexmediaserver_crack.so
ln -s /opt/plexmediaserver_crack/plexmediaserver_crack.so $PLEX_MEDIA_SERVER_DIR/lib/plexmediaserver_crack.so
patchelf --remove-needed plexmediaserver_crack.so $PLEX_MEDIA_SERVER_DIR/lib/libsoci_core.so
patchelf --add-needed plexmediaserver_crack.so $PLEX_MEDIA_SERVER_DIR/lib/libsoci_core.so
systemctl restart plexmediaserver
