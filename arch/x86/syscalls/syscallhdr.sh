#!/bin/sh

in="$1"
out="$2"
my_abis=`echo "($3)" | tr ',' '|'`
prefix="$4"
offset="$5"

xattr_flag="$6"
syscall_file="$(basename $1 .tbl)"

fileguard=_ASM_X86_`basename "$out" | sed \
    -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/' \
    -e 's/[^A-Z0-9_]/_/g' -e 's/__/_/g'`
grep -E "^[0-9A-Fa-fXx]+[[:space:]]+${my_abis}" "$in" | sort -n | (
    echo "#ifndef ${fileguard}"
    echo "#define ${fileguard} 1"
    echo ""

    if ([ "$xattr_flag" = "no_xattr_syscall" ] && [ "$syscall_file" = "syscall_32" ]); then

	while read nr abi name entry ; do
	    case $nr in
		22[6-9]) continue ;;
		23[0-7]) continue ;;
		*) ;;
	    esac
	    if [ -z "$offset" ]; then
		echo "#define __NR_${prefix}${name} $nr"
	    else
		echo "#define __NR_${prefix}${name} ($offset + $nr)"
	    fi
	done

    elif ([ "$xattr_flag" = "no_xattr_syscall" ] && [ "$syscall_file" = "syscall_64" ]); then

	while read nr abi name entry ; do
	    case $nr in
		18[8-9]) continue ;;
		19[0-9]) continue ;;
		*) ;;
	    esac
	    if [ -z "$offset" ]; then
		echo "#define __NR_${prefix}${name} $nr"
	    else
		echo "#define __NR_${prefix}${name} ($offset + $nr)"
	    fi
	done

    else

	while read nr abi name entry ; do
	    if [ -z "$offset" ]; then
		echo "#define __NR_${prefix}${name} $nr"
	    else
		echo "#define __NR_${prefix}${name} ($offset + $nr)"
	    fi
	done

    fi

    echo ""
    echo "#endif /* ${fileguard} */"
) > "$out"
