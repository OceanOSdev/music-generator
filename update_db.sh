#! /bin/bash

function warn_udb {
  echo -e "[\033[0;33mWARN\033[0m] update_db.sh: $1"
}

# Print info messages
function info_udb {
  echo -e "[\033[0;34mINFO\033[0m] update_db.sh: $1"
}

# generate compile_commands.json
function gen_compile_commands_file {
  if ! command -v bear &> /dev/null
  then
    warn_udb "bear not installed, could not generate compile_comands.json"
    return 1
  fi

  if ! bear -- make all-slow &> /dev/null 2>&1; then
    warn_udb "Error when running make, can not guarantee compile_commands.json is up to date"
    return 1
  fi

  info_udb "Compile commands db updated!"
}

# generate compile_commands.json
function gen_compile_commands_file_debug {
  if ! command -v bear &> /dev/null
  then
    warn_udb "bear not installed, could not generate compile_comands.json"
    return 1
  fi

  if ! bear -- make debug-slow &> /dev/null 2>&1; then
    warn_udb "Error when running make, can not guarantee compile_commands.json is up to date"
    return 1
  fi

  info_udb "Compile commands db updated!"
}

isDebug=false
args=()
for i in "$@"; do
  case $i in
    --debug)
      isDebug=true
      shift
    ;;
  --release)
    isDebug=false
    shift
    ;;
  *)
    args+=($i)
    ;;
esac
done

info_udb "Running 'make clean'"
make clean

if [ "${#args[@]}" -gt 0 ]; then
  echo "Unknown arguments: ${args[@]}"
  exit 1
fi

if [ "$isDebug" = true ]; then
  info_udb "Updating compile_comands.json with DEBUG set to true"
  gen_compile_commands_file_debug
else
  gen_compile_commands_file
fi
