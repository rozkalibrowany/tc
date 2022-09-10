#!/bin/bash
# @brief Only basic aliases, no complicated stuff
##

DIR="$( cd "$( dirname "${BASH_SOURCE[0]:-$0}" )" && pwd )"

cWsWrkDir=${cWsWrkDir-~/ws}
cWsGitDir=${cWsGitDir-${cWsWrkDir}/git}
cWsGitRgDir=${cWsGitRgDir-${cWsGitDir}/tc}
cWsToolDir=${cWsToolDir-${cWsGitDir}/tc/tool}
cWsAnsibleDir=${cWsAnsibleDir-${cWsToolDir}/ansible}
cWsPrefixDir=${cWsPrefixDir-/usr/local/ws}


function ws-lsync ()
{
	if [ "$#" -ne 1 ]; then
	echo "Illegal number of parameters";
		return 1;
	fi;
	WsLsyncSource=${HOME}/ws WsLsyncTarget=/home/ws/ws WsLsyncHosts="$1" WsLsyncExclude=${DIR}/.lsyncd.exclude lsyncd ${DIR}/.lsyncd.lua
}

function ws-ansible-run-playbook()
{
	if [ "$#" -lt 1 ]; then
		echo "At least one paramter (target) is required"
		return 1
	fi
	target=${1}
	shift

	ANSIBLE_CONFIG="${cWsAnsibleDir}/ansible.cfg" \
	ansible-playbook -i "${cWsAnsibleDir}/hosts.ini" "${cWsAnsibleDir}/playbooks/${target}.yml" "$@"

}

function ws-ansible()
{
	# run roles, selected by specifing tags, on a single host
	if [ "$#" -lt 1 ]; then
		echo
		echo " ws-ansible - select roles by tags and then run them on a single host"
		echo
		echo "Usage:"

		echo -ne "\t"
		echo " ws-ansible HOST TAGS [REST]"

		echo -ne "\n\t"
		echo "HOST - Hostname from inventory file (hosts.ini), passed to --limit option of ansible-playbook"

		echo -ne "\n\t"
		echo "TAGS - Comma sperated tags used to select what tasks to run. Passed to the --tags option of ansible-playbook. Available tags can be found in  ws-ansible.yml playbook"

		echo -ne "\n\t"
		echo "REST - Rest of options will be passed to ansible-playbook command"
		echo
		return 1
	fi

	host=${1}
	shift
	tags=${1}
	shift

	ANSIBLE_CONFIG="${cWsAnsibleDir}/ansible.cfg" \
	ansible-playbook -i "${cWsAnsibleDir}/hosts.ini" "${cWsAnsibleDir}/playbooks/ws-ansible.yml" --limit ${host} --tags ${tags} "$@"
}