#!/usr/bin/expect -f

spawn parted -a optimal /dev/sda rm 2
expect "Yes/No?"
send "Yes\r"
expect "Ignore/Cancel?"
send "Ignore\r"
expect "$ "

spawn parted -a optimal /dev/sda resizepart 1
expect "Yes/No?"
send "Yes\r"
expect "]?"
send "19GB\r"
expect "Ignore/Cancel?"
send "Ignore\r"
expect "$ "

spawn parted -a optimal /dev/sda mkpart extended 19GB 20GB
expect "Ignore/Cancel?"
send "Ignore\r"
expect "$ "

spawn parted -a optimal /dev/sda mkpart logical "linux-swap(v1)" 19GB 20GB
expect "Ignore/Cancel?"
send "Ignore\r"
expect "$ "

send "exit\r"