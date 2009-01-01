#!/bin/bash

VPOPMAILDIR=/var/vpopmail
LOG_PREFIX="qmail-spp (`basename $0`) [$$]:"

log() {
	[[ "${LOG_PREFIX}" == "" ]] && return
	logger "${LOG_PREFIX}: $@"
}

error() {
	echo "E451 Temporary problem in vchkuser (#4.3.5)"
	exit 1
}

nack() {
	echo "E511 Sorry, no mailbox here by that name (#5.1.1)"
	exit 1
}

ack() {
	exit 0
}

export PATH="${VPOPMAILDIR}/bin:/bin:/usr/bin"

# if the recipient is empty it does not exist
if [[ -z "${SMTPRCPTTO}" ]]; then
	log "invalid empty recipient"
	nack
fi

# check if the box name contains invalid characters
if echo ${SMTPRCPTTO} | grep -q "[^-0-9A-Za-z\.@_=]\+"; then
	log "invalid characters in recipient name: ${SMTPRCPTTO}"
	nack
fi

# get the domain name of the recipient
HOST=${SMTPRCPTTO##*@}

# get the user-name
EXT=${SMTPRCPTTO%%@*}

# check if the domain exists
HDIR=$(vdominfo -d ${HOST})

# if domain does not exist ...
if [[ $? -ne 0 ]]; then
	# ... and RELAYCLIENT or SMTPAUTHUSER is set, user is allowed to relay
	if [[ -n ${RELAYCLIENT} || -n ${SMTPAUTHUSER} ]]; then
		log "relaying email to: ${SMTPRCPTTO}"
		ack
	else
		log "no such domain: ${HOST}"
		nack
	fi
else
	# ... otherwise, sanitize alias domains and continue
	HOST=$(vdominfo -n ${HOST} | head -n1)
	SMTPRCPTTO=${EXT}@${HOST}
fi

# remove alias lines if any
HDIR=$(set -- $HDIR; echo $1)

# check the existence of the user
EDIR=$(vuserinfo -d ${SMTPRCPTTO})

if [[ $? -eq 0 ]]; then
	log "${SMTPRCPTTO} exists (vuserinfo)."
	ack
fi

# check with valias if an alias or an e-mail address of that name exists
if valias -s ${HOST} | grep -iq "^${SMTPRCPTTO} ->"; then
	log "${SMTPRCPTTO} exists (valias)."
	ack
fi

# if a .qmail-${BOX} file exists, then delivery is possible
if [[ -e ${HDIR}/.qmail-${EXT} ]]; then
	log "${SMTPRCPTTO} exists (.qmail-${EXT})."
	ack
fi

# if a .qmail-default file exists, then delivery is possible
# catchall is very bad, but possible
if [[ -e ${HDIR}/.qmail-default ]]; then
	if ! grep -E -q 'vdelivermail(.*)(bounce-no-mailbox|delete)' "${HDIR}/.qmail-default"; then
		log "${SMTPRCPTTO} exists (.qmail-default)."
		ack
	fi
fi

# ezmlm-list? remove things like -1176124408.5270.menbhdjbcdifngjljjgh
LIST=${EXT%-*}
LIST=${LIST%-accept}
LIST=${LIST%-reject}

if [[ "${LIST}" != "${BOX}" ]]; then
	# if a .qmail-${LIST}-default file exists, then delivery is possible (ezmlm: list-subscribe...)
	if [[ -e ${HDIR}/.qmail-${LIST}-default ]]; then
		log "${SMTPRCPTTO} exists (.qmail-${LIST}-default)."
		ack
	fi

	# no ezmlm-list. now check for DIR/.qmail-ext
	LISTEXT=${EXT##*-}

	if [[ -e ${HDIR}/${LIST}/.qmail-${LISTEXT} ]]; then
		log "${SMTPRCPTTO} exists (${LIST}/.qmail-${LISTEXT})."
		ack
	fi
fi

# special: ezmlm-list with listname-subscribe-email-or-more=domain.tld@domain.tld
# ${var%pattern} remove shortest pattern from right
LIST=${EXT%-subscribe-*=*.*}
LIST=${LIST%-unsubscribe-*=*.*}
LIST=${LIST%-accept-*=*.*}
LIST=${LIST%-allow-tc.*=*.*}
LIST=${LIST%-reject-*=*.*}
LIST=${LIST%-deny-*=*.*}
LIST=${LIST%-sc.*=*.*}
LIST=${LIST%-tc.*=*.*}
LIST=${LIST%-uc.*=*.*}
LIST=${LIST%-vc.*=*.*}

if [[ "${LIST}" != "${EXT}" ]]; then
	# if a .qmail-${LIST}-default file exists, then delivery is possible (ezmlm: list-subscribe...)
	if [[ -e ${HDIR}/.qmail-${LIST}-default ]]; then
		log "${SMTPRCPTTO} exists (.qmail-${LIST}-default)."
		ack
	fi
fi

# no other checks have prooven the existence of this email address
log "no such recipient: ${SMTPRCPTTO}"
nack
