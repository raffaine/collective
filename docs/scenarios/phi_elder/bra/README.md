# Cenário Fi: Envelhecimento Digno, Sábios Eméritos Sem Decaimento & Santuário de Vigília Paliativa (`SCN-PHI-ELDER`)

* **Identificador:** `SCN-PHI-ELDER`
* **Onda Horizontal:** Onda F (Saúde, Resiliência, Envelhecimento, Justiça Transformativa & Desacoplamento Terminal)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 09):** [../../../../doc/podcast/pt-br/episodes/ep09_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep09_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Fi governa **como as pessoas envelhecem com dignidade, fazem a transição do trabalho físico pesado para a mentoria intergeracional honrada (`T4_Emeritus_Sage`), recebem cuidado diário livre de câmeras de vigilância e vivem a despedida da vida em vigílias paliativas domiciliares serenas e terramação ecológica em Bosques Memoriais**.

Ele cura diretamente duas distorções opostas da sociedade legada:
* **Contra o Asilamento Mercantil de Idosos:** Recusa-se a segregar os anciãos em casas de repouso comerciais solitárias, afastadas das crianças, das hortas e dos ofícios vivos; os idosos vivem em suítes térreas acessíveis sem degraus (`Cenário Zeta`/`Ksi`) abertas diretamente para os pátios compartilhados com as cirandas infantis do `Cenário Delta`, as cozinhas do `Cenário Gama` e as bancadas de mentoria do `Cenário Capa`.
* **Contra o Descarte Etário e Contra a Gerontocracia Autoritária (`wisdom_decay_rate_bp == 0`, `coercive_command_rank_bp == 0`):** Honra a experiência vivida e a memória ecológica multidecadal (zerando para sempre o decaimento reputacional no Livro-Contábil quando o corpo desacelera) enquanto garante que os anciãos jamais acumulem poder de comando coercitivo sobre os jovens—substituindo a chefia burocrática pelo conselho socrático procurado.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

No capitalismo tardio e nas instituições hierárquicas (Camada 7), o envelhecimento e o fim da vida são deformados por quatro falhas sistêmicas:

1. **Asilos Comerciais Extrativos & Epidemia de Solidão:** Clínicas geriátricas privadas drenam as economias de uma vida inteira ($\text{R\$ } 6.000\text{ a R\$ } 15.000/\text{mês}$) ou instituições precarizadas confinam idosos em alas estéreis, separados da convivência intergeracional do bairro.
2. **A Armadilha da "Filha ou Esposa Única Exausta" vs. Descarte Etário:** Quando uma pessoa idosa desenvolve demência ou fragilidade motora num apartamento nuclear isolado, uma única mulher da família frequentemente carrega $100\%$ do cuidado 24 horas por dia até adoecer; enquanto no mercado de trabalho os mais velhos são descartados como "obsoletos" assim que seu ritmo físico diminui.
3. **Vigilância Invasiva por Câmeras Ópticas de Queda:** Tecnologias comerciais para idosos instalam câmeras de vídeo conectadas à nuvem dentro dos quartos e banheiros, destruindo o pudor e a dignidade corporal em nome da segurança.
4. **Mortes Hipermedicalizadas em UTIs (Distanásia), Formol Tóxico & Litígios de Inventário:** Pessoas em fim de vida são frequentemente isoladas sob luzes fluorescentes de UTI entubadas contra sua vontade, embalsamadas com formol tóxico e seguidas por processos judiciais de inventário que liquidam as ferramentas e a terra da família (`Cenário Ômega`).

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Fi substitui o confinamento geriátrico e as câmeras invasivas por cinco laços de dignidade ao longo de todo o ciclo da vida:

1. **Estatuto Permanente de Sábio Emérito Sem Decaimento (`T4_Emeritus_Sage`: `wisdom_decay_rate_bp == 0`, `coercive_command_rank_bp == 0`):** Quando um mestre carpinteiro, parteira, agroflorestador ou engenheiro decide desacelerar dos turnos físicos pesados, sua maestria `T4` do `Cenário Capa` converte-se no estatuto permanente de **`T4_Emeritus_Sage` (Sábio Emérito)**. Sua voz reputacional e credenciais de mentoria no Livro-Contábil **jamais sofrem decaimento temporal**, e todo projeto comunitário de longo horizonte (manejo florestal de 25 anos no `Cenário Eta`, bacias de contenção de cheias de 50 anos no `Cenário Ípsilon`, círculos restaurativos no `Cenário Psi`) inclui uma **Consulta de Memória de Longo Horizonte** aos Sábios Eméritos—mantendo `coercive_command_rank_bp == 0` para que o conselho guie pela sabedoria e jamais por comando coercitivo.
2. **Radar de Queda `60 GHz mmWave` Sem Câmeras & Desligamento Automático de Fogão/Banheira:** As suítes dos idosos proíbem câmeras ópticas e microfones contínuos (`0` fluxos de vídeo/áudio). Em vez disso, sensores de radar de ondas milimétricas de $60\,\mathrm{GHz}$ no teto detectam quedas bruscas de postura ou imobilidade prolongada no banheiro (processando as nuvens de pontos estritamente na memória RAM volátil do microcontrolador local e descartando a forma de onda em milissegundos) para fazer uma confirmação suave de voz local e, se necessário, acionar a roda de cuidado e os socorristas do `Cenário Qui` em $< 500\,\mathrm{ms}$, além de cortar automaticamente fogões de indução esquecidos ligados.
3. **Teto de Dispersão do Cuidado Rotativo ($\le 2.000\,\mathrm{bp}$ / $20\%$ Máximo por Cuidador):** A companhia diária, partilha de refeições, passeios no jardim e auxílio no banho/mobilidade rodam entre uma roda intergeracional de cuidado de $5\text{ a }10$ vizinhos onde nenhum familiar ou voluntário individual carrega mais de $2.000\,\mathrm{bp}$ ($20\%$) das horas semanais de cuidado (`domestic_care_token_minting == 0`), impedindo o colapso do cuidador familiar (`Cenário Sigma`).
4. **Pátios Intergeracionais Acessíveis (`Delta` + `Fi` + `Lambda`):** Suítes térreas sem degraus, com banheiros acessíveis e piso aquecido, abrem diretamente para pátios ensolarados e seguros compartilhados com as cirandas infantis do `Cenário Delta` e os salões acústicos do `Cenário Lambda`—tornando a contação de histórias, a seleção de sementes crioulas e a música um ritmo diário natural.
5. **Vigílias Domiciliares de Cuidados Paliativos 100% Não-Mediadas (`Testamento Vital CFM 1.995/2012`) & Terramação Ecológica em Bosques Memoriais (`Cenário Eta`):** Diretivas Antecipadas de Vontade assinadas em hardware (e procuradores de parentesco escolhido do `Cenário Ômicron`) garantem que os desejos de fim de vida do ancião sejam honrados em casa, cercado por quem ama e pelo cuidado paliativo de conforto do `Cenário Qui` em um **Santuário 100% Não-Mediado** (com chave física de latão cortando toda telemetria não-paliativa). Após a partida, o corpo retorna suavemente à terra viva por redução orgânica natural (**terramação**) ou mortalha de linho em um **Bosque Memorial de Ipês, Castanheiras e Jequitibás do `Cenário Eta`**, enquanto o DID do ancião é consagrado permanentemente como uma `Wisdom_Anchor` (Âncora de Sabedoria, `Cenário Ômega`).

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`ElderStewardshipAndWisdomIntent`:** Transição para o estatuto de `T4_Emeritus_Sage`, solicitação de suíte térrea acessível sem degraus no `Cenário Zeta` ou configuração de uma roda intergeracional de cuidado (com teto $\le 2.000\,\mathrm{bp}$ por cuidador).
* **`LongHorizonMemoryConsultation`:** Convite aos Sábios Eméritos para anexar contexto histórico ou ecológico oral/escrito (ex.: onde o riacho transbordou na grande chuva de 1998 ou como uma geração anterior resolveu um defeito metalúrgico) a uma proposta maior dos `Cenários Eta`, `Ksi` ou `Ípsilon`.
* **`AdvanceDirectiveAndHospiceIntent`:** Autoria e assinatura física do Testamento Vital / Diretivas Antecipadas de Vontade (`Resolução CFM 1.995/2012`), plano de conforto paliativo domiciliar, legado de relíquias afetivas (`Cenário Ômicron`) e cerimônia escolhida no Bosque Memorial.
* **`ElderSafetyAttestation` (Reflexo de Borda mmWave):** Alerta local de `1 bit` de queda postural emitido pelo microcontrolador de radar para o anel de cuidado/socorristas com zero imagem ou áudio anexado.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Invariante Emérito de Decaimento Zero (`wisdom_decay_rate_bp == 0`):** No `col-kmsd` e no `col-commonsd`, a transição de um guardião para `T4_Emeritus_Sage` zera permanentemente o decaimento reputacional baseado no tempo no Livro-Contábil, mantendo `coerciveCommandRankBp: 0` e prioridade integral $10.000\,\mathrm{bp}$ no piso $B_{\mathrm{baseline}}$.
* **Leitura Determinística Obrigatória de Testamento Vital & Vigília:** Quando um idoso elabora ou atualiza seu `AdvanceDirectiveAndHospiceIntent`, sua lousa apresenta uma **Leitura Determinística (*Read-Back*)** pausada, falada em voz clara e impressa em letras grandes, confirmando suas preferências paliativas exatas, seus procuradores de saúde escolhidos (`Cenário Ômicron`) e a validade jurídica perante o `CFM`/`SUS` antes da assinatura física.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Companheiro Tátil & de Voz Offline `WCAG 2.2 AAA`** | Poltrona / Cabeceira da Suíte (`phi:companion`) | Botões físicos grandes de latão, agenda E-Ink de altíssimo contraste com letras grandes e leitura por voz TTS offline pausada; zero senhas, zero ícones minúsculos e zero câmeras. |
| **Quadro da Roda Intergeracional de Cuidado & Histórias** | Vestíbulo do Pátio (`phi:care_pod`, `delta:pod`) | Coordena visitas para o café/chá, caminhadas no jardim, horas de contação de histórias com as crianças e auxílio de mobilidade entre $5\text{ a }10$ vizinhos, impondo o teto de $\le 2.000\,\mathrm{bp}$ ($20\%$) por cuidador. |
| **Sensor de Postura por Radar `60 GHz mmWave` (Zero-Câmera)** | Teto do Banheiro / Quarto (`r0:edge_reflex`) | Radar não-óptico sem tela e sem lente: processa reflexos de ondas milimétricas na RAM volátil local para detectar quedas e fazer uma checagem suave de voz (*"Está tudo bem, Dona Maria?"*) antes de chamar a roda de cuidado. |
| **Chave-Faca Física de Latão da Vigília Paliativa Não-Mediada** | Entrada da Suíte (`r0:hardware_latch`) | Chave mecânica de latão que silencia alertas não-paliativos, aciona um polígono suave de silêncio `No-Transit` no pátio (`Cenário Beta`) e garante um espaço sagrado e 100% não-gravado para a despedida familiar. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Escala do Idoso Independente / Vida Solo ($N = 1 \implies N_s = 1\text{ idoso}, N_p = 1\text{ morador}$):**
   * *Contexto:* Uma pessoa idosa independente morando sozinha em sua casa ou suíte térrea que deseja autonomia total sem isolamento.
   * *Modelo de Governança:* **Independência soberana amparada por redes silenciosas de proteção.** O idoso governa sua própria rotina, usa o Companheiro Tátil/Voz para gravar memórias orais ou responder a consultas das guildas, e conta com o radar de queda `60 GHz` sem câmera, desligamento automático de boca de fogão e a visita diária carinhosa na entrega da marmita quente do `Cenário Gama`.
2. **Escala da Família Multigeracional / Roda de Cuidado ($N_s = 3\text{ a }8\text{ cuidadores da roda}, N_p = 1\text{ a }4\text{ idosos atendidos}$):**
   * *Contexto:* Uma família multigeracional ou roda de vizinhos apoiando pais ou moradores idosos com demência ou fragilidade motora ($N_p \ge 1$).
   * *Modelo de Governança:*
     * **Quebrando a Armadilha do Esgotamento da Cuidadora Única (Teto $\le 2.000\,\mathrm{bp}$ / $20\%$):** Na Camada 7, uma única filha ou cônjuge frequentemente carrega $100\%$ do cuidado de demência ou mobilidade até adoecer. O Cenário Fi distribui estruturalmente o cuidado entre pelo menos $5+$ membros da roda ($\le 20\%$ cada), apoiado pela Licença-Cuidado com Congelamento Duplo do `Cenário Sigma` para os familiares nas semanas mais delicadas.
     * **Caminhada Segura em Pátio Aberto para Idosos com Fragilidade de Memória:** Em vez de trancar o idoso num quarto ou sedá-lo, os pátios internos arborizados com caminhos circulares suaves permitem passear ao sol com segurança ao lado dos jardineiros do `Cenário Eta` e das cirandas do `Cenário Delta`.
3. **Comunidade Multi-Guardiões & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* O Conselho de Sábios Eméritos, o Anel de Cuidados Paliativos (`Cenário Qui`) e a Guilda dos Bosques Memoriais através dos anéis $R_1 \dots R_4$.
   * *Modelo de Governança:*
     * **Sabedoria Vivida Acima de Cargo Burocrático (`Consulta de Memória de Longo Horizonte`):** Antes que a assembleia ratifique uma grande mudança de uso do solo ou infraestrutura de 20 anos, o `col-commonsd` convida os Sábios Eméritos a aportarem contexto histórico oral ou escrito—garantindo que a comunidade aprenda com as décadas passadas sem conceder veto gerontocrático sobre a juventude.
     * **Liberação de Espólio & Continuidade Memorial (`Cenário Ômega`):** Na partida de um ancião, suas relíquias pessoais $R_0$ passam à família escolhida (`Ômicron`), as ferramentas de ofício $R_2$ retornam aos aprendizes do `Cenário Capa`, e sua história de vida e cadernos técnicos permanecem vivos na Biblioteca de Alexandria (`Cenário Iota`).

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Recebimento Integral de Aposentadorias, Pensões & `BPC/LOAS` (`INSS` / Previdência):** Garantir que os guardiões idosos recebam $100\%$ das aposentadorias, pensões e benefícios assistenciais para os quais contribuíram ou a que têm direito na Camada 7.<br>**B. Aquisição de Equipamentos de Mobilidade & Conforto Paliativo (`GPO`):** Importar camas articuladas, guinchos de transferência ou processadores de aparelhos auditivos. | • **Soberania Pessoal sobre a Aposentadoria:** Proventos de aposentadoria ou `BPC/LOAS` pertencem integralmente ao idoso em seu anel privado $R_0$—jamais são confiscados por uma tesouraria central.<br>• **Propriedade Assistiva Inviolável em $R_0$ (`Estatuto da Pessoa Idosa — Lei 10.741/2003` e `LBI Lei 13.146/2015`):** Qualquer andador, cadeira de rodas motorizada ou aparelho auditivo fornecido é titulado permanentemente ao idoso (`Cenário Tau`). |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Blindagem do Testamento Vital (`Resolução CFM 1.995/2012`), Tomada de Decisão Apoiada (`Art. 1.783-A CC`) & Cuidados Paliativos Domiciliares (`Portaria GM/MS 3.681/2024`):** Impedir que hospitais externos ou parentes distantes imponham distanásia contra a vontade do idoso ou promovam interdições predatórias.<br>**B. Conformidade Legal de Sepultamento Ecológico & Bosques Memoriais (`CONAMA 335/2003` e `RPPN`):** Proteger o retorno ecológico do corpo à terra nos Bosques Memoriais do `Cenário Eta`. | • **Compilação Ricardiana de Diretivas Antecipadas de Vontade (`Resolução CFM nº 1.995/2012`) & Procuração de Saúde (`Cenário Ômicron`):** Gera documentos físicos e assinados digitalmente (`ICP-Brasil`) co-assinados com médicos do `Cenário Qui`, integrados à **Política Nacional de Cuidados Paliativos do `SUS` (`Portaria GM/MS nº 3.681/2024`)**.<br>• **Bosques Memoriais em `RPPN (Lei 9.985/2000)` / Cemitério Parque Ecológico (`CONAMA nº 335/2003`) & Bloqueio Antiliquidação (`TTC`):** Consagra os Bosques Memoriais de Ipês e Castanheiras sob servidão perpétua e titularidade coletiva `TTC`, extinguindo para sempre o risco de venda forçada da terra comunitária em processos de inventário. |
