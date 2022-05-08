#!/bin/bash
#
# Copyright (c) 2019-2020 Synopsys, Inc. This Synopsys processor model
# captures an ASIP Designer Design Technique. The model and all associated
# documentation are proprietary to Synopsys, Inc. and may only be used
# pursuant to the terms and conditions of a written license agreement with
# Synopsys, Inc.  All other use, reproduction, modification, or distribution
# of the Synopsys processor model or the associated  documentation is
# strictly prohibited.
#

mydir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
projdir=$(readlink -f $mydir/..)

PARAMS=()

# argument parsing
while (( "$#" )); do
  case "$1" in
    -tmo)
      temp="${2%\"}"
      TIMEOUT="${temp#\"}m"
      echo "TIMEOUT=$TIMEOUT"
      shift
      ;;
    -msg)
      temp="${2%\"}"
      MSG="${temp#\"}"
      shift
      ;;
    --) # end argument parsing
      shift
      break
      ;;
    *) # preserve unknown arguments
      PARAMS+=("$1")
      ;;
  esac
  shift
done

# set arguments in their proper place
set -- "${PARAMS[@]}" "$@"

# import list_descendant_processes
source $projdir/bin/clean_on_interrupt.sh

remove_from_list()
{
  local arr=("$1")
  arr=( ${arr[@]/$2/} )
  echo "${arr[@]}"
}

# timeout
(
  _KILLER=$BASHPID
  trap '[[ $sleep_pid ]] && kill "$sleep_pid"' EXIT
  sleep "${TIMEOUT:-20m}" & sleep_pid=$!
  wait $sleep_pid
  sleep_pid=
  echo "${MSG:-ERROR: Timeout}"
  # descendants of our parent, don't include parent
  procs=$(list_descendant_processes "$$" false)
  # remove this timeout subshell
  procs=$(remove_from_list "$procs" "$_KILLER")
  kill -s 9 -- $procs
  wait $procs
) & 2> /dev/null
KILLER=$!
# NOTE: on timeout, $KILLER should not kill itself before finishing

cleanup() {
  rc=$?
  kill -0 "$KILLER" && kill -HUP "$KILLER" && wait "$KILLER" 2> /dev/null
  exit $rc
}
trap cleanup EXIT

# launch in background, such that timeout works
( "$@" ) &
PID=$!
wait $PID 2> /dev/null
exit $?