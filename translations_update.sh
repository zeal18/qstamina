#!/bin/sh

LUPDATE=""

if [ `which lupdate-qt4` ] ; then
	LUPDATE="`which lupdate-qt4`"
elif [ `which lupdate` ] ; then
	LUPDATE="`which lupdate`"
fi

if [ "$LUPDATE" = "" ] ; then
	echo "Error: lupdate / lupdate-qt4 not found."
	exit 1
fi

cd `dirname $0`/src
$LUPDATE src.pro
