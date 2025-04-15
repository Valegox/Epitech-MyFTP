#!/bin/bash 

if [ "$#" -ne 2 ]; then
    echo "USAGE: $0 host port"
    exit 0
fi

USERNAME="Anonymous"
PASS=""

HOST=$1
PORT=$2
MKFIFO=`which mkfifo`
PIPE=fifo
OUT=outfile
TAIL=`which tail`
NC="`which nc`"
TIMEOUT=1 #max time before reading server response


getcode()
{
  sleep $TIMEOUT
  local code=$1
  echo "Waiting for $code reply-code"
  local data=`$TAIL -n 1 $OUT |cat -e |grep "^$code.*[$]$" |wc -l`
  return $data
}

print_failed()
{
    echo "$1 test failed"
    echo "Expected reply-code: $2"
    echo "Received : ["`$TAIL -n 1 $OUT| cat -e`"]"
    echo "KO"
}

print_succeeded()
{
  echo "$1 test succeeded"
  echo "OK"  
  kill_client 2>&1 >/dev/null
}

launch_client()
{
  local host=$1
  local port=$2

  $MKFIFO $PIPE
  ($TAIL -f $PIPE 2>/dev/null | $NC $host $port &> $OUT &) >/dev/null 2>/dev/null

  echo "Connecting to $host : $port"
  sleep $TIMEOUT
  getcode 220
  if [[ $? -eq 1 ]]; then
    echo "Reply-code OK"
    return 1
  else
    echo "Connection to $host:$port failed"
    echo "Expected reply-code: 220"
    echo "Received : ["`tail -n 1 $OUT |cat -e`"]"
    return 0
  fi  
}

launch_test()
{
  local test_name=$1
  local cmd=$2
  local code=$3

  echo "Sending [$cmd^M$]"
  echo "$cmd" >$PIPE
  getcode $code
  if [[ ! $? -eq 1 ]]; then
    print_failed "$test_name" "$code"
    kill_client
    clean
  fi
  echo "Reply-code OK"
}

launch_test_2_parts()
{
  local test_name=$1
  local cmd_part_1=$2
  local cmd_part_2=$3
  local code=$4

  echo "Sending [$cmd_part_1 + $cmd_part_2^M$]"
  echo -ne "$cmd_part_1" >$PIPE
  sleep 2
  echo -ne "$cmd_part_2\r\n" >$PIPE
  getcode $code
  if [[ ! $? -eq 1 ]]; then
    print_failed "$test_name" "$code"
    kill_client
    clean
  fi
  echo "Reply-code OK"
}

kill_client()
{
  local nc=`which nc`

  if [ `pidof $TAIL | wc -l` -ne 0 ]
  then
    killall $TAIL &>/dev/null
  fi
  if [ `pidof $nc | wc -l` -ne 0 ]
  then
    killall $nc &>/dev/null
  fi  
  rm -f $PIPE $OUT &> /dev/null
}

clean()
{
  rm -f $PIPE $OUT log &>/dev/null
}

# Simple authentication with USER + PASS command
test00()
{
  local test_name="Authentication"

  launch_client $HOST $PORT
  if [[ ! $? -eq 1 ]]; then
    echo "KO"
    kill_client
    return
  fi

  launch_test "$test_name" "USER $USERNAME" 331
  launch_test "$test_name" "PASS $PASS" 230

  print_succeeded "$test_name"
  return
}

test01()
{
  local test_name="Working directory"

  launch_client $HOST $PORT
  if [[ ! $? -eq 1 ]]; then
    echo "KO"
    kill_client
    return
  fi

  launch_test "$test_name" "USER $USERNAME" 331
  launch_test "$test_name" "PASS $PASS" 230
  launch_test "$test_name" "PWD" 257
  launch_test "$test_name" "CWD src" 250
  launch_test "$test_name" "CDUP" 250

  print_succeeded "$test_name"
  return
}

test02()
{
  local test_name="Buffer management"

  launch_client $HOST $PORT
  if [[ ! $? -eq 1 ]]; then
    echo "KO"
    kill_client
    return
  fi

  launch_test "$test_name" "USER $USERNAME" 331
  launch_test "$test_name" "PASS $PASS" 230
  launch_test_2_parts "$test_name" "PW" "D" 257

  print_succeeded "$test_name"
  return
}

test00
test01
test02
clean