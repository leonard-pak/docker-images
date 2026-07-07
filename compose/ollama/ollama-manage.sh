#!/usr/bin/env bash
set -euo pipefail

COMPOSE_DIR="$HOME/reps/docker-images/compose/ollama"
COMPOSE_FILE="$COMPOSE_DIR/docker-compose.yaml"

cd "$COMPOSE_DIR" || exit 1

function status() {
    RUNNING=$(docker compose -f "$COMPOSE_FILE" ps --status running -q | wc -l)
    TOTAL=$(docker compose -f "$COMPOSE_FILE" ps -a -q | wc -l)

    if [ "$RUNNING" -gt 0 ] && [ "$RUNNING" -eq "$TOTAL" ] && [ "$TOTAL" -gt 0 ]; then
        STATUS_ICON="🟢"
    elif [ "$RUNNING" -gt 0 ]; then
        STATUS_ICON="🟡"
    else
        STATUS_ICON="🔴"
    fi

    echo "$STATUS_ICON"
}

function run() {
    docker compose -f "$COMPOSE_FILE" up -d && notify-send "Ollama" "Docker started"
}
    
function stop() {
    docker compose -f "$COMPOSE_FILE" stop && notify-send "Ollama" "Docker stopped"
}

function webui() {
    xdg-open http://localhost:3000
}

# ==============================
#   Usage
# ==============================

if [[ $# -eq 0 ]]; then
    usage
    exit 1
fi

while getopts ":irsw" opt; do
    case "$opt" in
        i) status ;;
        r) run ;;
        s) stop ;;
        w) webui ;;
        \?) echo "Unknown option: -$OPTARG" >&2; usage; exit 1 ;;
    esac
done