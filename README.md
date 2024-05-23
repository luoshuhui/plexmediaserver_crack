# plexmediaserver_crack

Enables Godmode, unlocking *all features* on (hardware transcoding, intro/credit detection, HEVC transcoding, etc..) on Plex Media Server. Including Plex Pass features without a required subscription, and including early-access/development/Plex Ninja features.

# Installation

## Windows

1. Download `IPHLPAPI.dll` from [the latest release](https://github.com/yuv420p10le/plexmediaserver_crack/releases/latest/download/IPHLPAPI.dll) and put it in your Plex Media Server's installation folder. (e.g. `C:\Program Files\Plex\Plex Media Server`)
2. Restart Plex Media Server. All features will be unlocked.

## Linux

x86-64 only.

First, install `patchelf` from your distribution's package manager or `pip`:

```bash
sudo apt update && sudo apt install patchelf # Debian/Ubuntu/etc
sudo pacman -S patchelf # Arch
sudo yum install patchelf # Fedora/RHEL/Centos/Rocky/OpenSUSE/etc
sudo apk update && apk add --upgrade patchelf # Alpine
sudo pip install patchelf # pip; you might need --break-system-packages if installing if you're on an externally managed environment
```

And then:

### Native

The script assumes Plex Media Server is currently running.

Run the following command:

```bash
sudo sh -c "$(curl -sSL https://github.com/yuv420p10le/plexmediaserver_crack/releases/latest/download/crack_native.sh)"
```

Your Plex Media Server should now be restarted with all features unlocked.

For persistance with Plex updates, create the above as a bash script (run as root!) and run it manually (or, trigger it) after updates or run it as a daily cronjob.

### Docker

Tested on [linuxserver/docker-plex](https://github.com/linuxserver/docker-plex), but should work for all Docker setups.

The script assumes Plex Media Server is currently running, that you have a mounted `/config` volume in the container, and that your container is named `plex`.  
If your container is named differently or if your external volume is mounted elsewhere, change it at the top of the script instead of running it through this command.

Run the following command: (you can emit `sudo` if the executing user is in the `docker` group)

```bash
sudo sh -c "$(curl -sSL https://github.com/yuv420p10le/plexmediaserver_crack/releases/latest/download/crack_docker.sh)"
```

Your Plex Media Server should now be restarted with all features unlocked.

This will NOT persist through Docker image updates, as rebuilding the container will copy libraries freshly from the image.  
Setup the above commands as a script for an easy installation, and optionally, set it as a cronjob to run daily.


###  Other configurations

- For an UNRAID (x86-64) setup guide, [check Mizz141's gist](https://gist.github.com/mizz141/608d21fbc2fe4480286c76cc421f40d3).
- For binary patching for Windows/Linux (x86-64/ARM64) instead of DLL sideloading/import hijacking, [check edde746's fork](https://github.com/edde746/plexmediaserver_crack).

### Building

#### Windows

Build using Visual Studio 2022, C++20.

#### Linux

[linuxserver/docker-plex](https://github.com/linuxserver/docker-plex)'s image uses GLIBC 2.35, and Debian stable (bookworm, as of now) is at 2.36. Use a host with an older version to build (e.g. Debian Bullseye).

### Troubleshooting

* For intro/credit detection, go to Settings -> Library -> Marker source; and select the "local detection only" option.
* If hardware transcoding (or any other feature) does not work, it should not be related to this repository. Use Google to troubleshoot why said feature doesn't work on your setup specifically.
* The "Skip Intro" button will not be displayed on clients that don't have the Plex Pass. It's a client sided limitation. I wrote a crack for the Plex client on Windows to circumvent it, at [plexpass_hook](https://github.com/yuv420p10le/plexpass_hook). The "Skip Credits" button will appear on all clients, including the free ones.

### Screenshots

![](https://i.imgur.com/6LGkLcm.png) ![](https://i.imgur.com/su02dh3.png)  
![](https://i.imgur.com/sSysxcq.png)