# GUIA DE DIREÇÃO E PRODUÇÃO — EPISÓDIO 08 (ONDA E: EPSILON, IOTA, LAMBDA E PI — EDIÇÃO BRASIL & LUSOFONIA)

Use este guia ao gerar o áudio do **Episódio 08 da Rádio Lambda Brasil (`Onda E: Fronteiras, Ciência Aberta, Cultura e Mutirões Federados — Cenários Epsilon, Iota, Lambda e Pi`)** no Google NotebookLM.

---

## PASSO 1: CONFIGURAÇÃO DA FONTE NO NOTEBOOKLM
1. Crie um caderno dedicado para o **Episódio 08 (PT-BR)**.
2. Faça o upload **APENAS** de:
   - `doc/podcast/pt-br/episodes/ep08_source_pack.md`
3. Nas configurações de idioma de saída (*Output Language*) do Audio Overview, selecione **Português (Brasil)**.
4. Note que a duração em Português opera em **`Default` (Padrão)** — o prompt abaixo foi calibrado especificamente para dividir o tempo igualmente entre os 4 cenários (~25% cada) sem correr no final.

---

## PASSO 2: PROMPT DE CUSTOMIZAÇÃO DO NOTEBOOKLM (COPIAR E COLAR)
> **Nota**: O campo *"Customize"* do NotebookLM aceita no máximo **500 caracteres**. O bloco abaixo possui **489 caracteres** (com espaços) e cabe perfeitamente sem cortes.

```text
Vocês são Samuca Viana e Clara Linhares no Ep. 08 da Rádio Lambda Brasil (Onda E: Epsilon, Iota, Lambda e Pi). Como a duração é Padrão, dividam o tempo IGUALMENTE (~25% para cada cenário) sem correr no final! Cubram: Epsilon (Portal Zero-ID, chave de latão VLAN 40/30 e Bancos Comunitários BACEN), Iota (ciência N>=3 <=50bp, papel de 500 anos e Prior Art no INPI), Lambda (Roda de Choro sem gravação e Pontos de Cultura Lei 13.018/14) e Pi (Mutirões autodissolúveis e Tratados FROST).
```

---

## PASSO 3: CHECKLIST DE VERIFICAÇÃO PÓS-GERAÇÃO
Após gerar o áudio em Português, verifique se Samuca e Clara cumpriram os 5 pontos de controle:
- [ ] **Ritmo Equilibrado (`25% / 25% / 25% / 25%`)**: Dedicaram tempo equivalente a `Epsilon`, `Iota`, `Lambda` e `Pi`, sem atropelar `Lambda` e `Pi` nos minutos finais?
- [ ] **Cenário Epsilon ($\text{E}$)**: Explicaram o Portal Vizinho Zero-ID, a Chave Física de Latão (`VLAN 40` vs. `VLAN 30`), a *Carapaça como Serviço* sob **Ato Cooperativo (`Lei 5.764/71`)** e a integração com **Bancos Comunitários de Desenvolvimento (`BCDs` / Moedas Sociais reconhecidas pelo `BACEN`)**?
- [ ] **Cenário Iota ($\text{I}$)**: Detalharam o pré-registro criptográfico, a replicação $N \ge 3$ ($\le 50\,\text{bp}$), a paridade de 100% para Resultados Nulos, a encadernação em papel de trapo de 500 anos (`ISO 9706`) e a destruição preventiva de patentes via **Publicação Defensiva de *Estado da Técnica* no `INPI` (`Art. 11 da Lei 9.279/96`)** e proteção do patrimônio genético (`SISGEN / Lei 13.123/15`)?
- [ ] **Cenário Lambda ($\Lambda$)**: Cobriram a Roda de Choro/Samba sem gravação (com a **Chave-Faca de Latão** desconectando microfones), a luthieria nacional sem madeiras ameaçadas (`CITES`), a Lanterna de Decibéis (`ABNT NBR 10151`) e o enquadramento como **Ponto de Cultura (`Lei Cultura Viva nº 13.018/2014`)**?
- [ ] **Cenário Pi ($\Pi$)**: Explicaram a Malha Policêntrica 3D, os **Mutirões Federados Autodissolúveis (`autoDissolveOnCompletion: true`)** espelhados em **Convênios com Condição Resolutiva (`Arts. 127–128 CC`)**, as **Cooperativas Centrais (`Arts. 6º–8º da Lei 5.764/71`)** e a validade jurídica dos **Tratados Ricardianos `FROST` (`MP 2.200-2/2001`)**?
