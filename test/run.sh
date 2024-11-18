#!/bin/bash

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
include_path="$script_dir/../include"
sources=(
  "$script_dir/../src/network.c"
  "$script_dir/../src/config.c"
  "$script_dir/../src/log.c"
)

gcc -o "$script_dir/test" "$script_dir/test.c" "${sources[@]}" -I"$include_path" -Wall &&
"$script_dir/test"
