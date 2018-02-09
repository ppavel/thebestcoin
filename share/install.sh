#!/bin/bash

set -e

function make_config()
{
	mkdir -p $CONF_DIR
	cp thebestcoin/thebestcoin.conf "$CONF_DEST"
	echo -e "rpcuser=thebestcoinrpc\nrpcpassword=$(xxd -l 16 -p /dev/urandom)" >> "$CONF_DEST"
}

function create_symlinks()
{
	echo "You can be prompted for sudo password"
	sudo ln -sfv $BIN_DIR/thebestcoin* /usr/local/bin
	echo "Symlink created"
}

function set_autostart()
{
	sudo cp $DAEMON_CONF $LAUNCH_DIR
	if [[ $OS == Linux ]]; then
		sudo systemctl daemon-reload
		sudo systemctl enable $DAEMON_CONF
	fi
	echo "Startup job created"
}

cd "$(dirname ${BASH_SOURCE[0]})"

# TODO:
# ajust path
BIN_DIR="$(dirname "$(pwd)")"/bin

OS=`uname -s`
if [[ $OS == Linux ]]; then
	CONF_DIR=~/.thebestcoin
	DAEMON_CONF=thebestcoind.service
	LAUNCH_DIR=/etc/systemd/system
elif [[ ($OS == Darwin)]]; then
	CONF_DIR=/Users/${USER}/Library/Application\ Support/TheBestCoin
	DAEMON_CONF=org.thebestcoind.plist
	LAUNCH_DIR=/Library/LaunchAgents
else
    echo "Unsupported system: $OS\n"
    exit 1
fi

if [ ! -f $BIN_DIR/thebestcoind ]; then
	echo "Binaries doesn't exist"
    exit 1
fi

CONF_DEST=${CONF_DIR}/thebestcoin.conf

# write conf only if not exists
[ -f "$CONF_DEST" ] || make_config

create_symlinks

read -r -p "Would you like to start headless TheBestCoin daemon at system boot? [y/N] " response
if [[ "$response" =~ ^([yY][eE][sS]|[yY])+$ ]]
then
    set_autostart
fi

cd - > /dev/null
