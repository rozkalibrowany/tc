#!/bin/bash
# @brief Only basic aliases, no complicated stuff
##

DIR="$( cd "$( dirname "${BASH_SOURCE[0]:-$0}" )" && pwd )"

cWsWrkDir=${cZosWrkDir-~/ws}

function ws-lsync ()
{
	if [ "$#" -ne 1 ]; then
	echo "Illegal number of parameters";
		return 1;
	fi;
	WsLsyncSource=${HOME}/ws WsLsyncTarget=/home/ws/ws WsLsyncHosts="$1" WsLsyncExclude=${DIR}/.lsyncd.exclude lsyncd ${DIR}/.lsyncd.lua
}
