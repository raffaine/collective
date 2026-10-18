#!/bin/bash
# Convenience launcher inside core/1_simulation/src — delegates to repo-root scripts/start_dev.sh
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec "$SCRIPT_DIR/../../../scripts/start_dev.sh" "$@"
