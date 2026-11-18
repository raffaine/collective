# GUIA DE DIREÇÃO E PRODUÇÃO — EPISÓDIO 06 (ONDA C: DELTA, OMICRON, KAPPA E ZETA — EDIÇÃO BRASIL & LUSOFONIA)

Use este guia ao gerar o áudio do **Episódio 06 da Rádio Lambda Brasil (`Onda C: O Casulo Pós-Parto de 90 Dias, Família Escolhida, Mestria sem Dívida e Casas sem Telas — Cenários Delta, Omicron, Kappa e Zeta`)** no Google NotebookLM.

---

## PASSO 1: CONFIGURAÇÃO DA FONTE NO NOTEBOOKLM
1. Crie (ou atualize) o caderno dedicado ao **Episódio 06 (PT-BR)**.
2. Remova a fonte antiga e faça o upload **APENAS** do novo arquivo revisado:
   - `doc/podcast/pt-br/episodes/ep06_source_pack.md`
3. Nas configurações de idioma de saída (*Output Language*) do Audio Overview, selecione **Português (Brasil)**.
4. Note que a duração em Português opera em **`Default` (Padrão)** — o prompt abaixo foi calibrado especificamente para dividir o tempo igualmente entre os 4 cenários reais da Onda C (~25% cada) sem correr no final.

---

## PASSO 2: PROMPT DE CUSTOMIZAÇÃO DO NOTEBOOKLM (COPIAR E COLAR)
> **Nota**: O campo *"Customize"* do NotebookLM aceita no máximo **500 caracteres**. O bloco abaixo possui **495 caracteres** (com espaços) e cabe perfeitamente sem cortes.

```text
Vocês são Samuca Viana e Clara Linhares no Ep. 06 da Rádio Lambda Brasil (Onda C: Delta, Omicron, Kappa e Zeta). Como a duração é Padrão, dividam o tempo IGUALMENTE (~25% cada) sem correr no final! Cubram: 1) Delta (Casulo Pós-Parto 90d, ciranda >=2 adultos sem câmera <=38°C e Banco de Leite Fiocruz), 2) Omicron (Família escolhida, Multiparentalidade STF/CNJ, Anti-Moralismo e saída <500ms), 3) Kappa (T1-T4, BKT privado em R0 e coleira BLE 2m) e 4) Zeta (Casa R0 sem telas, TTC/CDRU e Banhos).
```

---

## PASSO 3: CHECKLIST DE VERIFICAÇÃO PÓS-GERAÇÃO
Após gerar o áudio em Português, verifique se Samuca e Clara cumpriram os 5 pontos de controle:
- [ ] **Ritmo Equilibrado (`25% / 25% / 25% / 25%`)**: Dedicaram tempo equivalente a cada um dos 4 cenários (`Delta`, `Omicron`, `Kappa` e `Zeta`), sem atropelar `Kappa` e `Zeta` nos minutos finais?
- [ ] **Cenário Delta (`SCN-DELTA-CHILD`)**: Explicaram o **Casulo Pós-Parto de 90 Dias com Uma Só Assinatura** (marmitas `Gamma`, fraldas de pano a $71^\circ\text{C}$ + ozônio no `Beta`, licença `Sigma`), a **Salvaguarda Obrigatória de Dois Adultos ($N_{\mathrm{adults}} \ge 2$)** com teto ponderado por idade ($\le 40.000\text{ bp}$), a segurança física sem câmeras ($\le 38,0^\circ\text{C}$ anti-escaldadura) e a integração com a **Rede Brasileira de Bancos de Leite Humano (`Fiocruz/SUS`, pasteurização Holder a $62,5^\circ\text{C}$)**?
- [ ] **Cenário Omicron (`SCN-OMICRON-KIN`)**: Detalharam os pactos $N\text{-ários}$ de parentesco escolhido (`SovereignKinshipCovenant`), o Cartão Hospitalar compilado com a **Multiparentalidade (`Tema 622 STF` / `Provimentos CNJ 63 e 83`) e Diretivas `CFM 1.995/12`**, o **Firewall Anti-Moralismo** e a **Saída Unilateral de Porto Seguro em `< 500 ms`** (`SafeHarborSeveranceIntent` com reserva de $5\%$ de suítes de refúgio e `Lei Maria da Penha`)?
- [ ] **Cenário Kappa (`SCN-KAPPA-APPRENTICE`)**: Explicaram os 4 graus (`T1` a `T4`), o **Rastreamento Bayesiano (`BKT` $\ge 9.500\text{ bp}$) 100% privado em $R_0$** (onde todos os erros ficam só no dispositivo do aprendiz), a **Coleira `BLE` de $2,0\text{ metros}$ do mentor**, as **Bancas `3-de-3` Anti-Nepotismo** e a ponte com o **`Art. 23/24 da LDB` e `NR-12`**?
- [ ] **Cenário Zeta (`SCN-ZETA-LODGE`)**: Cobriram a moradia descomodificada a aluguel zero sob **`TTC`/`CDRU` e Bem de Família Impenhorável**, a fronteira **`R0_INTERIOR_DARK` (casas 100% sem telas, câmeras ou microfones, com interruptores táteis de latão)**, a **Casa de Banhos aquecida por serpentinas `DWHR` ($\ge 55\%$) e calor dos servidores `Zero`**, e os jardins filtrantes de 72h + câmaras de biochar a $\le -25\text{ Pa}$ sem odor?
