#!/usr/bin/env sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
REPO_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/.." && pwd)
VENV_PATH="$REPO_ROOT/.venv"
REQUIREMENTS_PATH="$REPO_ROOT/requirements.txt"
PYTHON_BIN="${PYTHON:-python3}"

if ! command -v "$PYTHON_BIN" >/dev/null 2>&1; then
    echo "Python was not found. Install Python 3 or set PYTHON to a valid interpreter." >&2
    exit 1
fi

if [ ! -d "$VENV_PATH" ]; then
    "$PYTHON_BIN" -m venv "$VENV_PATH"
fi

"$VENV_PATH/bin/python" -m pip install --upgrade pip

if [ -f "$REQUIREMENTS_PATH" ]; then
    "$VENV_PATH/bin/python" -m pip install -r "$REQUIREMENTS_PATH"
fi

echo "Python virtual environment is ready at $VENV_PATH"
echo "Activate it with: . .venv/bin/activate"

