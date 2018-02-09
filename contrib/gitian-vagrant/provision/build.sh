#!/bin/bash

echo 'if [ -f /etc/bash_completion ] && ! shopt -oq posix; then\
    . /etc/bash_completion\
fi'\
>> /home/vagrant/.bashrc

mkdir /home/vagrant/.ssh
chmod 700 /home/vagrant/.ssh
ssh-keygen -f /home/vagrant/.ssh/id_rsa -t rsa -N ''

cd /home/vagrant
export USE_LXC=1

git clone git://github.com/devrandom/gitian-builder.git
mkdir gitian-builder/inputs

wget 'http://miniupnp.tuxfamily.org/files/download.php?file=miniupnpc-1.5.tar.gz' -O gitian-builder/inputs/miniupnpc-1.5.tar.gz
wget 'http://downloads.sourceforge.net/project/wxwindows/2.9.1/wxWidgets-2.9.1.tar.bz2' -O gitian-builder/inputs/wxWidgets-2.9.1.tar.bz2

cp /vagrant/osslsigncode-Backports-to-1.7.1.patch /home/vagrant/gitian-builder/inputs
cp /vagrant/osslsigncode-1.7.1.tar.gz /home/vagrant/gitian-builder/inputs

wget -O /home/vagrant/gitian-builder/inputs/MacOSX10.11.sdk.tar.xz https://github.com/phracker/MacOSX-SDKs/releases/download/MacOSX10.11.sdk/MacOSX10.11.sdk.tar.xz

#cp /vagrant/MacOSX10.11.sdk.tar.xz /home/vagrant/gitian-builder/inputs

cd /home/vagrant/gitian-builder

# Create base images

export USE_LXC=1
