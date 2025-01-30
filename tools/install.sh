#!/bin/bash
cd "$(dirname "$0")/../build"
if ! make; then
  echo "build failed"
  exit 1
fi
cd ..
rm /opt/minesweeper/minesweeper 2>/dev/null || true
sudo mv build/minesweeper /opt/minesweeper/