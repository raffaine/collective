# GUIA DE DIREÇÃO E PRODUÇÃO — EPISÓDIO 07 (ONDA D: ETA, MU, THETA, NU E XI — EDIÇÃO BRASIL & LUSOFONIA)

Use este guia ao gerar o áudio do **Episódio 07 da Rádio Lambda Brasil (`Onda D: Veto da Árvore-Mãe, Desconstrução Urbana de 98%, Fundição Solar, Alfaiataria Zero-RAM e Mutirão Kigumi — Cenários Eta, Mu, Theta, Nu e Xi`)** no Google NotebookLM.

---

## PASSO 1: CONFIGURAÇÃO DA FONTE NO NOTEBOOKLM
1. Crie (ou atualize) o caderno dedicado ao **Episódio 07 (PT-BR)**.
2. Faça o upload **APENAS** do novo arquivo revisado:
   - `doc/podcast/pt-br/episodes/ep07_source_pack.md`
3. Nas configurações de idioma de saída (*Output Language*) do Audio Overview, selecione **Português (Brasil)**.
4. Note que a duração em Português opera em **`Default` (Padrão)** — o prompt abaixo foi calibrado especificamente para dividir o tempo igualmente entre os 5 cenários reais da Onda D (~20% cada) sem correr no final.

---

## PASSO 2: PROMPT DE CUSTOMIZAÇÃO DO NOTEBOOKLM (COPIAR E COLAR)
> **Nota**: O campo *"Customize"* do NotebookLM aceita no máximo **500 caracteres**. O bloco abaixo possui **493 caracteres** (com espaços) e cabe perfeitamente sem cortes.

```text
Vocês são Samuca Viana e Clara Linhares no Ep. 07 da Rádio Lambda Brasil (Onda D: Eta, Mu, Theta, Nu e Xi). Dividam o tempo IGUALMENTE (~20% cada) sem correr no final! Cubram: 1) Eta (Talhadia na dormência, Veto da Árvore-Mãe, coleta <=25% e RPPN 100% isenta de ITR), 2) Mu (Desconstrução 98%, inversão de taxa, composto 72h/estrubita e Terra Preta), 3) Theta (Reuso antes de fusão e vazamento 2 pessoas <=250ms), 4) Nu (Alfaiataria Zero-RAM OP_PURGE) e 5) Xi (Habraken, Kigumi e NBR 7190).
```

---

## PASSO 3: CHECKLIST DE VERIFICAÇÃO PÓS-GERAÇÃO
Após gerar o áudio em Português, verifique se Samuca e Clara cumpriram os 5 pontos de controle:
- [ ] **Ritmo Equilibrado (`20% x 5 Cenários`)**: Dividiram o tempo equilibradamente entre `Eta`, `Mu`, `Theta`, `Nu` e `Xi`, sem espremer `Nu` e `Xi` nos últimos 2 minutos?
- [ ] **Cenário Eta (`SCN-ETA-COPPICE`)**: Explicaram a agrofloresta de 7 estratos com talhadia rotacional na dormência da seiva, o **Veto do Polígono da Árvore-Mãe (`ERR_MOTHER_TREE_SANCTUARY_VETO`)**, o teto de coleta silvestre de 1-em-4 ($\le 25\%$), as estufas solares ($\le 12\%$ estrutural / $\le 8\%$ luthieria), a Botica de Ensaio Duplo (`amatoxin == 0 ng`) e a **isenção de 100% do `ITR` via `RPPN` (`Lei 9.985/2000`) e `SAFs em Reserva Legal`**?
- [ ] **Cenário Mu (`SCN-MU-EXTRACTION`)**: Detalharam a desconstrução urbana cirúrgica de $\ge 98\%$ em 5 fluxos com scanners `NIR`/`XRF`, a **Inversão da Taxa de Demolição** (onde proprietários externos pagam para O Coletivo desmontar prédios e ficar com a Peroba-Rosa e o cobre sob a `PNRS Lei 12.305/10`), o composto termofílico de **72h ininterruptas a $55^\circ\text{C}$** (resetando para `0 ms` se esfriar), a recuperação de $\ge 90\%$ do fósforo em **estrubita**, as retortas `TLUD` de ***Terra Preta de Índio*** e a quarentena `TAINTED_HEAVY_METAL_ASH`?
- [ ] **Cenário Theta (`SCN-THETA-FOUNDRY`)**: Explicaram a **Hierarquia de Materiais em 3 Níveis** (*Reuso Antes de Fusão* economizando $95\%$ da energia, exigindo `tier1CacheMissProof` antes de fundir no pico solar $\ge 85\%$), o **Enclave de Vazamento de Duas Pessoas com coincidência de $\le 250\text{ ms}$ e lavador de gases (`NR-12`/`NR-15`)**, o vidro borossilicato e a síntese solar de cloro-álcali (soda cáustica e cal)?
- [ ] **Cenário Nu (`SCN-NU-TEXTILES`) e Cenário Xi (`SCN-XI-ARCH`)**: Cobriram a Bacia de Fibras Zero-PFAS/Zero-Cr(VI), a **Alfaiataria e Sapataria *Goodyear-welt* em RAM Efêmera (`OP_PURGE_R0_LINEAGE`, zeroizando as medidas 3D do corpo sob a `LGPD`)**, o remendo *Sashiko*, a **Doutrina de Duas Vias de Habraken (`Support` com fator $2,5\times$ vs. `Infill` privado $R_0$ sem telemetria)**, os encaixes ***Kigumi* sem pregos com algarismos romanos** erguidos em mutirão de fim de semana e o **Pool Federado `CREA`/`CAU` (`NBR 7190:2022`, `NBR 16814` e `Lei ATHIS 11.888/08`)**?
