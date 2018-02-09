#! /bin/bash

# ~/.profile
# export URL=https://github.com/thebestcoin/thebestcoin.git
# export COMMIT=HEAD
# export VERSION=1.11.12
# export GIT_SRC=$HOME/thebestcoin
# export BUILD_DIR=$HOME/gitian-builder/build/out
# export RELEASE_DIR=$HOME/gitian-builder/release
# export KEY_ID=TheBest

linux=true
windows=true
osx=true

# Help Message
read -d '' usage <<- EOF
Usage: $scriptName [-o l|w|x]

Run this script from the gitian-builder directory.

Options:
-o|--os		Specify which Operating Systems the build is for. Default is lwx. l for linux, w for windows, x for osx
-h|--help	Print this help message
EOF

# Get options and arguments
while :; do
    case $1 in
        # Operating Systems
        -o|--os)
	    if [ -n "$2" ]
	    then
		linux=false
		windows=false
		osx=false
		if [[ "$2" = *"l"* ]]
		then
		    linux=true
		fi
		if [[ "$2" = *"w"* ]]
		then
		    windows=true
		fi
		if [[ "$2" = *"x"* ]]
		then
		    osx=true
		fi
		shift
	    else
		echo 'Error: "--os" requires an argument containing an l (for linux), w (for windows), or x (for Mac OSX)\n'
		exit 1
	    fi
	    ;;
	# Help message
	-h|--help)
	    echo "$usage"
	    exit 0
	    ;;
	*) # Default: no more options, break out of the loop.
             break
    esac
    shift
done

if [[ $linux = true ]]
then
	bin/gbuild -j7 -m6000 \
	 --commit thebestcoin=${COMMIT} \
	 --url thebestcoin=${URL} \
	 --skip-image \
	 ${GIT_SRC}/contrib/gitian-descriptors/gitian-linux.yml

	cp ${BUILD_DIR}/thebestcoin-${VERSION}-x86_64-linux-gnu.tar.gz ${RELEASE_DIR}/lin
	cp ${BUILD_DIR}/thebestcoin-${VERSION}-i686-pc-linux-gnu.tar.gz ${RELEASE_DIR}/lin

	fpm -s tar \
	 -t rpm --prefix usr/local \
	 -C thebestcoin-${VERSION}  \
	 -a amd64 \
	 --rpm-os linux \
	 --after-install ${GIT_SRC}/share/postinst-rpm \
	 --before-remove ${GIT_SRC}/share/prerm \
	 -n thebestcoin \
	 -v ${VERSION} \
	 -p ${BUILD_DIR}/thebestcoin-${VERSION}.x86_64.rpm  \
	 ${BUILD_DIR}/thebestcoin-${VERSION}-x86_64-linux-gnu.tar.gz

	rpmsign --addsign --key-id=${KEY_ID} ${BUILD_DIR}/thebestcoin-${VERSION}.x86_64.rpm
	cp ${BUILD_DIR}/thebestcoin-${VERSION}.x86_64.rpm ${RELEASE_DIR}/lin

	fpm -s tar \
	 -t rpm \
	 --prefix usr/local \
	 -C thebestcoin-${VERSION}  \
	 -a i386 \
	 --rpm-os linux \
	 --after-install ${GIT_SRC}/share/postinst-rpm \
	 --before-remove ${GIT_SRC}/share/prerm \
	 -n thebestcoin \
	 -v ${VERSION} \
	 -p ${BUILD_DIR}/thebestcoin-${VERSION}.i686-pc.rpm  \
	 ${BUILD_DIR}/thebestcoin-${VERSION}-i686-pc-linux-gnu.tar.gz

	rpmsign --addsign --key-id=${KEY_ID} ${BUILD_DIR}/thebestcoin-${VERSION}.i686-pc.rpm
	cp ${BUILD_DIR}/thebestcoin-${VERSION}.i686-pc.rpm ${RELEASE_DIR}/lin

	fpm --deb-no-default-config-files \
	 -s tar \
	 -t deb \
	 --prefix usr/local \
	 -C thebestcoin-${VERSION}  \
	 -a amd64 \
	 --after-install ${GIT_SRC}/share/postinst-deb \
	 --before-remove ${GIT_SRC}/share/prerm \
	 -n thebestcoin \
	 -v ${VERSION} \
	 -p ${BUILD_DIR}/thebestcoin-${VERSION}.x86_64.deb  \
	 ${BUILD_DIR}/thebestcoin-${VERSION}-x86_64-linux-gnu.tar.gz

	dpkg-sig -k ${KEY_ID} --sign builder ${BUILD_DIR}/thebestcoin-${VERSION}.x86_64.deb
	cp ${BUILD_DIR}/thebestcoin-${VERSION}.x86_64.deb ${RELEASE_DIR}/lin

	fpm --deb-no-default-config-files \
	 -s tar \
	 -t deb \
	 --prefix usr/local \
	 -C thebestcoin-${VERSION}  \
	 -a i386 \
	 --after-install ${GIT_SRC}/share/postinst-deb \
	 --before-remove ${GIT_SRC}/share/prerm \
	 -n thebestcoin \
	 -v ${VERSION} \
	 -p ${BUILD_DIR}/thebestcoin-${VERSION}.i686-pc.deb  \
	 ${BUILD_DIR}/thebestcoin-${VERSION}-i686-pc-linux-gnu.tar.gz

	dpkg-sig -k ${KEY_ID} --sign builder ${BUILD_DIR}/thebestcoin-${VERSION}.i686-pc.deb
	cp ${BUILD_DIR}/thebestcoin-${VERSION}.i686-pc.deb ${RELEASE_DIR}/lin

	pushd ${RELEASE_DIR}/lin
	sha256sum thebestcoin-${VERSION}.i686-pc.deb > thebestcoin-${VERSION}.i686-pc.deb.sha256
	sha256sum thebestcoin-${VERSION}.x86_64.deb > thebestcoin-${VERSION}.x86_64.deb.sha256
	sha256sum thebestcoin-${VERSION}.x86_64.rpm > thebestcoin-${VERSION}.x86_64.rpm.sha256
	sha256sum thebestcoin-${VERSION}.i686-pc.rpm > thebestcoin-${VERSION}.i686-pc.rpm.sha256
	sha256sum thebestcoin-${VERSION}-x86_64-linux-gnu.tar.gz > thebestcoin-${VERSION}-x86_64-linux-gnu.tar.gz.sha256
	sha256sum thebestcoin-${VERSION}-i686-pc-linux-gnu.tar.gz > thebestcoin-${VERSION}-i686-pc-linux-gnu.tar.gz.sha256
	popd
fi

if [[ $windows = true ]]
then
	bin/gbuild -j7 -m6000 \
	 --commit thebestcoin=${COMMIT} \
	 --url thebestcoin=${URL} \
	 --skip-image \
	 ${GIT_SRC}/contrib/gitian-descriptors/gitian-win.yml

	cp ${BUILD_DIR}/thebestcoin-${VERSION}-win-unsigned.tar.gz inputs/thebestcoin-win-unsigned.tar.gz

	osslsigncode sign \
	 -h sha256 \
	 -pkcs12 inputs/code.p12 \
	 -askpass \
	 -in ${BUILD_DIR}/thebestcoin-${VERSION}-win64-setup-unsigned.exe \
	 -out ${RELEASE_DIR}/win/thebestcoin-${VERSION}-win64-setup.exe

	osslsigncode sign \
	 -h sha256 \
	 -pkcs12 inputs/code.p12 \
	 -askpass \
	 -in ${BUILD_DIR}/thebestcoin-${VERSION}-win32-setup-unsigned.exe \
	 -out ${RELEASE_DIR}/win/thebestcoin-${VERSION}-win32-setup.exe

	cp ${BUILD_DIR}/thebestcoin-${VERSION}-win32.zip ${RELEASE_DIR}/win
	cp ${BUILD_DIR}/thebestcoin-${VERSION}-win64.zip ${RELEASE_DIR}/win
	pushd ${RELEASE_DIR}/win
	sha256sum thebestcoin-${VERSION}-win32-setup.exe > thebestcoin-${VERSION}-win32-setup.exe.sha256
	sha256sum thebestcoin-${VERSION}-win64-setup.exe > thebestcoin-${VERSION}-win64-setup.exe.sha256
	sha256sum thebestcoin-${VERSION}-win32.zip > thebestcoin-${VERSION}-win32.zip.sha256
	sha256sum thebestcoin-${VERSION}-win64.zip > thebestcoin-${VERSION}-win64.zip.sha256
	popd
fi

if [[ $osx = true ]]
then
	bin/gbuild -j7 -m6000 \
	 --commit thebestcoin=${COMMIT} \
	 --url thebestcoin=${URL} \
	 --skip-image \
	 ${GIT_SRC}/contrib/gitian-descriptors/gitian-osx.yml

	cp ${BUILD_DIR}/thebestcoin-${VERSION}-osx-unsigned.tar.gz inputs/thebestcoin-osx-unsigned.tar.gz
	cp ${BUILD_DIR}/thebestcoin-${VERSION}-osx-unsigned.tar.gz ${RELEASE_DIR}/osx/thebestcoin-${VERSION}-osx.tar.gz
	cp ${BUILD_DIR}/thebestcoin-${VERSION}-osx-unsigned.dmg ${RELEASE_DIR}/osx/thebestcoin-${VERSION}-osx.dmg
	pushd ${RELEASE_DIR}/osx
	sha256sum thebestcoin-${VERSION}-osx.dmg > thebestcoin-${VERSION}-osx.dmg.sha256
	sha256sum thebestcoin-${VERSION}-osx.tar.gz > thebestcoin-${VERSION}-osx.tar.gz.sha256
	popd
fi
