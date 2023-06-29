#!/bin/bash
cd `dirname $0`
cd ..

./misc/cloc/cloc \
  ./src \
  ./inc \
  ./examples/suit_manifest_process_main_min.c \
  ./examples/suit_examples_cose.c \
  ./examples/inc/suit_examples_cose.c \
  ./examples/inc/trust_anchor_prime256v1_cose_key_public.h

