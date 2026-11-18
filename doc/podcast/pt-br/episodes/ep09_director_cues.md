# GUIA DE DIREÇÃO E PRODUÇÃO — EPISÓDIO 09 (ONDA F PARTE 1: CHI, UPSILON E PHI — EDIÇÃO BRASIL & LUSOFONIA)

Use este guia ao gerar o áudio do **Episódio 09 da Rádio Lambda Brasil (`Onda F Parte 1: A Guarda da Vida — Cenários Chi, Upsilon e Phi`)** no Google NotebookLM.

---

## PASSO 1: CONFIGURAÇÃO DA FONTE NO NOTEBOOKLM
1. Crie um caderno dedicado para o **Episódio 09 (PT-BR)**.
2. Faça o upload **APENAS** de:
   - `doc/podcast/pt-br/episodes/ep09_source_pack.md`
3. Nas configurações de idioma de saída (*Output Language*) do Audio Overview, selecione **Português (Brasil)**.
4. Note que a duração em Português opera em **`Default` (Padrão)** — o prompt abaixo foi calibrado especificamente para dividir o tempo igualmente entre os 3 cenários (~33% cada) sem correr no final.

---

## PASSO 2: PROMPT DE CUSTOMIZAÇÃO DO NOTEBOOKLM (COPIAR E COLAR)
> **Nota**: O campo *"Customize"* do NotebookLM aceita no máximo **500 caracteres**. O bloco abaixo possui **491 caracteres** (com espaços) e cabe perfeitamente sem cortes.

```text
Vocês são Samuca Viana e Clara Linhares no Ep. 09 da Rádio Lambda Brasil (Onda F Parte 1: Chi, Upsilon e Phi). Como a duração é Padrão, dividam o tempo IGUALMENTE (~33% para cada cenário) sem correr no final! Cubram: Chi (trauma <500ms Zero-LLM, Luta Antimanicomial Lei 10.216/01, SUS e Farmácias Vivas RENISUS/HPLC >=99,5%), Upsilon (Keyline, ilhamento <16ms, refúgio R$ 0, Pôr-do-Sol de 72h e NUDEC Lei 12.608/12) e Phi (Sábios Eméritos, radar 60GHz sem câmera, teto <=20% e Testamento Vital CFM).
```

---

## PASSO 3: CHECKLIST DE VERIFICAÇÃO PÓS-GERAÇÃO
Após gerar o áudio em Português, verifique se Samuca e Clara cumpriram os 4 pontos de controle:
- [ ] **Ritmo Equilibrado (`33% / 33% / 33%`)**: Dedicaram tempo equivalente a `Chi`, `Upsilon` e `Phi`, sem atropelar `Phi` nos minutos finais?
- [ ] **Cenário Chi ($\text{X}$)**: Explicaram o Cordão Vermelho `< 500 ms` Zero-LLM (`< 180 s` chegada física), as Suítes de Acolhimento Psíquico sem polícia ancoradas na **Lei da Reforma Psiquiátrica Antimanicomial (`Lei 10.216/2001` e Nise da Silveira)**, a ponte `HL7/FHIR` com o **`SUS`** e as **Farmácias Vivas (`RENISUS` / `RDC ANVISA 18/2013`)** com trava dupla `HPLC` + `GC-MS` $\ge 9.950\,\text{bp}$?
- [ ] **Cenário Upsilon ($\Upsilon$)**: Detalharam a engenharia hidrológica *Keyline* + *Terra Preta* contra enchentes e deslizamentos, o ilhamento elétrico `< 16 ms`, o Refúgio de Porta Aberta a `R$ 0,00`, o **Pôr-do-Sol Constitucional de 72 Horas**, o Jubileu de Choque e os **`NUDECs` (`Lei 12.608/2012`) / `RENER ANATEL`**?
- [ ] **Cenário Phi ($\Phi$)**: Cobriram o estatuto de **Sábio Emérito (`T4_Emeritus_Sage`, zero decaimento)**, o radar `60 GHz` sem câmera, o teto anti-burnout de $\le 20\%$ por cuidador, a Vigília Sem Telas sob o **Testamento Vital (`Res. CFM 1.995/2012`) e Política Nacional de Cuidados Paliativos do SUS (`Portaria MS 3.681/2024`)**, e os Bosques Memoriais de Ipês e Castanheiras (`CONAMA 335/2003`)?
