#!/bin/sh

in="$1"
out="$2"

xattr_flag="$3"
syscall_file="$(basename $1 .tbl)"

grep '^[0-9]' "$in" | sort -n | (
    if ([ "$xattr_flag" = "no_xattr_syscall" ] && [ "$syscall_file" = "syscall_32" ]); then

	    while read nr abi name entry compat; do
		case $nr in
		    22[6-9]) continue ;;
		    23[0-7]) continue ;;
		    *) ;;
		esac
		abi=`echo "$abi" | tr '[a-z]' '[A-Z]'`
		if [ -n "$compat" ]; then
		    echo "__SYSCALL_${abi}($nr, $entry, $compat)"
		elif [ -n "$entry" ]; then
		    echo "__SYSCALL_${abi}($nr, $entry, $entry)"
		fi
	    done

    elif ([ "$xattr_flag" = "no_xattr_syscall" ] && [ "$syscall_file" = "syscall_64" ]); then

	    while read nr abi name entry compat; do
		case $nr in
		    18[8-9]) continue ;;
		    19[0-9]) continue ;;
		    *) ;;
		esac
		abi=`echo "$abi" | tr '[a-z]' '[A-Z]'`
		if [ -n "$compat" ]; then
		    echo "__SYSCALL_${abi}($nr, $entry, $compat)"
		elif [ -n "$entry" ]; then
		    echo "__SYSCALL_${abi}($nr, $entry, $entry)"
		fi
	    done

    else

	    while read nr abi name entry compat; do
		abi=`echo "$abi" | tr '[a-z]' '[A-Z]'`
		if [ -n "$compat" ]; then
		    echo "__SYSCALL_${abi}($nr, $entry, $compat)"
		elif [ -n "$entry" ]; then
		    echo "__SYSCALL_${abi}($nr, $entry, $entry)"
		fi
	    done

    fi
) > "$out"
