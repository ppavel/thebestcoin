#!/bin/bash

apt-get update && apt-get install -y apt-utils

echo -e 'if [ -f /etc/bash_completion ] && ! shopt -oq posix; then
    . /etc/bash_completion
fi\n'\
>> /home/vagrant/.bashrc

apt-get update && \
  apt-get install -y apt-utils

apt-get install -y autoconf2.13 automake build-essential bsdmainutils faketime g++ g++-mingw-w64 git-core libqt4-dev libtool libz-dev mingw-w64 nsis pciutils pkg-config psmisc subversion unzip zip

apt-get install -y git ruby sudo apt-cacher-ng qemu-utils debootstrap lxc python-cheetah parted kpartx bridge-utils make ubuntu-archive-keyring curl xz-utils libgmp-dev

# adduser debian sudo
echo "%sudo ALL=NOPASSWD: /usr/bin/lxc-start" > /etc/sudoers.d/gitian-lxc
echo '#!/bin/sh -e' > /etc/rc.local
echo 'brctl addbr br0' >> /etc/rc.local
echo 'ifconfig br0 10.0.3.2/24 up' >> /etc/rc.local
echo 'iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE' >> /etc/rc.local
echo 'echo 1 > /proc/sys/net/ipv4/ip_forward' >> /etc/rc.local
echo 'exit 0' >> /etc/rc.local
echo 'export USE_LXC=1' >> /home/vagrant/.profile
echo 'export GITIAN_HOST_IP=10.0.3.2' >> /home/vagrant/.profile
echo 'export LXC_GUEST_IP=10.0.3.5' >> /home/vagrant/.profile
ls -lah /home/vagrant/
# reboot

export USE_LXC=1
export GITIAN_HOST_IP=10.0.3.2
export LXC_GUEST_IP=10.0.3.5

apt-get install -y build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev && \
apt-get install -y software-properties-common python-software-properties  && \
apt-get install -y curl mc vim git net-tools ruby-dev tmux rpm dpkg-sig

gem install fpm


cd /home/vagrant
wget http://archive.ubuntu.com/ubuntu/pool/universe/v/vm-builder/vm-builder_0.12.4+bzr494.orig.tar.gz
echo "76cbf8c52c391160b2641e7120dbade5afded713afaa6032f733a261f13e6a8e  vm-builder_0.12.4+bzr494.orig.tar.gz" | sha256sum -c
# (verification -- must return OK)
tar -zxvf vm-builder_0.12.4+bzr494.orig.tar.gz
cd vm-builder-0.12.4+bzr494
python /home/vagrant/vm-builder-0.12.4+bzr494/setup.py install
cd /home/vagrant

# service apt-cacher start

sudo apt-get -y install parted expect


# reboot

