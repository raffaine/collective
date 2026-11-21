# Cenário Delta: Santuário Perinatal, Casulos Pós-Parto de 90 Dias & Cirandas Infantis Cooperativas (`SCN-DELTA-CHILD`)

* **Identificador:** `SCN-DELTA-CHILD`
* **Onda Horizontal:** Onda C (Cuidado, Parentesco, Aprendizagem & Moradia Desmercantilizada)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 06):** [../../../../doc/podcast/pt-br/episodes/ep06_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep06_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Delta governa **como uma comunidade dá as boas-vindas a uma nova vida, ampara mães e pais durante o quarto trimestre (pós-parto) e nutre bebês e crianças pequenas**. Ele substitui a exaustão solitária da família nuclear isolada e a mercantilização das creches comerciais por um **Santuário Perinatal & Comum de Cirandas Infantis Cooperativas** caloroso e multigeracional.

Seu escopo abrange:
* **Apoio Pré-Natal, Obstetrícia & Santuário de Parto:** Coordenação de parteiras, enfermagem obstétrica (`Cenário Qui`), doulas e suítes calmas de parto.
* **O Casulo Pós-Parto de 90 Dias (`PostpartumCocoonIntent`):** Envolvendo novos pais biológicos, adotivos ou acolhedores em três meses inteiros de alimentação nutritiva, higienização hospitalar de fraldas de pano e proteção de descanso silenciosamente orquestrados a partir de uma única assinatura pré-parto.
* **Cirandas Cooperativas Sem Câmeras (*Cooperative Play Pods*):** Berçários de bairro e jardins de brincar na floresta governados por salvaguarda física de dois adultos verificada em hardware ($N_{\mathrm{adults}} \ge 2$), tetos estritos ponderados por faixa etária e segurança puramente termodinâmica/ambiental (zero câmeras ópticas ou microfones).
* **Cuidado Não-Tokenizável (`domestic_care_token_minting == 0`):** Tratando a criação das crianças como uma alegria sagrada e compartilhada do comum, jamais uma transação de mercado ou dívida no Livro-Contábil.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

Na sociedade urbana e periurbana legada (Camada 7), trazer uma criança ao mundo ou criar filhos pequenos é punido por quatro falhas sistêmicas:

1. **Isolamento Brutal no "Quarto Trimestre" & Exaustão Parental:** Dois dias após o parto, os pais voltam para um apartamento isolado onde precisam simultaneamente cicatrizar do parto, amamentar de duas em duas horas na madrugada, cozinhar, lavar pilhas de roupa e retornar rapidamente ao trabalho assalariado—disparando epidemias de depressão pós-parto e esgotamento.
2. **Asfixia Financeira das Creches vs. Pobreza das Educadoras:** As famílias comprometem de $30\%$ a $50\%$ da renda em berçários particulares caríssimos ou enfrentam longas filas de espera, enquanto as próprias educadoras infantis recebem salários precarizados e são sobrecarregadas com proporções inseguras de crianças por adulto.
3. **"Berçários Inteligentes" de Vigilância:** Babás eletrônicas Wi-Fi e aplicativos comerciais de creche transmitem vídeo HD e áudio de bebês dormindo para servidores em nuvem de corporações estrangeiras, normalizando a vigilância na primeira infância e expondo dados biométricos íntimos.
4. **Montanhas de Fraldas Petroquímicas & Dependência de Fórmulas:** Milhões de fraldas plásticas descartáveis vão parar em aterros todos os dias enquanto faltam às famílias bancos comunitários seguros de leite humano pasteurizado e cozinhas de papinhas orgânicas livres de alérgenos.

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Delta substitui o isolamento parental e a creche mercantilizada por cinco laços integrados de cuidado:

1. **O Casulo Pós-Parto de 90 Dias com Uma Só Assinatura (`PostpartumCocoonIntent`):** Assinar uma única intenção antes do parto ou adoção orquestra silenciosamente **90 dias ($7.776.000\,\mathrm{s}$) de fluxos inter-cenários em segundo plano**, sem exigir nenhuma tarefa diária de agendamento dos pais privados de sono:
   * Entrega diária silenciosa na caixa térmica da varanda de marmitas de inox do `Cenário Gama` com caldos ricos em colágeno, ensopados pró-lactação e pães de fermentação natural.
   * Coleta duas vezes por semana pelo triciclo elétrico do `Cenário Beta` e higienização térmica de grau hospitalar ($\ge 344.150\,\mathrm{mK}$ / $71{,}0^\circ\mathrm{C}$ + ozônio aquoso) das fraldas reutilizáveis de algodão orgânico e enxovais do `Cenário Ni`.
   * Ativação automática no `Cenário Sigma` da **Licença-Cuidado com Congelamento Duplo** para todos os adultos da casa, silenciando chamados das guildas e congelando o decaimento de credenciais técnicas enquanto preserva $100\%$ do piso $B_{\mathrm{baseline}} = 10.000\,\mathrm{bp}$.
2. **Salvaguarda NFC de Dois Adultos ($N_{\mathrm{adults}} \ge 2$) & Tetos Ponderados por Idade:** As cirandas infantis exigem no mínimo $N_{\mathrm{adults}} \ge 2$ adultos guardiões credenciados via `BBS+` (frequentemente pareando pais/educadores com avós contadores de histórias do `Cenário Fi`) e impõem em hardware um teto estrito ponderado por idade ($\sum w_i \le 40.000 \cdot N_{\mathrm{adults}}$, onde bebês de colo $0\text{–}12\text{ m} = 20.000\,\mathrm{bp}$ [máx. 2/adulto], crianças de $1\text{–}3\text{ anos} = 10.000\,\mathrm{bp}$ [máx. 4/adulto] e pré-escolares de $3\text{–}5\text{ anos} = 5.000\,\mathrm{bp}$ [máx. 8/adulto]).
3. **Segurança Termodinâmica de Berçário Sem Câmeras (`opticalAndAudioSensorsPermitted == false`):** Proíbe câmeras ópticas e microfones dentro dos berçários, garantindo a segurança por meio da física da Camada 1 e Camada 2: válvulas misturadoras termostáticas mecanicamente travadas em $\le 311.150\,\mathrm{mK}$ ($38{,}0^\circ\mathrm{C}$) contra escaldamento, ventilação de ar fresco mantendo $\mathrm{CO}_2 < 800\,\mathrm{ppm}$ e calma acústica $\le 450\,\mathrm{cB}$ ($45\,\mathrm{dBA}$).
4. **Banco Comunitário de Leite Humano Pasteurizado por Holder ($62{,}5^\circ\mathrm{C}$) & Despensa de Primeiras Papinhas:** Doações de leite materno passam pela pasteurização Holder verificada ($335.650\,\mathrm{mK}$ / $62{,}5^\circ\mathrm{C}$ por $30\,\mathrm{min}$, padrão `rBLH-BR / Fiocruz`) e resfriamento rápido antes da dispensação ao lado das papinhas do `Cenário Gama` protegidas por travas `BBS+` contra alérgenos.
5. **Exploração Montessori & Ciranda na Floresta (Ponte `Cenário Eta` / `Iota`):** As crianças pequenas transitam naturalmente das salas aconchegantes para hortas pedagógicas, cozinhas de barro e rodas de descoberta da natureza que semeiam a curiosidade científica para o `Cenário Iota` e `Cenário Capa`.

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`PostpartumCocoonIntent` (ou `PrenatalAndBirthSanctuaryIntent`):** Pré-registro de uma janela esperada de parto ou adoção para encadear automaticamente 90 dias de entrega de marmitas (`Gama`), ciclo de fraldas de pano (`Beta`/`Ni`) e Licença-Cuidado (`Sigma`).
* **`CarePodSessionIntent`:** Abertura ou check-in de uma criança numa sessão da ciranda cooperativa do bairro, verificando a copresença de dois adultos e a folga de capacidade ponderada por idade.
* **`HumanMilkAndInfantNutritionAttestation`:** Registro dos patamares térmicos de pasteurização Holder e compatibilidade zero-knowledge contra alérgenos na alimentação infantil.
* **`EmergencyCareFlare`:** Alerta de prioridade Classe 1 Zero-LLM acionado por cordão físico que convoca um adulto suplente imediato da quadra ou um socorrista pediátrico do `Cenário Qui` em $< 500\,\mathrm{ms}$.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Expansão Silenciosa de DAG por 90 Dias (`col-commonsd` $\to$ `col-execd`):** Demonstra a forma mais compassiva de atenuação de variedade da Camada 6: um único documento JSON-LD assinado antes do parto compila-se em dezenas de fluxos recorrentes de cozinha, triciclo, lavanderia hospitalar e escudo de descanso que rodam por três meses sem pedir aos pais que toquem num único botão.
* **Invariantes Pediátricos Impostos em Hardware:** As sessões da ciranda não entram em `SESSION_ACTIVE` a menos que $N_{\mathrm{adults}} \ge 2$ toques NFC de hardware estejam ativos, a carga infantil ponderada cumpra $\le 40.000 \cdot N_{\mathrm{adults}}$, a água das torneiras esteja $\le 311.150\,\mathrm{mK}$ ($38{,}0^\circ\mathrm{C}$) e `opticalAndAudioSensorsPermitted == false`.
* **Leitura Determinística Obrigatória de Casulo & Custódia:** Antes que os pais assinem um `PostpartumCocoonIntent` ou autorizem adultos de confiança para buscar a criança (parentesco escolhido do `Cenário Ômicron`), a lousa apresenta uma **Leitura Determinística (*Read-Back*)** em linguagem clara confirmando os DIDs autorizados para retirada, as máscaras de alergia e zero compartilhamento externo de dados.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Lousa E-Ink Sem Câmera do Vestíbulo Perinatal & NFC de 2 Adultos** | Entrada da Ciranda Infantil (`care:pod_steward`) | Dois adultos credenciados encostam seus cartões para abrir a ciranda; os pais encostam a pulseira/ficha passiva da criança na chegada e saída; exibe $\mathrm{CO}_2$, temperatura segura da água ($\le 38^\circ\mathrm{C}$) e proporção adulto/criança com zero câmeras. |
| **Planejador do Casulo Pós-Parto de Uma Só Assinatura** | Tablet da Família Gestante / Lousa (`intent:author`) | Configuração calma de 1 página antes do parto: escolha o número de marmitas, restrições alimentares e tamanho das fraldas uma única vez e feche a tela por 90 dias. |
| **Cordão Tátil de Apoio Pediátrico / Socorrista** | Parede da Ciranda (`chi:responder`, `care:pod_steward`) | Cordão sem tela e acionador de 2 botões: o Botão 1 chama um adulto suplente da vizinhança (ex.: quando um bebê precisa de colo exclusivo); o Botão 2 despacha um socorrista pediátrico do `Cenário Qui` em $< 500\,\mathrm{ms}$. |
| **Etiqueta NFC do Cesto de Fraldas de Pano & Enxoval** | Cesto Vedado na Varanda (`courier:dtn`) | Sinaliza automaticamente os cestos de fraldas de algodão para a próxima coleta do triciclo de carga do `Cenário Beta` e lavagem térmica hospitalar a $71^\circ\mathrm{C}$ + ozônio. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Em todas as escalas, a governança da Camada 5 no Cenário Delta ancora-se na fórmula unificada de peso de voto restrita estritamente aos **Membros do Nó ($N_s$)**:
$$V_i(t) = B + R_i(t) = 1 + R_i(t) \qquad \left(V_{i,\mathrm{bp}}(t) = 10.000 + R_{i,\mathrm{bp}}(t)\right)$$
onde $B = 1$ ($10.000\,\mathrm{bp}$) é a **Base Cívica Universal** não-decaível detida por cada mãe, pai e educador guardião da ciranda ($N_s > R_i(t) + 2$), enquanto a ala infantil detém proteção de Via Rápida com prioridade `950` de silêncio (`4.000 cB` durante a soneca) e conforto térmico (`294.150 mK`) no **Hipervisor de Nó Envolvente** do edifício.

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ família monoparental}, N_p = 1\text{ unidade familiar}$):**
   * *Contexto:* Uma mãe solo ou pai solo criando um bebê ou criança pequena em sua casa.
   * *Modelo de Governança:* **Abraço comunitário incondicional (`Nenhuma Mãe ou Pai Solo Fica Isolado`).** Mesmo quando uma casa tem apenas $N_s = 1$ adulto, ativar um `PostpartumCocoonIntent` ou `EmergencyCareFlare` mobiliza imediatamente as marmitas da cozinha solidária, o ciclo de fraldas e os cuidadores de alívio dos anéis $R_1 / R_2$, garantindo que uma família monoparental jamais enfrente febre, pós-parto ou privação de sono sozinha.
2. **Núcleo de Cuidado Atendendo um Círculo de Bairro ($N_s = 2\text{ guardiões da ciranda}, N_p > 1\text{ famílias vizinhas}$):**
   * *Contexto:* Dois pais, educadores ou avós guardiões em rodízio acolhendo uma ciranda infantil na sala ou quintal arborizado para $4\text{ a }8$ crianças da rua ($N_p > 1$).
   * *Modelo de Governança:*
     * **Copresença Obrigatória de Dois Adultos ($N_s \ge 2$):** Diferente da oficina ou da horta onde $N_s = 1$ pode operar sozinho, o Cenário Delta exige constitucionalmente **no mínimo dois adultos credenciados presentes** em qualquer sessão compartilhada de cuidado infantil—protegendo tanto as crianças quanto os próprios cuidadores contra sobrecarga.
     * **Teto Rígido de Proporção Ponderada:** Se chegar mais uma criança que elevaria a carga ponderada acima de $40.000\,\mathrm{bp}$ por adulto, a lousa do vestíbulo segura gentilmente o check-in e chama na hora o terceiro adulto da escala de sobreaviso da quadra.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Guilda Perinatal e de Primeira Infância do assentamento ($N_s = 15\text{ a }60+$ educadores infantis, parteiras, enfermeiras obstétricas e pais/avós em rodízio) operando suítes de parto, berçários, banco de leite humano e bosques-escola através dos anéis $R_1 \dots R_3$.
   * *Modelo de Governança:*
     * **Encontro Intergeracional com os Anciãos do `Cenário Fi`:** Conecta avós contadores de histórias, músicos (`Cenário Lambda`) e agroflorestadores (`Cenário Eta`) às cirandas da primeira infância sob salvaguarda não-coercitiva de dois adultos.
     * **Zero Tokenização do Cuidado (`domestic_care_token_minting == 0`):** O acesso às cirandas infantis e aos Casulos Pós-Parto de 90 dias é um direito universal de cada criança e família—jamais condicionado a saldo no Livro-Contábil ou horas de trabalho.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Insumos Pediátricos e Obstétricos Especializados:** Adquirir frascos de vidro borossilicato para pasteurização de leite materno, cadeirinhas veiculares homologadas (`INMETRO`) ou insumos neonatais ainda não fabricados localmente.<br>**B. Exercício de Licença-Maternidade/Paternidade & Salário-Maternidade (`INSS`/`CLT`):** Apoiar mães e pais híbridos (Fases 1 e 2) a receberem integralmente seu salário-maternidade e benefícios legais na Camada 7. | • **Prioridade Máxima à Segurança Pediátrica:** A Camada 5 concede prioridade máxima no orçamento externo do nó para equipamentos certificados de segurança infantil, enquanto registra a demanda de enxovais e mobiliário infantil para produção local no `Cenário Ni` e `Cenário Alfa`.<br>• **Retenção Integral pela Família:** Qualquer salário-maternidade ou benefício infantil recebido na Camada 7 permanece $100\%$ com a família em $R_0$. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Padrão-Ouro de Banco de Leite (`rBLH-BR / Fiocruz / RDC ANVISA 171/2006`) & Cirandas Parentais Associativas:** Proteger as cirandas cooperativas de vizinhos contra exigências mercantis desenhadas para redes comerciais de creches e garantir excelência sanitária no leite materno doado.<br>**B. Sigilo Absoluto de Dados Infantis (`ECA`, `LGPD` e `Camuflagem Semântica Mundana`):** Impedir que corretores de dados ou sistemas externos coletem registros de desenvolvimento infantil ou listas de presença. | • **Carapaça Associativa Parental (`Art. 5º, XVII–XVIII CF/88`), Marco Legal da Primeira Infância (`Lei 13.257/2016`), `ECA (Lei 8.069/1990)` & `RDC ANVISA 171/2006`:** Estrutura as cirandas da quadra como grupos associativos recíprocos de cuidado parental sem fins lucrativos (`0` mensalidade comercial) e alinha a pasteurização Holder ($62{,}5^\circ\mathrm{C}$ / 30 min) às normas da **Rede Brasileira de Bancos de Leite Humano (`rBLH-BR / Fiocruz / SUS`)**.<br>• **Bloqueio Total de Telemetria Infantil (`Art. 14 da LGPD — Lei 13.709/2018`):** Nomes, datas de nascimento e listas de presença de crianças são estritamente bloqueados de cruzar o soquete `l6_l7.sock`. |

---

## 7. Vinculação Arquitetural do Conselho Horizonte v3.2 (*A Arquitetura do Tempo Ilimitado* — D1–D8)

| Dimensão do Conselho Horizonte | Vinculação no Cenário Delta (`SCN-DELTA-CHILDCARE` / `delta_childcare`) |
| :--- | :--- |
| **Regime de Membros do Nó ($N_s$) & Voz Unificada $V_i(t) = 1 + R_i(t)$** | $N_s = 4 \to 18$ educadores guardiões e famílias ($B=1$ garante cogovernança entre famílias e educadores sob $N_s > R_i(t) + 2$ com `domesticCareTokenMinting: 0`). |
| **Lente Primária da Camada 6 & Modo de Captura de Necessidades** | ***A Lente da Despensa*** (TTL de Arrhenius para nutrição/leite) + ***A Lente do Hipervisor de Nó Envolvente*** (escudo acústico na hora da soneca). |
| **Concessão do Hipervisor de Nó Envolvente (`cB`, `mW`, `mK`, `mm`)** | **Envelope da Ala Infantil / Ciranda** (escudo estrito de silêncio de `4.000 cB` nas janelas de soneca, piso térmico de `294.150 mK`). |
| **Cascata L4 de 3 Níveis & Arquétipo de Retrofit de Hardware Estágio I$\to$III** | Detém proteção acústica e térmica de Via Rápida com prioridade `950` dentro do envelope predial, modulando automaticamente concessões de oficinas ruidosas adjacentes. |
