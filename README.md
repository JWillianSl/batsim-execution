# batsim-execution

Ambiente Docker reprodutível para rodar o simulador [Batsim](https://batsim.readthedocs.io/).


## Pré-requisitos

- Docker e Docker Compose instalados.

## Como rodar

```bash
./scripts/smoke-test.sh
```

Na primeira execução, o build do `batsched` leva 1-2 minutos.
Se tudo estiver correto, você verá o log completo
da simulação seguido de `OK: simulação completa rodou do início ao fim com sucesso`
e o script encerra com código de saída `0`. Os dois
containers são derrubados automaticamente ao final.

**Nota:** Se o smoke test for executado contra um arquivo de workload
deliberadamente malformado, o binário Batsim pode gerar um arquivo
`core.*` no diretório `data/`.

## Configuração

Todos os parâmetros relevantes ficam no arquivo `.env.example`, na raiz do
repositório. Copie o template:

```bash
cp .env.example .env
```

## Estrutura

```
batsim-execution/
├── docker/
│   └── batsched/          # build do scheduler batsched
├── docker-compose.yml     # serviços Batsim + batsched
├── .env.example           # template de configuração
├── data/
│   ├── SOURCE.md          # origem dos arquivos de exemplo
│   ├── platforms/         # plataformas de simulação (XML)
│   ├── workloads/         # cargas de trabalho de exemplo (JSON)
│   └── output/            # saída gerada pelo Batsim (ignorada pelo git)
└── scripts/
    └── smoke-test.sh      # valida uma simulação completa do início ao fim
```