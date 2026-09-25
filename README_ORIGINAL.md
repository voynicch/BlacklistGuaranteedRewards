# Blacklist Guaranteed Rewards v0.1.0

Mod experimental para **Need for Speed: Most Wanted (2005) PC v1.3**.

## O que esta versão faz

A v0.1.0 usa o caminho já validado pela comunidade para permitir escolher **os 6 Bonus Markers** após derrotar um rival da Blacklist. Com isso, **Pink Slip + Unique Performance Upgrade não podem ser perdidos**: você pode reivindicar todos os markers.

> Importante: esta primeira versão **ainda não seleciona automaticamente somente Pink Slip + Unique Performance**. Eu não incluí um hook não verificado que pudesse corromper a progressão ou o save. O objetivo da v0.1.0 é termos uma base pequena e testável no seu Redux 3.04. A próxima etapa é rastrear a estrutura/tipo real dos rewards na tela e automatizar exatamente os dois desejados.

## Instalação

Copie:

`BlacklistGuaranteedRewards.asi`

para:

`<NFS Most Wanted>\scripts\`

Seu Redux já possui ASI Loader (`dinput8.dll`).

## Compatibilidade

- Alvo: NFSMW (2005) PC **v1.3 / Black Edition**.
- Pensado para coexistir com Redux e Extra Options.
- O plugin reaplica somente os quatro bytes do contador de Bonus Markers depois da inicialização, para não ser sobrescrito pelo Extra Options.
- Não altera arquivos do save diretamente.

## Como testar

1. Faça backup do save.
2. Entre no jogo com um rival da Blacklist ainda não derrotado.
3. Derrote o rival.
4. Na tela de Bonus Markers, confirme que é possível selecionar os **6**.
5. Verifique que o Pink Slip e o Unique Performance podem ser obtidos sem risco.

## Próxima etapa: Auto Exact Two

Para a versão 0.2 o alvo é:

- detectar a tela de recompensa;
- identificar o marker que contém o **Pink Slip**;
- identificar o **Unique Performance Upgrade**;
- selecionar somente esses dois;
- avançar a tela normalmente.

Isso precisa de um hook de reward-type confirmado em runtime. A documentação pública atual confirma os contadores de markers, mas não publica ainda um endereço estável para classificar cada marker antes da seleção.

## Créditos técnicos

Endereços dos Bonus Markers documentados originalmente por **nlgxzef / ExOptsTeam** e usados também pelo projeto **EveryBonusMarkerNFSMW** de RafaelRS04. Implementação deste pacote foi escrita separadamente para este teste.
