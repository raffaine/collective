# GUIA DE DIREÇÃO E PRODUÇÃO — EPISÓDIO 10 (ONDA F PARTE 2: PSI, RHO E OMEGA — EDIÇÃO BRASIL & LUSOFONIA)

Use este guia ao gerar o áudio do **Episódio 10 da Rádio Lambda Brasil (`Onda F Parte 2: Justiça Restaurativa, Imunidade Autopoética e o Farol Planetário — Cenários Psi, Rho e Omega`)** no Google NotebookLM.

---

## PASSO 1: CONFIGURAÇÃO DA FONTE NO NOTEBOOKLM
1. Crie um caderno dedicado para o **Episódio 10 (PT-BR)**.
2. Faça o upload **APENAS** de:
   - `doc/podcast/pt-br/episodes/ep10_source_pack.md`
3. Nas configurações de idioma de saída (*Output Language*) do Audio Overview, selecione **Português (Brasil)**.
4. Note que a duração em Português opera em **`Default` (Padrão)** — o prompt abaixo foi calibrado especificamente para dividir o tempo igualmente entre os 3 cenários (~33% cada) sem correr no final e anunciar o Grande Final no Episódio 11.

---

## PASSO 2: PROMPT DE CUSTOMIZAÇÃO DO NOTEBOOKLM (COPIAR E COLAR)
> **Nota**: O campo *"Customize"* do NotebookLM aceita no máximo **500 caracteres**. O bloco abaixo possui **492 caracteres** (com espaços) e cabe perfeitamente sem cortes.

```text
Vocês são Samuca Viana e Clara Linhares no Ep. 10 da Rádio Lambda Brasil (Onda F Parte 2: Psi, Rho e Omega — penúltimo episódio!). Como a duração é Padrão, dividam o tempo IGUALMENTE (~33% para cada cenário) sem correr no final! Cubram: Psi (bypass <500ms Lei Maria da Penha, Lareira da Paz sem microfone e Arbitragem Lei 9.307/96), Rho (Falha Antes de Malícia, Art. 5º XVIII CF/88 e LGPD Art. 6º III) e Omega (5 domínios <=200bp, Fundo Lei 13.800/19, semana de 15h e anúncio do Final no Ep. 11).
```

---

## PASSO 3: CHECKLIST DE VERIFICAÇÃO PÓS-GERAÇÃO
Após gerar o áudio em Português, verifique se Samuca e Clara cumpriram os 4 pontos de controle:
- [ ] **Ritmo Equilibrado (`33% / 33% / 33%`) e Gancho para o Ep. 11**: Dedicaram tempo equivalente a `Psi`, `Rho` e `Omega` e trataram o Episódio 10 como o **penúltimo episódio**, convidando os ouvintes para o Grande Final da Temporada no Episódio 11?
- [ ] **Cenário Psi ($\Psi$)**: Explicaram o Bypass de Segurança Física `< 500 ms` integrado à **`Lei Maria da Penha (Lei 11.340/2006)`**, as Lareiras da Paz Sem Microfone (Chave-Faca de Latão), o `SystemicRootCauseIntent`, a personalidade ecológica da Bacia Hidrográfica ($F_{\mathrm{eco}}$), o Direito de Fissão Graciosa e a blindagem pela **`Lei de Arbitragem (Lei 9.307/1996)` e `Lei de Mediação (Lei 13.140/2015)`**?
- [ ] **Cenário Rho ($\text{P}$)**: Detalharam a doutrina **"Falha Antes de Malícia"**, o isolamento cibernético da `VLAN 40` em `< 50 ms`, e a Esterilidade Forense fundamentada na **vedação constitucional de interferência estatal em associações/cooperativas (`Art. 5º, XVIII da CF/88`)** e no **Princípio da Minimização de Dados da `LGPD` (`Lei 13.709/2018, Art. 6º, III` e `Art. 16`)**?
- [ ] **Cenário Omega ($\Omega$)**: Cobriram o Observatório de 5 Domínios ($D_t \le 200\,\text{bp}$), a zeroização da `VLAN 40` em `< 100 ms`, a consolidação perpétua via **`RPPN` (`100% isenta de ITR`) + `Fundo Patrimonial Perpétuo (Lei 13.800/2019)`**, a semana de trabalho de $\le 15\text{ horas}$ e o **Farol Planetário (`R6`)**?
