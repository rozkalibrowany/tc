#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]:-$0}" )" >/dev/null && pwd )"


# todo move this
ulimit -c unlimited
mkdir -p /tmp/core
chmod u+rwx,g+rwxs,o+rwx /tmp/core
echo /tmp/core/core.%e.%p.%h.%t > /proc/sys/kernel/core_pattern;

mkdir -p /usr/local/ws/etc/{{service_name}} /usr/local/ws/log
touch /usr/local/ws/log/{{service_name}}.log
chmod u+rw,g+rw /usr/local/ws/log/{{service_name}}.log

export LD_LIBRARY_PATH=/usr/local/ws/lib:${LD_LIBRARY_PATH}
export PATH=/usr/local/ws/bin:${PATH}


# I think this function printfs to journalctl
function log()
{
	printf "$@" | systemd-cat -p info -t {{service_name}}
}

log starting
log "${PATH}"
log "${LD_LIBRARY_PATH}"
umask 0002
{{service_startup_command}}
exit $?

