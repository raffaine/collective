# Cenário Sigma: Prevenção ao Burnout, Homeostase Somática & Cuidado Rotativo (`SCN-SIGMA-BURNOUT`)

* **Identificador:** `SCN-SIGMA-BURNOUT`
* **Onda Horizontal:** Onda A (Meta-Núcleo Autopoético, Acolhimento & Homeostase)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 04):** [../../../../doc/podcast/pt-br/episodes/ep04_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep04_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Sigma é o **sistema imunológico humano e somático** d'O Coletivo. Mesmo a comunidade ecologicamente mais perfeita pode se romper por dentro se as pessoas mais generosas e dedicadas entrarem em exaustão silenciosa (*burnout*) carregando nas costas a manutenção invisível, o cuidado e a organização enquanto outros se acomodam—ou se a pressão informal do grupo empurrar guardiões cansados para a sobrecarga crônica.

O Cenário Sigma governa o **equilíbrio metabólico entre contribuição humana, descanso, recuperação e automação**:
* **Descanso Incondicional com Zero Fricção (`RestShield` & Ferrolho de Madeira e Latão):** Garantindo que qualquer guardião possa recuar para o descanso a qualquer momento com um único gesto físico, zero explicação e zero penalidade no Livro-Contábil.
* **Disjuntores Constitucionais Anti-Martírio ($\le 4.000\,\mathrm{bp}$ / $40\%$):** Detectando quando o trabalho crítico de uma guilda se concentra numa única pessoa e convocando automaticamente suplentes e aprendizes (`Cenário Capa`) antes que o esgotamento aconteça.
* **Licença-Cuidado com Congelamento Duplo:** Protegendo guardiões durante doença, luto, gestação/pós-parto (`Cenário Delta`) ou cuidado de familiares (`Cenário Fi`) ao congelar simultaneamente o decaimento reputacional e os relógios de recência de certificações técnicas, mantendo $100\%$ do piso de sobrevivência.
* **Mutirões Rotativos Justos & Pipeline *Da Tarefa Penosa à Automação*:** Partilhando tarefas pesadas ou desagradáveis de limpeza e saneamento de forma horizontal sem precificação coercitiva em tokens, e convertendo o desgaste físico repetitivo em missões prioritárias de engenharia e automação para a oficina do `Cenário Alfa` e o `Cenário Zero`.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

Tanto o mercado de trabalho corporativo quanto movimentos sociais e coletivos históricos sofrem com quatro patologias destrutivas em relação ao trabalho e ao descanso:

1. **A Sociedade do Cansaço & O Panóptico da Produtividade:** Empresas implantam rastreadores de cliques, metas abusivas, rankings de produtividade e algoritmos de escala que espremem cada minuto da atenção do trabalhador até o colapso físico ou psíquico.
2. **A "Armadilha do Mártir" nos Coletivos (*A Tirania das Organizações sem Estrutura*):** Como diagnosticou Jo Freeman, em grupos informais sem salvaguardas estruturais, três ou quatro pessoas de coração enorme acabam lavando todas as panelas, consertando todas as bombas d'água e mediando todas as crises até adoecerem e abandonarem o projeto com amargura.
3. **Punição ao Adoecimento e ao Cuidado Familiar:** Na Camada 7, afastar-se por meses para cuidar de um pai idoso, recuperar-se de uma cirurgia ou viver um luto significa risco de demissão, perda de renda e vencimento de habilitações profissionais.
4. **Leilão Precarizado ("Uberização") das Tarefas Pesadas:** Plataformas que usam leilões reversos ou tokens de preço dinâmico para alocar tarefas sujas de saneamento criam inevitavelmente uma subclasse interna—forçando recém-chegados ou pessoas vulneráveis a fazerem todo o trabalho pesado enquanto membros antigos "compram sua dispensa".

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Sigma substitui a produtividade coercitiva e o martírio informal por cinco laços homeostáticos:

1. **Fricção Assimétrica entre Descanso e Compromisso (`RestShield` + Ferrolho Físico do Santuário):** Entrar em descanso exige no máximo **um toque ou o deslizar do ferrolho físico de madeira e latão na porta do quarto**, silenciando instantaneamente $100\%$ das notificações não-emergenciais e baixando o teto acústico ao redor do dormitório para $\le 300\,\mathrm{cB}$ ($30\,\mathrm{dBA}$ — silêncio de biblioteca).
2. **Termostato Somático Schmitt-Trigger Estritamente Local em $R_0$ (`8.500 bp` / `4.500 bp`):** O guardião pode registrar opcionalmente sua sensação subjetiva de energia/fadiga (de 1 a 5) apenas no seu dispositivo privado em $R_0$. Quando o esforço acumulado cruza o limiar superior ($\ge 8.500\,\mathrm{bp}$ / $85\%$), o dispositivo ativa o `RestShield` automaticamente com histerese até que a fadiga caia abaixo de $\le 4.500\,\mathrm{bp}$ ($45\%$)—sem jamais enviar batimentos cardíacos ou biometria para o servidor da comunidade.
3. **O Disjuntor Constitucional Anti-Martírio de 40% (`4.000 bp`):** A Camada 5 (`col-kmsd`) monitora a dispersão das tarefas concluídas nos domínios essenciais. Se qualquer guardião individual carregar $> 4.000\,\mathrm{bp}$ ($40\%$) da carga de um domínio crítico numa janela móvel, o sistema dispara um **Sinalizador de Alívio Anti-Martírio**—convocando enxames de apoio do `Cenário Pi` e abrindo bolsas prioritárias de mentoria no `Cenário Capa` para dividir a bancada.
4. **Escudo de Licença-Cuidado com Congelamento Duplo:** Emitir um `CareLeaveDeclarationIntent` congela **tanto** o decaimento da Riqueza Reputacional ($\mathcal{R}$) na Camada 5 **quanto** os cronômetros de recência das credenciais de segurança do `Cenário Capa` durante todo o afastamento, mantendo o piso $B_{\mathrm{baseline}} = 10.000\,\mathrm{bp}$ intacto.
5. **Mutirões Rotativos (`0` Tokens) & O Pipeline *Da Tarefa Penosa à Automação*:** Tarefas essenciais de limpeza e manutenção (revirar composto, desentupir drenos, faxina pesada da cozinha) circulam horizontalmente em **Mutirões Rotativos** com café, música do `Cenário Lambda` e `domestic_care_token_minting == 0`. Sempre que uma tarefa gera desgaste ergonômico repetido ou desistências por fadiga (como lavar 80 kg de mandioca e batata-doce barrenta na chuva fria), a Camada 6 compila automaticamente um **`DrudgeryAutomationQuestIntent`** para que a oficina do `Cenário Alfa` projete e construa um lavador rotativo de raízes em inox!

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`RestIntent` & `UNCLAIM_GUILT_FREE`:** Um sinal cego de `1 bit` emitido por um toque único ou pelo deslizar do ferrolho físico da porta do quarto, devolvendo qualquer tarefa não-crítica à fila coletiva com zero justificativa em texto e $\Delta \mathcal{R} = 0\,\mathrm{bp}$ no Livro-Contábil.
* **`CareLeaveDeclarationIntent`:** Ativação do escudo de Licença-Cuidado com Congelamento Duplo por semanas ou meses para doença, recuperação pós-parto (`Cenário Delta`), luto ou cuidado de idosos (`Cenário Fi`).
* **`DrudgeryAutomationQuestIntent`:** Escalonamento de um trabalho manual cronicamente penoso em uma missão prioritária de projeto mecânico/automação para o `Cenário Alfa` e o `Cenário Zero`.
* **`AntiMartyrdomReliefBeacon`:** Solicitação temporária de apoio entre quadras ou nós vizinhos quando uma pequena guilda enfrenta um gargalo agudo de pessoal.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Projeção Cega de Disponibilidade em 1 Bit:** Dentro do `col-commonsd`, o estado de um guardião em descanso aparece para o restante do nó exclusivamente como `STEWARD_UNAVAILABLE_RESTING`—jamais distinguindo se a pessoa está dormindo, de luto, doente ou de férias, e proibindo qualquer ranking público de "horas trabalhadas".
* **Limites de Histerese Schmitt-Trigger (`8500 bp` / `4500 bp`):** Impede a oscilação rápida de liga/desliga nas notificações, mantendo o guardião protegido até a recuperação somática completa.
* **Leitura Determinística para Automação e Apoio:** Enquanto entrar em descanso é instantâneo e livre de confirmações, transmitir um `AntiMartyrdomReliefBeacon` entre nós ou comissionar um `DrudgeryAutomationQuestIntent` exibe uma **Leitura Determinística (*Read-Back*)** mostrando a frequência anonimizada de esforço ergonômico e a solução mecânica ou de ajuda mútua proposta.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Ferrolho Físico de Madeira e Latão na Porta do Quarto** | Porta de Dormitório / Suíte Privada (`r0:hardware_latch`) | Trava mecânica sem tela ligada a um contato seco local (*reed switch*): fechar o ferrolho barra fisicamente a entrada, baixa o alvo acústico para $\le 300\,\mathrm{cB}$ e silencia todos os chamados de tarefas da Camada 6. |
| **Widget Homeostático `RestShield`** | Lousa Pessoal / Relógio / E-Ink (`sigma:rest`) | Botão único e calmo (`Descansar Agora` / `Devolver Tarefa`) sem caixas de diálogo culpabilizadoras; mantém registros subjetivos de fadiga (1 a 5) estritamente na memória volátil $R_0$. |
| **Quadro E-Ink de Mutirão & Missões de Ergonomia** | Cozinha / Galpão da Guilda (`sigma:workbee`) | Exibe a escala semanal do mutirão de cuidado (acompanhado de café e música) e permite sinalizar com um toque: *"Esta tarefa dá dor nas costas—automatizem na oficina Alfa!"*. |
| **Agendador Hidrônico da Casa de Banhos** | Vestíbulo da Casa de Banhos (`zeta:hospitality`, `sigma:rest`) | Reserva automaticamente banhos quentes de imersão aquecidos pelo calor residual dos servidores (`Cenário Zeta`) para socorristas (`Cenário Qui`), fundidores (`Cenário Teta`) e guardiões após turnos pesados. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Em todas as escalas, a governança da Camada 5 no Cenário Sigma ancora-se na fórmula unificada de peso de voto restrita estritamente aos **Membros do Nó ($N_s$)**:
$$V_i(t) = B + R_i(t) = 1 + R_i(t) \qquad \left(V_{i,\mathrm{bp}}(t) = 10.000 + R_{i,\mathrm{bp}}(t)\right)$$
onde $B = 1$ ($10.000\,\mathrm{bp}$) é a **Base Cívica Universal** não-decaível no Livro-Contábil Termodinâmico. Como $B=1$ jamais decai quando a taxa de trabalho físico de um guardião vai a zero ($\dot{r}_i(\tau) \to 0$ durante `RestIntent` ou `CareLeaveDeclarationIntent`), quem descansa nunca perde sua voz constitucional, e a evolução das quebras cegas do **Estágio I (Hardware Resgatado L7)** para as trocas preventivas de 5 minutos dos **Estágios II e III** conquista a **Libertação Zero-Stat (`D1`, `D7`)**.

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ pessoa atendida}$):**
   * *Contexto:* Um morador solitário cuidando da própria horta, sistema solar e bancada de oficina.
   * *Modelo de Governança:* **Ritmo autocompassivo & corte automático de carga.** Quando o guardião solitário fecha o Ferrolho do Santuário ou fica gripado, o `col-execd` coloca o sítio em **Modo Autonômico de Baixa Manutenção**—passando a irrigação e os comedouros das aves para temporizadores automáticos, pausando projetos não-essenciais da oficina e retendo alertas não-críticos até a recuperação.
2. **Guardião Único / Núcleo Pequeno Atendendo um Círculo Maior ($N_s = 1\text{ ou pequeno núcleo}, N_p > 1\text{ pessoas atendidas}$):**
   * *Contexto:* Um ou dois guardiões principais (ex.: o cozinheiro da rua, o mantenedor da oficina ou a mãe/pai cuidador) atendendo uma família extensa ou núcleo de $10\text{ pessoas}$ ($N_p > 1$).
   * *Modelo de Governança:*
     * **Protegendo Quem Cuida contra a Sobrecarga do Círculo:** Esta é a escala onde o *burnout* é mais perigoso na vida real. O Cenário Sigma impõe **filas de pedidos *pull-only*** para que os $N_p$ beneficiados não possam bombardear o guardião com cobranças diretas.
     * **Mobilização Automática do Círculo & Suplência:** Quando o guardião principal ativa o `RestIntent` ou a Licença-Cuidado, a Camada 6 aciona automaticamente pares habilitados no anel $R_1 / R_2$ (ou muda o núcleo para despensa de autoatendimento e entrega de marmitas da cozinha vizinha), garantindo que quem cuida possa descansar sem medo de deixar ninguém desassistido.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Guildas operando cozinhas, clínicas, fundições e infraestrutura para um bairro ou assentamento ($N_p = 100\text{ a }2.500+$).
   * *Modelo de Governança:*
     * **Teto Rígido de Concentração Individual ($\le 4.000\,\mathrm{bp}$ / $40\%$):** Nenhuma guilda pode depender de um único "herói insubstituível" para mais de $40\%$ de suas operações críticas. Cruzar esse limite congela automaticamente expansões não-essenciais e abre bolsas de aprendizado no `Cenário Capa` até que pelo menos $k \ge 3$ guardiões dominem a função.
     * **Proibição Constitucional de Tokenizar o Cuidado (`domestic_care_token_minting == 0`):** A assembleia é constitucionalmente proibida de transformar limpeza, cuidado ou apoio emocional em trabalho pago por peça; a manutenção pesada é partilhada horizontalmente em Mutirões Rotativos até que o `Cenário Alfa`/`Zero` automatize a tarefa.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Aquisição de Componentes Ergonômicos e de Automação:** Importar atuadores mecânicos, talhas de elevação ou bombas de lavagem necessários para cumprir um `DrudgeryAutomationQuestIntent` quando a oficina local ainda não os fabrica.<br>**B. Exercício de Licenças Médicas, Maternidade/Paternidade e Previdenciárias (`INSS`/`CLT`):** Apoiar membros híbridos (Fase 1 e 2) que ainda trabalham em empregos formais na Camada 7 a exercerem integralmente seus direitos de auxílio-doença, licença-maternidade/paternidade e afastamento remunerado. | • **Prioridade de Importação para Alívio Ergonômico:** A Camada 5 concede prioridade máxima no orçamento externo do nó para componentes que eliminem permanentemente lesões por esforço repetitivo (`LER/DORT`) ou trabalho braçal penoso, aplicando internamente padrões ergonômicos superiores à **NR-17 (Norma Regulamentadora de Ergonomia)**.<br>• **Apoio Documental de Afastamento:** O `col-adversaryd` auxilia o morador híbrido com a documentação necessária para seus direitos previdenciários sem expor dados internos da malha. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Blindagem do Mutirão Associativo, Cooperativo e Voluntário:** Impedir que a ajuda mútua e os mutirões entre vizinhos sejam confundidos com vínculo empregatício comercial clandestino.<br>**B. Sigilo Absoluto de Dados Somáticos e de Saúde (`LGPD`):** Impedir que empregadores externos ou planos de saúde acessem o histórico de descanso ou licença-cuidado de um morador. | • **Enquadramento Associativo, Cooperativo e de Voluntariado:** Mantém zero emissão de moeda/salário interno para o cuidado doméstico e ampara as atividades comunitárias no **Art. 5º, XVIII da CF/88 (Associação Civil)**, no **Ato Cooperativo (Lei 5.764/1971 e Lei 12.690/2012 — que assegura repouso remunerado e proteção social nas cooperativas de trabalho)** e na **Lei do Serviço Voluntário (Lei 9.608/1998)**.<br>• **Vazamento Zero de Dados Somáticos (`LGPD Lei 13.709/2018`):** Registros de `RestIntent` e `CareLeaveDeclarationIntent` jamais cruzam o soquete `l6_l7.sock`. |

---

## 7. Vinculação Arquitetural do Conselho Horizonte v3.2 (*A Arquitetura do Tempo Ilimitado* — D1–D8)

| Dimensão do Conselho Horizonte | Vinculação no Cenário Sigma (`SCN-SIGMA-BURNOUT` / `sigma_burnout`) |
| :--- | :--- |
| **Regime de Membros do Nó ($N_s$) & Voz Unificada $V_i(t) = 1 + R_i(t)$** | $N_s = 1 \to 6$ operadores (escalando pelas guildas); redistribuição automática de carga & piso cívico $B=1$ preservado intacto durante o descanso quando $\dot{r}_i(\tau) \to 0$. |
| **Lente Primária da Camada 6 & Modo de Captura de Necessidades** | ***A Lente da Oficina*** + ***A Lente da Despensa*** (**Libertação Zero-Stat** & UX de triagem calma com zero telemetria de ansiedade). |
| **Concessão do Hipervisor de Nó Envolvente (`cB`, `mW`, `mK`, `mm`)** | **Escudo de Silêncio do Santuário Pessoal** (teto de `3.800 cB`, zero notificações não-emergenciais quando o ferrolho de madeira está fechado). |
| **Cascata L4 de 3 Níveis & Arquétipo de Retrofit de Hardware Estágio I$\to$III** | Demonstra a **Libertação Zero-Stat (`D1`, `D7`)**: evoluir dos bloqueios cegos de Estágio I para trocas preditivas de 5 minutos nos Estágios II/III elimina o *burnout* dos fundadores. |
