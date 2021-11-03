#!/usr/bin/env bash

#
# Copyright (c) 2021 Project CHIP Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# cirque_android_docker_build.sh - utility for building and (optionally)
#                        pushing the Cirque Android Emulator Docker image.
#
# Example usage:
# $ ./cirque_android_docker_build.sh --push
#

set -ex

CHIP_ROOT="$(dirname "$0")/../.."
CIRQUE_RESOURCES=$CHIP_ROOT/third_party/cirque/repo/cirque/resources

ORG=connectedhomeip
IMAGE=android-emulator
VERSION=latest
ARGS=()

usage() {
    echo "Usage: $0 [--version <version> --push]" >&2
    exit 0
}

while (($#)); do
    case "$1" in
        --version)
            VERSION="$2"
            shift
            ;;
        --push) ARGS+=("--push") ;;
        --help) usage ;;
    esac
    shift
done

docker buildx build -t "$ORG/$IMAGE:$VERSION" -f $CIRQUE_RESOURCES/Dockerfile.android_emulator "${ARGS[@]}" $CIRQUE_RESOURCES
