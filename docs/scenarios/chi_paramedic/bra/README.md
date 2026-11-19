# Cenário Qui: Triagem Clínica Zero-LLM, Atenção Primária, Luta Antimanicomial & Farmacopeia Aberta (`SCN-CHI-PARAMEDIC`)

* **Identificador:** `SCN-CHI-PARAMEDIC`
* **Onda Horizontal:** Onda F (Saúde, Resiliência, Envelhecimento, Justiça Transformativa & Desacoplamento Terminal)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 09):** [../../../../doc/podcast/pt-br/episodes/ep09_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep09_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Qui governa **como o bairro ou assentamento protege a saúde humana em todas as dimensões em que nossos corpos e mentes podem falhar—e como fecha o ciclo da química farmacêutica e fitoterápica de consumo humano com segurança analítica rigorosa**.

Seu escopo abrange dois pilares estreitamente acoplados:
* **Saúde Soberana em Seis Modalidades (`domestic_care_token_minting == 0` no Livro-Contábil):**
  1. *Triagem Aguda de Trauma & Emergência Cardíaca* (despacho de pager Zero-LLM em $< 500\,\mathrm{ms}$ por cordão vermelho físico, chegada de socorrista à cabeceira em $< 180\,\mathrm{s}$).
  2. *Atenção Primária, Medicina Preventiva, Odontologia Restauradora Atraumática & Obstetrícia/Parteiras* (em par com o `Cenário Delta`).
  3. *Manejo de Condições Crônicas, Fisioterapia & Apoio à Pessoa com Deficiência* (em par com a titularidade permanente em $R_0$ do `Cenário Tau`).
  4. *Saúde Mental Não-Carcerária & Suítes de Acolhimento em Crise* (**Reforma Psiquiátrica e Luta Antimanicomial — `Lei Federal nº 10.216/2001`**, legada por Nise da Silveira: zero despacho policial armado; suítes calmas de acolhimento entre pares).
  5. *Soberania Corporal, Reprodutiva & de Afirmação de Gênero* (protegida sob o Firewall Anti-Moralismo do `Cenário Ômicron` e enclave criptografado `R0_CLINICAL_ENCLAVE`).
  6. *Escalonamento Hospitalar e Cirúrgico Terciário Sem Fricção para o `SUS`* (pacotes clínicos `HL7/FHIR` minimizados em dados + procuração de parentesco escolhido para o **`SAMU 192` / Hospitais de Referência do `SUS — Lei 8.080/1990`**).
* **Química Analítica de Consumo Humano & Farmacopeia Aberta (`Farmácias Vivas RENISUS` + `Open-Insulin`):** Síntese e manipulação magistral de Medicamentos Essenciais da OMS (`Open-Insulin`, antibióticos, analgésicos, sais de reidratação oral, anestésicos locais e fitoterápicos padronizados do `Cenário Eta`) travadas fisicamente por ensaios analíticos duplos independentes de `HPLC` e `GC-MS` ($\ge 9.950\,\mathrm{bp}$ / $99{,}5\%$ de pureza e controle de endotoxinas).

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

Nos monopólios farmacêuticos e planos de saúde mercantilizados (Camada 7), a vulnerabilidade humana é explorada por quatro falhas sistêmicas:

1. **Extorsão Médica & Escassez Artificial de Medicamentos:** Cartéis farmacêuticos cobram fortunas por frascos de insulina, autoinjetores de epinefrina ou inaladores de asma cujos precursores custam centavos, enquanto planos de saúde privados negam exames na hora da emergência.
2. **Alucinações Fatais de "Chatbots de Triagem por IA (`LLM`)":** Empresas de tecnologia vêm inserindo Modelos de Linguagem Estocásticos (`LLMs`) em funis de atendimento médico, onde uma única dosagem alucinada ou atraso de 90 segundos no despacho de socorro durante sepse, AVC ou anafilaxia pode ser fatal.
3. **Resposta Policial Carcerária a Crises de Saúde Mental:** Acionar serviços convencionais para um vizinho em surto psíquico agudo ou crise de pânico frequentemente envia viaturas policiais armadas em vez de equipes de acolhimento—transformando crises de cuidado em violência ou internação manicomial.
4. **"Biohacking Caseiro" Sem Análise vs. Dependência Total de Caminhões:** Grupos alternativos que tentam produzir remédios sem instrumentação analítica (`HPLC`, `GC-MS`, ensaio `LAL` de endotoxinas) arriscam contaminações letais, enquanto comunidades $100\%$ dependentes de distribuidoras comerciais colapsam em 72 horas durante bloqueios logísticos (`Cenário Ípsilon`).

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Qui substitui tanto a extorsão médica corporativa quanto o improviso inseguro por cinco laços clínicos e analíticos:

1. **Despacho Determinístico de Trauma Zero-LLM (`< 500 ms` Alerta, `< 180 s` na Cabeceira):** Puxar o cordão vermelho físico na parede ou apertar o botão tátil de emergência ignora $100\%$ de qualquer código de IA/LLM—transmitindo um alerta determinístico Classe 1 diretamente aos pagers dos socorristas do bairro credenciados via `BBS+` (`Cenário Capa` `T2/T3_Medic`), que chegam com kit de trauma e desfibrilador (`DEA`) em menos de três minutos.
2. **Farmacopeia Aberta Travada por Ensaio Analítico Duplo (`HPLC` / `GC-MS` $\ge 9.950\,\mathrm{bp}$):** Biorreatores e laboratórios de síntese/manipulação (`Open-Insulin`, antibióticos essenciais, lidocaína, analgésicos, soro fisiológico estéril e tinturas das **Farmácias Vivas** do `Cenário Eta`) ficam mecanicamente trancados até que ensaios duplos independentes de `HPLC` e `GC-MS` comprovem concentração do princípio ativo dentro de $\pm 200\,\mathrm{bp}$ ($2\%$), pureza química $\ge 9.950\,\mathrm{bp}$ ($99{,}5\%$), endotoxina bacteriana $< 500\,\mathrm{mEU/mL}$ (`LAL`) e cadeia fria ininterrupta entre $[275.150,\, 277.150]\,\mathrm{mK}$ ($2{,}0\text{–}4{,}0^\circ\mathrm{C}$). Qualquer lote fora de especificação é desviado automaticamente para a pirólise de alta temperatura do `Cenário Mi` (`material_id = 99`).
3. **Clínica Integral de Atenção Primária, Odontologia, Obstetrícia & Prevenção:** Oferece exames laboratoriais de rotina, vacinação, odontologia preventiva e restauradora atraumática, fisioterapia e cuidado reprodutivo a custo interno zero (`0` tokens no Livro-Contábil).
4. **Suítes de Acolhimento Psíquico Não-Carcerário (`Lei 10.216/2001`):** Crises agudas de saúde mental, luto ou sobrecarga sensorial neurodivergente são acolhidas em suítes silenciosas de jardim sem câmeras (`Cenário Zeta`/`Sigma`) acompanhadas por terapeutas e pares treinados em redução de danos—jamais por forças policiais.
5. **Descompressão Automática do Socorrista & Ponte Terciária `HL7/FHIR` com o `SUS`:** No instante em que um chamado agudo de trauma se encerra, o `col-execd` aciona automaticamente uma pausa de **Descompressão e Hidroterapia (`Cenário Sigma`/`Zeta`)** para a equipe de socorro; e se o paciente precisar de cirurgia terciária ou UTI hospitalar, a lousa clínica gera com um toque um resumo `HL7/FHIR` minimizado em dados + a procuração de parentesco escolhido do `Cenário Ômicron` para a equipe do `SAMU 192` / hospital do `SUS`.

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`EmergencyTraumaDispatchIntent` (Classe 1 Zero-LLM):** Acionado pelo cordão vermelho físico, botão vestível ou radar de queda `60 GHz` do `Cenário Fi`; despacha o anel de socorristas mais próximo em $< 500\,\mathrm{ms}$.
* **`ClinicalAndPharmacopoeiaIntent`:** Agendamento de consulta de atenção primária, odontologia, obstetrícia ou acolhimento psíquico, ou solicitação de dispensação de medicamento (`domesticCareTokenMinting: 0`).
* **`TriageAndAnalyticalAssayAttestation`:** Co-assinatura física dupla (químico analítico + farmacêutico `CRF`) atestando pureza `HPLC`/`GC-MS` ($\ge 9.950\,\mathrm{bp}$), esterilidade e cadeia fria para destravar o lote no dispensário.
* **`TertiaryHospitalEscalationPacket`:** Exportação autorizada pelo paciente ou procurador `Ômicron` contendo sinais vitais, medicações administradas e diretivas de saúde para transferência ao `SAMU 192` / hospital do `SUS`.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Grafo Clínico Criptografado no Anel de Cuidado (`R0_CLINICAL_ENCLAVE`):** Em `https://w3id.org/collective/v1/`, prontuários médicos pessoais, notas de saúde mental e registros reprodutivos são criptografados estritamente para a chave de hardware do paciente e seu anel clínico designado—totalmente invisíveis para assembleias de bairro, guildas de oficina ou corretores de dados da Camada 7.
* **Intertravamento Físico "Ensaio Antes da Dispensação":** O `col-commonsd` e as travas solenoides da Camada 2 impedem mecanicamente a abertura do gabinete farmacêutico para qualquer lote sintetizado ou manipulado localmente que não possua uma `TriageAndAnalyticalAssayAttestation` válida com `purityBp >= 9950` e telemetria ininterrupta de cadeia fria `[275150, 277150] mK`.
* **Leitura Determinística Obrigatória Dupla (Químico + Farmacêutico):** Antes de liberar um lote farmacêutico, dois guardiões certificados (`Capa T3`) revisam uma **Leitura Determinística (*Read-Back*)** da integração dos picos do cromatograma `HPLC`, verificação de solventes residuais no `GC-MS` (`0` arraste tóxico) e ensaio de endotoxina antes de encostarem suas chaves físicas.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Pager Determinístico de Trauma Zero-LLM & Cordão Vermelho** | Todos os Vestíbulos & Cinto do Socorrista (`chi:responder`) | Caminho de hardware/firmware $100\%$ determinístico (`0` código LLM/NLU): puxar o cordão na parede aciona os socorristas de plantão em $< 500\,\mathrm{ms}$ com coordenadas exatas da sala e localização do `DEA`. |
| **Lousa de Ensaio `HPLC` / `GC-MS` da Farmacopeia Aberta** | Laboratório Analítico & Dispensário (`chi:pharma`) | Exibe curvas cromatográficas de pureza ao vivo ($\ge 9.950\,\mathrm{bp}$), logs das geladeiras ($275.150\text{–}277.150\,\mathrm{mK}$) e co-assinaturas de liberação; envia frascos vencidos automaticamente para a pirólise do `Cenário Mi`. |
| **Prontuário Clínico & Obstétrico Privado em $R_0$** | Consultório / Lousa do Paciente (`chi:clinical`, `r0:local_only`) | Prontuário criptografado offline para atenção primária, odontograma e acompanhamento crônico; imprime resumos `HL7/FHIR` em papel/QR se houver transferência hospitalar. |
| **Lacre NFC Colorido da Maleta de Trauma & Cadeia Fria** | Armários de Primeiros Socorros & Geladeiras de Insulina (`chi:responder`) | Lacre NFC sem tela nas bolsas de emergência: permite ver num relance que suprimentos de vias aéreas, hemorragia e anafilaxia estão completos, estéreis e na validade. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ morador}$):**
   * *Contexto:* Um morador mantendo uma maleta de trauma/primeiros socorros, medicamentos essenciais numa geladeira médica ligada à bateria de $48\,\mathrm{V}$ e um botão vestível de emergência pareado com os vizinhos da rua.
   * *Modelo de Governança:* **Guardião automatizado de validade & cadeia fria.** A Camada 6 permanece silenciosa até que uma caneta de epinefrina ou antibiótico se aproxime do vencimento ou a temperatura da geladeira oscile, enquanto apertar o botão de emergência alerta imediatamente os vizinhos pareados em $R_1$ e o `SAMU 192`.
2. **Anel de Socorristas Atendendo um Círculo de Bairro ($N_s = 2\text{ a }6\text{ socorristas/enfermeiros}, N_p > 1\text{ núcleo de bairro}$):**
   * *Contexto:* Um anel de socorristas de rua e posto de atenção primária/Farmácia Viva atendendo de $10\text{ a }40$ famílias ($N_p > 1$).
   * *Modelo de Governança:*
     * **Rodízio de Sobreaviso & Descompressão Pós-Chamado (`Cenário Sigma`):** Os socorristas do bairro alternam o sobreaviso do pager para que nenhum enfermeiro ou médico fique preso 24/7; todo atendimento de emergência concede automaticamente um escudo de descanso no `Cenário Sigma` e horário reservado na Casa de Banhos (`Cenário Zeta`).
     * **Escopo Clínico Estrito & Disciplina de Escalonamento:** As credenciais do `Cenário Capa` definem fronteiras claras (`T1` Primeiros Socorros/RCP $\to$ `T2` Socorrista/Técnico de Enfermagem $\to$ `T3` Enfermeiro/Obstetriz/Médico/Farmacêutico); qualquer caso além do escopo local aciona transferência imediata `HL7/FHIR` ao hospital terciário sem hesitação.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Clínica-Santuário Regional, Consultório Odontológico, Suíte de Acolhimento Antimanicomial e Laboratório Analítico de Farmacopeia ($N_s = 20\text{ a }150+$ médicos, enfermeiros, parteiras, dentistas, psicólogos/terapeutas ocupacionais, farmacêuticos e químicos analíticos) atendendo de $500\text{ a }5.000+$ moradores através dos anéis $R_2 \dots R_5$.
   * *Modelo de Governança:*
     * **Separação Absoluta entre Saúde e Saldo no Livro-Contábil (`domestic_care_token_minting == 0`):** Todo atendimento clínico, cirurgia, acolhimento psíquico, tratamento odontológico e medicamento dispensado é um direito incondicional $B_{\mathrm{baseline}} = 10.000\,\mathrm{bp}$—jamais cobrado ou racionado por reputação.
     * **Segregação Independente na Verificação Analítica (`HPLC`/`GC-MS`):** A equipe que sintetiza um lote farmacêutico (`Open-Insulin` ou antibióticos) **jamais** pode liberar seu próprio lote para o dispensário; um químico analítico e um farmacêutico `Capa T3` independentes precisam co-assinar o laudo $\ge 9.950\,\mathrm{bp}$ (`Cenário Iota`/`Capa`).

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Integração com Alta Complexidade do `SUS` (`SAMU 192` / Hospitais Terciários) & Fundo Catastrófico Cooperativo:** Utilizar centros cirúrgicos, tomografia/ressonância e UTIs da rede pública (`SUS`) ou conveniada para intervenções terciárias complexas.<br>**B. Compra Coletiva (`GPO`) de Precursores Farmacêuticos, Colunas `HPLC` & Insumos:** Adquirir precursores grau farmacopeico, colunas cromatográficas, fios de sutura ou vacinas ainda não produzidos na biorregião. | • **Ponte Universal com o `SUS (Lei 8.080/1990)` & Reserva Catastrófica ($\Phi_t$):** A atenção primária comunitária desonera os prontos-socorros públicos enquanto a carapaça cooperativa do `Cenário Épsilon` mantém um fundo médico solidário para que nenhum morador enfrente despesas médicas desamparado.<br>• **Rastreamento de Dependência Farmacêutica ($D_{\mathrm{medicina}}$):** Todo fármaco ou reagente importado é registrado no Observatório de 5 Domínios do Livro-Contábil (`Cenário Ômega`), priorizando a pesquisa de biorreatores abertos e síntese local (`Cenário Iota`) para os medicamentos essenciais mais críticos. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Marco das *Farmácias Vivas* (`Decreto 5.813/2006` / `Portaria MS 886/2010` / `RDC ANVISA 18/2013` / `RENISUS`), Manipulação Magistral (`RDC 67/2007`) & Reforma Psiquiátrica (`Lei 10.216/2001`):** Amparar legalmente a clínica cooperativa, a dispensação fitoterápica/magistral e as suítes de acolhimento antimanicomial.<br>**B. Imunidade Absoluta de Prontuários Clínicos contra Devassas (`Sigilo Médico CFM`, `LGPD` e `Camuflagem Semântica Mundana`):** Proteger prontuários sensíveis contra seguradoras, empregadores ou devassas externas. | • **Cooperativa de Saúde & Farmácia Viva/Magistral (`CRM`/`COREN`/`CRO`/`CRF`):** Pareia as credenciais internas do `Cenário Capa` com profissionais cooperados registrados em seus conselhos de classe, operando a botica clínica sob o marco brasileiro das **Farmácias Vivas (criado pelo Prof. Francisco José de Abreu Matos)** e da manipulação magistral, e as suítes psíquicas sob a **Lei Antimanicomial nº 10.216/2001 (legado de Nise da Silveira)**.<br>• **Inexistência de Prontuários na Camada 7 (`R0_CLINICAL_ENCLAVE` + `LGPD Art. 11`):** Prontuários vivem estritamente criptografados pela chave do paciente em $R_0$; o `col-adversaryd` guarda zero identidades de pacientes ou históricos clínicos. |
