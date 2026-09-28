#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")/.."

if [ -f .env ]; then
  declare -A __caller_overrides=()
  while IFS= read -r line; do
    case "$line" in
      ''|'#'*) continue ;;
    esac
    key="${line%%=*}"
    if [ -n "${!key+x}" ]; then
      __caller_overrides["$key"]="${!key}"
    fi
  done < .env

  set -a
  . ./.env
  set +a

  for key in "${!__caller_overrides[@]}"; do
    export "$key=${__caller_overrides[$key]}"
  done
  unset __caller_overrides
fi

if ! command -v docker >/dev/null 2>&1; then
  echo "ERRO: docker não encontrado no PATH. Instale o Docker antes de continuar." >&2
  exit 1
fi

if ! docker compose version >/dev/null 2>&1; then
  echo "ERRO: 'docker compose' (plugin v2) não encontrado. Instale o Docker Compose." >&2
  exit 1
fi

export BATSIM_UID="$(id -u)"
export BATSIM_GID="$(id -g)"

SIM_TIMEOUT="${SMOKE_TEST_SIMULATION_TIMEOUT_SECONDS:-60}"
SUCCESS_MARKER="success_rate=1.000000"

cleanup() {
  docker compose down --remove-orphans >/dev/null 2>&1 || true
}
trap cleanup EXIT

echo "Limpando execuções anteriores"
docker compose down --remove-orphans >/dev/null 2>&1 || true

echo "Buildando e subindo o scheduler"
docker compose up -d --build batsched

echo "Rodando o Batsim até a simulação terminar (timeout: ${SIM_TIMEOUT}s)"
set +e
logs="$(timeout "$SIM_TIMEOUT" docker compose run --rm -T batsim < /dev/null 2>&1)"
batsim_exit=$?
set -e

echo "$logs"

if [ "$batsim_exit" -eq 0 ] && grep -q "$SUCCESS_MARKER" <<<"$logs"; then
  echo "OK: simulação completa rodou do início ao fim com sucesso (scheduler: ${BATSCHED_ALGORITHM:-easy_bf})."
  exit 0
elif [ "$batsim_exit" -eq 124 ]; then
  echo "FALHA: a simulação não terminou dentro de ${SIM_TIMEOUT}s (possível travamento do scheduler)." >&2
  exit 1
else
  echo "FALHA: o Batsim terminou com código ${batsim_exit} ou sem a marca de sucesso esperada." >&2
  exit 1
fi
