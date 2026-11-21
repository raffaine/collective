# Cenário Capa: Aprendizagem Polímata Sem Dívida, Círculos de Cultura & Mestria Não-Rastreável (`SCN-KAPPA-APPRENTICE`)

* **Identificador:** `SCN-KAPPA-APPRENTICE`
* **Onda Horizontal:** Onda C (Cuidado, Parentesco, Aprendizagem & Moradia Desmercantilizada)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 06):** [../../../../doc/podcast/pt-br/episodes/ep06_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep06_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Capa governa **como crianças, jovens e adultos aprendem artes e ofícios práticos, adquirem competências operacionais seguras, mentoram uns aos outros e conquistam credenciais verificáveis (`T1` a `T4`) em todas as guildas d'O Coletivo**. Unindo as **Redes de Convivialidade de Ivan Illich** aos **Círculos de Cultura de Paulo Freire** e ao rigor da segurança física de bancada (`NR-12`), enquanto o `Cenário Iota` cultiva a ciência cidadã aberta e a Biblioteca de Alexandria, o Cenário Capa transforma o conhecimento em **prontidão psicomotora corporificada e guarda física segura**.

Seu escopo abrange:
* **Aprendizagem Polímata de Guilda Sem Dívida & Infância Livre de Telas:** Permitindo que crianças cresçam com zero telas antes dos 7 anos (explorando madeira, barro, música e agrofloresta) e que qualquer jovem ou adulto domine marcenaria, usinagem CNC (`Alfa`), triagem clínica (`Qui`), metalurgia solar (`Teta`), agrofloresta sintrópica (`Eta`) ou engenharia de software (`Zero`) com zero mensalidade, zero dívida estudantil e $100\%$ do piso de sobrevivência $B_{\mathrm{baseline}}$.
* **Rastreamento de Mestria 100% Privado em $R_0$ (`BKT` em Inteiros):** Utilizando Rastreamento Bayesiano de Conhecimento local em inteiros para que todos os erros, tentativas e ritmo de aprendizado permaneçam $100\%$ privados no dispositivo do aprendiz.
* **Prática Supervisionada com Coleira `BLE` de $2{,}0\,\mathrm{m}$ & Destravamento Zero-Knowledge `BBS+` de Máquinas:** Combinando coleiras criptografadas de proximidade do mentor durante o treino prático com credenciais cegas `BBS+` de $< 50\,\mathrm{ms}$ que destravam máquinas perigosas sem vigiar quem usou qual ferramenta.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

No mercado educacional e de certificação legado (Camada 7), a formação humana é deformada por quatro patologias estruturais:

1. **Servidão por Dívida Estudantil & Inflação de Diplomas:** Faculdades privadas e cursos comerciais cobram mensalidades pesadas ou prendem jovens em décadas de dívida bancária antes mesmo que possam tocar num torno mecânico, numa bancada clínica ou num laboratório.
2. **Provas Vigiadas por Webcam & Ansiedade Punitiva:** O ensino padronizado classifica alunos por rankings e provas de múltipla escolha vigiadas por softwares invasivos que punem o erro inicial, geram ansiedade crônica e são incapazes de aferir se a pessoa sabe operar uma serra circular ou esterilizar uma autoclave com segurança real.
3. **Hiperespecialização Vitalícia em "Caixinhas Profissionais":** Trabalhadores são forçados aos 18 anos a entrar numa única gaveta ocupacional pelo resto da vida (*"ou você é programador ou é marceneiro, nunca os dois"*), sufocando a polimatia humana e tornando as comunidades frágeis quando falta um especialista.
4. **Reserva Corporativa de Mercado & Nepotismo:** Conselhos e corporações frequentemente restringem o acesso ao aprendizado para manter escassez artificial ou favorecer parentes em vez de acolher novos aprendizes.

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Capa substitui a dívida estudantil e as provas punitivas por cinco laços conviviais de aprendizagem:

1. **Progressão de Competência Somática em Quatro Graus (`T1` $\to$ `T4`):**
   * *`T1_Supervised_Novice` (Novato Supervisionado):* Concluiu o ensaio interativo de riscos no simulador **Oasis** (`Camada 1b`, proveniência `0x01`); habilitado a operar equipamentos reais em modo `ARMED_SUPERVISED` dentro do raio de $\le 2.000\,\mathrm{mm}$ ($2{,}0\,\mathrm{m}$) da coleira criptografada `BLE` de um mentor.
   * *`T2_Autonomous_Practitioner` (Praticante Autônomo):* Atingiu $\ge 9.500\,\mathrm{bp}$ ($95\%$) de domínio no seu caderno privado de Rastreamento Bayesiano (`BKT`) e passou na validação prática de bancada; habilitado para operação autônoma (`ZeroKnowledgeToolUnlockIntent`).
   * *`T3_Guild_Steward` (Guardião de Guilda):* Qualificado para calibrar, consertar e assinar liberações críticas de lote (autoclaves do `Cenário Gama`, ensaios `HPLC` do `Cenário Qui`, vazamentos de fundição do `Cenário Teta`).
   * *`T4_Master_Mentor` (Mestre Mentor — e `T4_Emeritus_Sage` no `Cenário Fi`):* Qualificado para supervisionar novatos via coleira `BLE`, elaborar módulos `GuildPreparednessSyllabus` e integrar bancas `3-de-3` de certificação.
2. **Rastreamento Bayesiano de Conhecimento (`BKT`) em Inteiros 100% Privado em $R_0$:** Enquanto o aprendiz pratica nas simulações de risco do `Oasis` e nas bancadas mentoradas, seu caderno pessoal atualiza a probabilidade de domínio $P(L_t)$ em pontos-base inteiros de 64 bits (`0..10000 bp`). **Todos os erros, tentativas falhas e curvas de aprendizado ficam estritamente guardados na memória privada $R_0$ do aprendiz**—a guilda só vê a prova criptográfica final de prontidão quando $P(L_t) \ge 9.500\,\mathrm{bp}$.
3. **Banca `3-de-3` de Certificação à Prova de Nepotismo (`SociologicalCapstoneAttestation`):** Conceder uma credencial `T2+` exige a co-assinatura física simultânea de uma banca de 3 examinadores (`T3/T4`) cujos membros compartilham **zero vínculo domiciliar ou pacto de parentesco do `Cenário Ômicron`** com o candidato ou entre si.
4. **Intertravamentos de Máquinas em Zero-Knowledge via `BBS+` Não-Rastreável:** Uma vez certificado, o cartão físico do guardião apresenta uma prova cega `BBS+` em qualquer serra (`Alfa`), forno de indução (`Teta`) ou armário clínico (`Qui`), destravando o contator `LOTO` em $< 50\,\mathrm{ms}$ sem registrar a identidade pessoal do operador num banco de dados central de vigilância.
5. **Decaimento Natural de Recência & Exercícios Leves de Atualização (com Congelamento na Licença-Cuidado do `Cenário Sigma`):** Credenciais de alto risco (`T2/T3` em quadros de alta tensão, motosserras ou trauma) incluem um relógio natural de recência; se um guardião passar 6 meses sem praticar uma manobra rara de emergência, sua lousa agenda um exercício calmo de 15 minutos no `Oasis` ou na bancada (congelado automaticamente durante qualquer Licença-Cuidado do `Cenário Sigma`).

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`ApprenticeshipEnrollmentIntent`:** Vinculação de um aprendiz a um currículo prático `GuildPreparednessSyllabus` e pareamento com mentores `T4` disponíveis (guiado pela calma sensorial do `Cenário Tau`).
* **`SociologicalCapstoneAttestation`:** A co-assinatura 3-de-3 da banca independente de parentesco certificando que o aprendiz demonstrou segurança física, precisão de ofício e cuidado ecológico em $\ge 9.500\,\mathrm{bp}$.
* **`ZeroKnowledgeToolUnlockIntent`:** Apresentação de uma prova de capacidade `BBS+` não-rastreável no intertravamento da Camada 2 para energizar uma máquina em $< 50\,\mathrm{ms}$.
* **`MentorshipFellowshipOffer` (em par com `Cenário Sigma` / `Pi`):** Bolsas prioritárias de aprendizado publicadas automaticamente no Livro-Contábil quando uma guilda atinge o alerta Anti-Martírio de concentração ($> 4.000\,\mathrm{bp}$ / $40\%$) ou emite um `MeshGrowthBeacon`.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Grafos Declarativos `GuildPreparednessSyllabus`:** Vinculam fundamentos teóricos do `Cenário Iota`, simulações interativas de modos de falha no `Oasis` (`0x01`, ex.: prevenção de coice de serra, neutralização de derramamento químico) e horas práticas de bancada mentorada em uma árvore de saberes autodescritiva.
* **Leitura Determinística Obrigatória de Banca & Segurança:** Antes que a Banca 3-de-3 co-assine um `SociologicalCapstoneAttestation`, suas lousas exibem uma **Leitura Determinística (*Read-Back*)** confirmando: (1) domínio local `BKT` $\ge 9.500\,\mathrm{bp}$, (2) zero sobreposição familiar no `Cenário Ômicron` entre examinadores e candidato, e (3) as classes exatas de máquinas e janelas de recência destravadas pela credencial.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Caderno de Aprendizagem Privado em $R_0$ (`BKT`)** | Lousa Pessoal do Aprendiz (`kappa:learner`, `r0:local_only`) | Acompanha o progresso pessoal `BKT` em marcenaria, culinária, código ou botânica; zero notas públicas, zero rankings e zero telemetria de erros. |
| **Lente de Simulação de Risco & Ensaio no Oasis** | Estação / Tablet (`kappa:drill`) | Permite que novatos vivenciem e superem com segurança travamentos simulados de torno, sobrepressão de autoclave ou *black-start* de microgrid (`0x01`) antes de tocar no hardware real. |
| **Crachá de Coleira `BLE` Mentor-Aprendiz ($\le 2{,}0\,\mathrm{m}$)** | Crachá de Bolso / Bancada Sem Tela (`tool:unlock`) | Sinalizador físico sem tela (`IEEE 802.15.4z / BLE`): mantém a máquina do novato `T1` em modo `ARMED_SUPERVISED` enquanto o mentor estiver a $\le 2.000\,\mathrm{mm}$; pausa suavemente o fuso se o mentor se afastar. |
| **Lousa de Co-Assinatura da Banca `3-de-3`** | Bancada da Guilda (`gov:ratify`, `kappa:examiner`) | Exibe a lista de verificação de tolerâncias físicas da peça de teste do candidato e recebe os toques NFC simultâneos dos três examinadores independentes. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Em todas as escalas, a governança da Camada 5 no Cenário Capa ancora-se na fórmula unificada de peso de voto restrita estritamente aos **Membros do Nó ($N_s$)**:
$$V_i(t) = B + R_i(t) = 1 + R_i(t) \qquad \left(V_{i,\mathrm{bp}}(t) = 10.000 + R_{i,\mathrm{bp}}(t)\right)$$
O Cenário Capa é a vitrine canônica do **Pilar Constitucional II (`collective:WorkCapabilityMatrix`, `D3`, `D7`)**: um aprendiz ingressa no Dia 0 com $R_{\mathrm{aprendiz}}(0) = 0$ no Livro-Contábil Termodinâmico, mas já detém igualdade cívica plena $V_{\mathrm{aprendiz}}(0) = B + 0 = 1$ ($10.000\,\mathrm{bp}$), construindo $R_i(t) > 0$ por meio de tarefas iniciais não-supervisionadas (Tier-0) e tarefas de chave dupla co-assinadas por um mentor (Tier-1, incluindo a montagem de **Kits de Retrofit Híbridos de Direito ao Reparo — Estágio II**).

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ aprendiz/operador}$):**
   * *Contexto:* Um morador solitário aprendendo um novo ofício (ex.: cabeamento de bateria solar, conserva em autoclave ou encaixes de madeira) usando o currículo offline do nó e o simulador `Oasis`.
   * *Modelo de Governança:* **Ensaio de segurança no próprio ritmo & validação em visita.** O guardião solitário usa o Caderno Privado em $R_0$ e os ensaios de risco no `Oasis` para construir domínio no seu tempo; para máquinas de alto risco, pode concluir sua validação `3-de-3` durante a passagem de artesãos itinerantes (`Cenário Tau` Caminho 3) ou num mutirão de fim de semana no nó vizinho.
2. **Mentor Único / Núcleo Pequeno Atendendo um Círculo Maior ($N_s = 1\text{ ou pequeno núcleo}, N_p > 1\text{ aprendizes/vizinhos}$):**
   * *Contexto:* Um mestre marceneiro, cozinheiro ou torneiro mentorando de $3\text{ a }10$ aprendizes do bairro e cuidando do acesso à oficina compartilhada ($N_p > 1$).
   * *Modelo de Governança:*
     * **Mentoria Multiplicada Sem Sobrecarga:** Quando o aprendiz atinge o grau `T1` nos simuladores offline, a coleira `BLE` de $\le 2.000\,\mathrm{mm}$ permite ao mestre supervisionar com segurança 2 ou 3 novatos em bancadas adjacentes; quando atingem o grau `T2`, o intertravamento cego `BBS+` cuida automaticamente de todos os usos futuros sem incomodar o mestre.
     * **Reconhecimento do Mentor & Incentivo Anti-Monopólio do Saber:** Na Camada 5, o teto de Riqueza Reputacional ($\mathcal{R}$) de um mestre no Livro-Contábil cresce **especificamente quando ele forma novos pares `T2/T3` até a autonomia**—recompensando estruturalmente a partilha do saber e tornando o monopólio do conhecimento contraproducente.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ mentores}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Guildas policêntricas e Círculos de Cultura em um assentamento e bacia hidrográfica ($N_s = 20\text{ a }200+$ mentores nos 25 cenários).
   * *Modelo de Governança:*
     * **Invariante Anti-Nepotismo da Banca `3-de-3`:** Impõe bancas independentes de parentesco entre diferentes casas para que as credenciais sejam confiáveis em toda a federação $R_1 \dots R_5$.
     * **Mobilidade Polímata entre Guildas:** Como a sobrevivência é incondicionalmente garantida ($B_{\mathrm{baseline}} = 10.000\,\mathrm{bp}$) e o ensino é 100% gratuito, uma mesma pessoa pode passar as manhãs assando pão no `Cenário Gama`, as tardes aprendendo luthieria no `Cenário Lambda`/`Alfa` e o inverno estudando fitoquímica no `Cenário Eta`/`Qui`.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Portabilidade Externa de Certificação Profissional e Técnica (`LDB Art. 41` / `SENAI` / `Rede Federal`):** Traduzir horas práticas de bancada e atestados `SociologicalCapstoneAttestation` internos em certificações reconhecidas pelo Estado para eletricistas, mecânicos, socorristas (`Cenário Qui`) ou créditos técnicos/profissionais.<br>**B. Aquisição de Manuais Técnicos & Padrões Metrológicos:** Importar livros de referência ou blocos-padrão rastreáveis ao `INMETRO` ainda não presentes na malha. | • **Exportação Unidirecional de Certificados por Competência:** Quando um membro deseja uma certificação externa da Camada 7 (ex.: para assinar laudos no `Cenário Ksi`), o `col-adversaryd` emite a documentação de horas práticas e avaliação qualitativa sob o amparo do **`Art. 41 da LDB (Lei 9.394/1996 — reconhecimento e certificação de saberes adquiridos na prática e no trabalho)`**.<br>• **Ingestão Imediata na Biblioteca de Alexandria (`Cenário Iota`):** Qualquer manual técnico importado é digitalizado para o acervo comum para que nenhum outro nó precise comprá-lo novamente. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Liberdade Pedagógica por Ciclos (`LDB Arts. 23 e 24`) & Demonstração de Segurança (`NR-12`):** Amparar a organização pedagógica sem provas padronizadas na legislação educacional brasileira e comprovar a inspetores que as oficinas cumprem segurança física superior às fábricas comerciais.<br>**B. Sigilo Absoluto do Aprendiz (`LGPD` e `Camuflagem Semântica Mundana`):** Proteger as curvas de aprendizado e identidades dos aprendizes contra recrutadores corporativos ou bancos de dados externos. | • **Amparo na `LDB (Lei 9.394/1996, Arts. 23 e 24, V, 'a')` & Auditoria Zero-Knowledge `NR-12`:** Organiza a educação comunitária em ciclos de aprendizagem com avaliação contínua e qualitativa (livre de notas classificatórias punitivas, na tradição de **Paulo Freire**) e comprova conformidade com a **`NR-10` e `NR-12`** via intertravamentos `< 10 ms LOTO` e `BBS+` sem revelar quais indivíduos operaram quais máquinas.<br>• **Educação Associativa Livre (`Art. 5º, XVIII e Art. 206, II–III CF/88`):** Nenhum dado de `BKT` privado em $R_0$ cruza jamais o soquete `l6_l7.sock`. |

---

## 7. Vinculação Arquitetural do Conselho Horizonte v3.2 (*A Arquitetura do Tempo Ilimitado* — D1–D8)

| Dimensão do Conselho Horizonte | Vinculação no Cenário Capa (`SCN-KAPPA-APPRENTICESHIP` / `kappa_apprenticeship`) |
| :--- | :--- |
| **Regime de Membros do Nó ($N_s$) & Voz Unificada $V_i(t) = 1 + R_i(t)$** | $N_s = 2 \to 20$ mestres e aprendizes ($R_{\mathrm{mestre}} > 0$ pareado com $R_{\mathrm{aprendiz}}(0) = 0$, piso cívico $B=1$ garantido desde o Dia 0). |
| **Lente Primária da Camada 6 & Modo de Captura de Necessidades** | ***A Lente da Oficina*** (tarefas iniciais não-supervisionadas Tier-0 & tarefas de chave dupla co-assinadas pelo mentor Tier-1 do Pilar II). |
| **Concessão do Hipervisor de Nó Envolvente (`cB`, `mW`, `mK`, `mm`)** | **Envelope da Baia de Treinamento** (`7.500 cB`, concessão de potência com intertravamento `mW`, coleira BLE de mentor $\le 2.000\,\mathrm{mm}$). |
| **Cascata L4 de 3 Níveis & Arquétipo de Retrofit de Hardware Estágio I$\to$III** | Vitrine canônica do **Pilar II (`collective:WorkCapabilityMatrix`)**: resolve o paradoxo de partida a frio $R_i=0$ via tarefas de **Retrofit de Direito ao Reparo (Estágio II)** co-assinadas por mentor. |
