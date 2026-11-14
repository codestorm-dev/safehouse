# SafeHouse — Simulador 3D de Segurança Residencial

O SafeHouse é um simulador de sistema de segurança para casas. Ele mostra uma casa em 3D com 23 sensores e permite testar invasões e emergências para ver o que cada sensor detecta, como a casa reage e o que chega no celular do morador.

**Site online:** https://luizprestes-ops.github.io/safehouse/

## Grupo

- Luiz Gustavo Silva Prestes
- Marcos Eduardo Martins da Silva
- Gilmar Victor Gama Da Silva
- Joao Gabryel Ferreira

**Professora:** Flavia

## O que o simulador tem

- Casa 3D com 5 cômodos (quarto, banheiro, cozinha, sala e garagem), quintal, muro, rua e vizinhança, de dia e de noite.
- 23 sensores de 7 tipos: porta/janela (magnético), movimento (PIR), quebra de vidro, câmera, barreira infravermelha no muro, fumaça e gás.
- 3 modos de alarme: **Desarmado**, **Em casa** e **Ausente**.
- 6 cenários: muro dos fundos, janela da sala, portão da garagem, vulto no quintal, princípio de incêndio e vazamento de gás.
- Resposta automática: sirene, luzes, travamento das portas, câmeras gravando, central de monitoramento, polícia (190) e bombeiros (193).
- Celular do morador com as notificações, registro de eventos e orçamento estimado da instalação (R$ 7.790 + R$ 129/mês).

## Arquivos

| Arquivo | Para que serve |
|---|---|
| `index.html` | O site completo: a casa 3D, a central de controle, os cenários e o app do morador. Tudo (HTML, CSS e JavaScript) está neste arquivo. |
| `README.md` | Este texto de apresentação do projeto. |
| `.gitignore` | Lista de arquivos que não vão para o GitHub. |

## Como abrir e usar

Não precisa instalar nem compilar nada.

1. Abra o link do site online **ou** baixe o `index.html` e dê dois cliques nele (abre no Chrome, Edge ou Firefox).
2. É preciso estar conectado à internet, porque a biblioteca 3D (Three.js) e as fontes são carregadas da internet.
3. Gire a casa arrastando com o mouse, aproxime com a rodinha e clique em um sensor para ver como ele funciona.
4. Escolha o modo do alarme, ligue ou desligue os sensores e clique em um cenário para ver a simulação.

## Tecnologias

- **HTML e CSS:** estrutura e visual do painel de monitoramento.
- **JavaScript:** lógica dos sensores, modos, cenários e respostas automáticas.
- **Three.js (WebGL):** desenho da casa 3D, luzes, sombras, pessoas e viaturas.
- **Canvas:** texturas geradas pelo próprio código (grama, madeira, piso, telhado) e foto da câmera.
- **Web Audio API:** som da sirene gerado pelo código.

O código foi escrito com a ajuda de uma inteligência artificial (Claude Code), a partir das ideias e decisões do grupo.
