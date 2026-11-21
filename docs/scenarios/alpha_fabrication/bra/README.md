# Cenário Alfa: Fabricação Convivial, Plantas Paramétricas, Reparo de Ciclo Completo & A Biblioteca de Ferramentas (`SCN-ALPHA-FAB`)

* **Identificador:** `SCN-ALPHA-FAB`
* **Onda Horizontal:** Onda B (Sobrevivência Periurbana, Energia, Fabricação & Logística Semente)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 05):** [../../../../doc/podcast/pt-br/episodes/ep05_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep05_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Alfa define como um bairro ou assentamento **projeta, fabrica, conserta e compartilha ferramentas físicas, componentes construtivos, eletrônicos e bens duráveis**. Em vez de tentar replicar uma fábrica monolítica verticalizada dentro de cada rua—ou de reduzir oficinas comunitárias a espaços de bricolagem recreativa—, o Cenário Alfa estabelece um **comum de manufatura biorregional federado**.

Seu escopo abrange todo o ciclo de vida físico dos bens duráveis:
* **Comum Cosmopolita de Projetos ("Projetar Global, Fabricar Local"):** Plantas globais de hardware aberto adaptadas parametricamente às madeiras manejadas brasileiras (eucalipto citriodora/cloeziana, bracatinga, algaroba, bambu-gigante *Dendrocalamus asper*), ligas recicladas e máquinas instaladas localmente.
* **Produção Multimodal:** Fabricação distribuída em marcenaria estrutural, usinagem CNC e serralheria, polímeros reciclados, eletrônica/bobinagem de motores e têxteis técnicos.
* **Atendimento Recursivo de Cadeia produtiva (`DAG BOM`):** Decomposição de montagens complexas entre células locais de trabalho, oficinas de vilas-irmãs pareadas e importações residuais estritamente rastreadas.
* **Reparo de Primeira Classe & Evolução Estigmérgica (`MTBF`):** Remanufatura diagnóstica onde a durabilidade empírica observada em campo retroalimenta diretamente a seleção das melhores plantas na bacia hidrográfica.
* **Biblioteca Compartilhada de Ferramentas & Autonomia Criativa Inviolável ($R_0$):** Circulação de ferramentas cotidianas e especializadas enquanto se garante acesso privado e **100% não-vigiado** às bancadas em horários fora de pico (`PersonalSlotIntent`).

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

Na economia industrial legada (Camada 7), o projeto, a fabricação e a manutenção dos bens físicos foram separados das comunidades que dependem deles, gerando quatro falhas estruturais:

1. **Monopólio Intelectual & Obsolescência Programada:** Fabricantes trancam geometrias CAD, esquemas elétricos, firmwares e manuais de serviço atrás de barreiras proprietárias. Eletrodomésticos, máquinas agrícolas e eletrônicos são projetados com engrenagens frágeis de plástico, carcaças coladas e travas serializadas para que um único rolamento gasto ou capacitor queimado obrigue o descarte da máquina inteira.
2. **Fragilidade de Cadeias Transoceânicas & Entropia Embutida:** Ferragens simples, peças de reposição e utensílios são fabricados a dezenas de milhares de quilômetros e enviados em embalagens plásticas descartáveis—embutindo enorme custo energético de transporte e deixando bairros inteiros desamparados quando a logística global falha.
3. **O Paradoxo das Trinta Furadeiras Paradas na Mesma Rua:** Por falta de infraestrutura confiável de compartilhamento, trinta casas na mesma rua compram separadamente trinta furadeiras baratas, serras e lavadoras de pressão que ficam **paradas pegando poeira na garagem durante mais de $9.500\,\mathrm{bp}$ ($95\%$) de sua vida útil mecânica**.
4. **Alienação Industrial vs. Impotência Amadora:** Máquinas industriais de precisão (fresadoras CNC multieixo, desempenadeiras pesadas, tornos mecânicos, bancadas de solda TIG, linhas de montagem de placas `PCB`) ficam trancadas em fábricas corporativas onde os trabalhadores não têm voz sobre o que produzem, enquanto *makerspaces* urbanos carecem de cadeia de suprimentos e coordenação para produção comunitária séria.

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Alfa substitui essas quatro falhas por cinco modalidades complementares de oficina e quatro laços de ciclo de vida:

* **Cinco Modalidades de Oficina:**
  1. *Marcenaria & Encaixes Estruturais:* Serraria, serras de gabinete, plainas, routers CNC de 3 eixos e estufas de curvatura a vapor trabalhando com eucalipto manejado, bracatinga, algaroba e bambu-gigante tratado (`Cenário Eta`) para produzir pórticos estruturais, treliças da horta, mobiliário, caixas de abelhas nativas sem ferrão (meliponicultura) e cabos ergonômicos.
  2. *Usinagem CNC & Serralheria:* Centros de usinagem de 3 a 5 eixos, tornos mecânicos, mesas de corte a plasma/laser, dobradeiras e bancadas de solda TIG produzindo engrenagens, eixos, mancais, quadros de triciclos de carga (`Cenário Beta`), o lavador rotativo de mandioca do `Cenário Sigma` e rotores de microturbinas hidráulicas.
  3. *Conformação Aditiva de Polímeros Reciclados & Elastômeros:* Extrusoras alimentadas por polímeros granulados no `Cenário Mi` produzindo gaxetas de vedação em TPU, válvulas, caixas elétricas IP67 e engrenagens de reposição para eletrodomésticos.
  4. *Eletrônica, Bobinagem de Motores & Mecatrônica:* Fresadoras de placas `PCB`, bancadas de refusão, bobinadeiras e soldadores de ponto para baterias produzindo placas de sensores/atuadores da Camada 2, controladores solares `MPPT`, estatores de motores *brushless* e transceptores de malha `LoRa`.
  5. *Têxteis Técnicos, Cordoaria & Biocompósitos:* Máquinas de costura pesada de transporte triplo, teares e prensas de fibras naturais/micélio (em ponte com o `Cenário Ni`) produzindo lonas impermeáveis para triciclos, cintos de elevação e painéis acústicos.
* **Quatro Laços Centrais de Ciclo de Vida:**
  * *Fork Paramétrico Global-para-Local:* Adaptação de plantas abertas globais para madeiras manejadas locais (`Cenário Eta`), ligas refundidas (`Cenário Teta`) ou polímeros recuperados (`Cenário Mi`).
  * *Encadeamento Recursivo de Árvores de Materiais (`DAG BOM`):* Divisão automática de montagens complexas entre bancadas locais e oficinas de nós vizinhos (transportadas pelas rotas *milk-run* do `Cenário Beta`).
  * *Triagem Diagnóstica & Remanufatura:* Leitura da etiqueta QR/NFC de linhagem de um equipamento para fabricar apenas a subpeça desgastada, ou encaminhamento da sucata final ao `Cenário Mi` / `Cenário Teta`.
  * *Biblioteca Escalonada de Ferramentas & Horários Livres de Criação ($R_0$):* Circulação de ferramentas portáteis pelos anéis de confiança ($R_0 \dots R_4$) e abertura das bancadas para criação pessoal livre nos horários fora de pico.
  * *Franquia de Código Aberto nas 3 Bibliotecas Soberanas & Cascata BPMN de 3 Níveis (`D7`, `D8`):* Armazena plantas testadas de oficinas e ferramentas no **Grafo Ontológico**, no **Registro de Orquestração** e na **Linha de Base Física** (*Instanciação Turnkey*, *Fork Semântico Localizado*, *Ranqueamento Empírico de Linhagem* via `collective:thermodynamicProof` e *Montagem Greenfield*). Quando uma planta encontra maquinário local fora do padrão, a Camada 4 executa a **Cascata de Adaptação de Hardware BPMN em 3 Níveis**: (Nível 1) *Escalonamento de Parâmetros SI* $\to$ (Nível 2) *Substituição de Macro-Atividade* (ex.: substituir uma sub-rotina de solda a laser por flange parafusado `M6`) $\to$ (Nível 3) *Parada Dura de Restrição Física Real* (descompilada para um **Diff Semântico** na Camada 6).
  * *Progressão da Frota de Hardware em 3 Estágios (Do Resgate da Camada 7 à Libertação Zero-Stat, `D8`):* Inicia no **Estágio I: Frota Resgatada da Camada 7** (superando as 4 penalidades do "imposto do shim": *Shims Parasitas na Camada 2*, *Paradas Duras Cegas no Meio do Lote*, *Escalada de Tributo em Reais à Camada 7* e *Penalidades de ERC na Camada 3*), avança pelo **Estágio II: Frota Híbrida Direito-ao-Reparo** (substituindo placas DRM resinadas por controladoras abertas ESP32/RISC-V da Camada 2 e adaptadores métricos impressos em 3D como **evento de Prova de Guarda de primeira classe no Livro-Contábil [$\Delta V / \Delta R$]**), e amadurece no **Estágio III: Frota Coletiva Nativa de Especificação Aberta** (transmitindo desgaste harmônico preditivo de rolamentos/RPM para trocas preventivas de 5 minutos que conquistam a **Libertação Zero-Stat**).

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
Moradores e guardiões de ofício interagem com o comum de fabricação por meio de seis intenções JSON-LD canônicas:
* **`BlueprintSpecializationIntent`:** *Fork* de uma planta aberta mestre (`parentBlueprintCid`) para adequá-la às dimensões da madeira/metal disponíveis localmente ou às fresas instaladas, preservando as interfaces padrão de encaixe.
* **`ManufacturingOrderIntent`:** Solicitação de fabricação de uma peça ou conjunto—decompondo sua Lista Recursiva de Materiais (`BOM`) em tarefas locais de oficina, subordens para nós vizinhos ou importações residuais sinalizadas.
* **`ToolCheckoutIntent` & `MaintenanceIntent`:** Empréstimo de ferramentas manuais/elétricas da Biblioteca de Ferramentas e agendamento automático de afiação, calibração ou troca de rolamentos quando limiares de desgaste são atingidos.
* **`PersonalSlotIntent`:** Reserva de uma bancada fora do horário de pico para marcenaria pessoal, luthieria (`Cenário Lambda`), presentes ou invenções com **zero telemetria registrada sobre qual objeto pessoal foi fabricado**.
* **`RepairDiagnosticIntent` & `SalvageIntent`:** Isolamento de um subcomponente defeituoso na árvore explodida de montagem para fabricação pontual da peça de reposição, ou encaminhamento do material em fim de vida ao `Cenário Mi` / `Cenário Teta`.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
O `col-commonsd` representa as capacidades e compromissos da oficina no grafo de conhecimento W3C JSON-LD 1.1 / SHACL (`https://w3id.org/collective/v1/`):
* **Pacotes de Plantas em Dupla Camada (`BlueprintSpecification` + `fBOM`):** Vinculam a geometria 3D paramétrica (STEP / OpenSCAD / 3MF) e árvores explodidas de reparo a uma **Lista Funcional de Materiais (`fBOM`)** definida por propriedades físicas de desempenho (ex.: tensão de cisalhamento, grau alimentício inox `316L`, resistência térmica) em vez de códigos proprietários de fornecedor.
* **Leitura Determinística Obrigatória do Vetor de Custo Total de Produção ($\mathbf{C}_{\mathrm{prod}}$):** Antes que um guardião assine qualquer `ManufacturingOrderIntent` ou `BlueprintSpecializationIntent` com sua chave física, sua lente exibe uma **Leitura Determinística (*Read-Back*)** em linguagem clara da pegada física completa no Livro-Contábil:
  $$\mathbf{C}_{\mathrm{prod}} = \bigl\langle E_{\mathrm{fuso}\,\mu\mathrm{J}},\; M_{\mathrm{estoque}\,\mathrm{mg}},\; \lambda_{\mathrm{ERC}\,\mu\mathrm{J}},\; \tau_{\mathrm{maquina}\,\mathrm{ms}} \bigr\rangle$$
  informando a exergia do fuso (`uJ`), a massa de matéria-prima (`mg`) de resgate local versus importação residual, o Custo de Reposição Ecológica Assimptótico ($\lambda_{\mathrm{ERC}}$), o tempo de preparação e a conformidade acústica na divisa do lote ($\le 550\,\mathrm{cB}$ dia / $\le 380\,\mathrm{cB}$ noite).
* **Linhagem Empírica de Durabilidade (`observed_mtbf_sec`):** Reparos concluídos atualizam o tempo médio entre falhas observado em campo de cada variante de planta, fazendo emergir automaticamente os projetos mais duráveis na bacia hidrográfica.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela
A Camada 6 projeta a superfície da oficina por meio de lentes robustas que condensam centenas de fluxos de trabalho em turnos calmos sincronizados com o pico solar:

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| ***A Oficina (The Workshop)* — Lente de Ciclos Duráveis, Custódia & Retrofit** | Desktop / Tablet / Quiosque (`workshop:steward`, `tool:unlock`) | Contraparte termodinâmica d'*A Despensa*: ignora validade de calendário para pregos, ferramentas ou fresadoras CNC, rastreando desgaste discreto por ciclo de uso (`cumulativeUseMs`, `wearIndexBp`), concessões de custódia e `HardwareFleetTier` (`Estágio I Resgatado` $\to$ `Estágio II Retrofit Híbrido` $\to$ `Estágio III Especificação Aberta`); prepara trocas preventivas de rolamentos `M8` em 5 minutos. |
| **Navegador Paramétrico de Plantas & Vetor de Custo** | Desktop / Tablet (`intent:author`, `crdt:read`) | Navega pelas 3 Bibliotecas Soberanas (*Grafo Ontológico*, *Registro de Orquestração*, *Linha de Base Física*), traduz medidas legadas (`polegadas` $\to$ `um`/`mm`), exibe a leitura prévia de $\mathbf{C}_{\mathrm{prod}}$ e agrupa cortes pesados de CNC no excedente solar da tarde (`SoC_bp`). |
| **Terminal E-Ink de Chão de Fábrica Selado contra Serragem** | Fixado ao lado de Tornos / Serras / CNC (`tool:unlock`, `bpmn:review`) | Lousa operável com luvas ligada aos contatores físicos **Lockout/Tagout (`LOTO`, `NR-12`)** e leitores NFC. O toque do cartão verifica a credencial cega `BBS+` do `Cenário Capa` em $< 50\,\mathrm{ms}$; se a guarda da lâmina abrir, a Camada 2 corta a força e crava o freio em $< 10\,\mathrm{ms}$ em hardware puro. |
| **Ordem de Viajante em Papel com QR Code** | Células de Trabalho Sem Tela | Ficha impressa com códigos QR que acompanha a peça de madeira ou metal do corte bruto à montagem final; os guardiões bipam a conclusão de etapa com leitor manual sem tocar em telas. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

A governança do Cenário Alfa calcula o peso de voto estritamente sobre os **Membros do Nó ($N_s$ guardiões ativos da oficina)** via $V_i(t) = B + R_i(t) = 1 + R_i(t)$ ($V_{i,\mathrm{bp}}(t) = 10.000 + R_{i,\mathrm{bp}}(t)$ no Livro-Contábil), enquanto atende o círculo maior ($N_p$) por concessões de custódia n'*A Oficina* e captura de demanda sem vigilância:

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ pessoa atendida}$):**
   * *Contexto:* Um morador ou artífice operando sua própria bancada de garagem, impressora 3D e bateria solar (frequentemente co-localizada dentro de um Nó Envolvente Residencial ao lado da Cozinha Familiar).
   * *Modelo de Governança:* **Zero reuniões, zero votações, zero burocracia ($V_1(t) = 1 + R_1(t) = 100\%$).** O artífice individual pode importar modelos externos de segurança da Camada 5 e plantas *turnkey* das Bibliotecas Soberanas enquanto a governança atua como companheiro automatizado de segurança física e respeito ao Nó Envolvente: intertravamentos `LOTO` em hardware, agendamento automático de cortes pesados no pico solar (`SoC_bp`), conformidade com a concessão acústica ($\le 550\,\mathrm{cB}$ dia / $\le 380\,\mathrm{cB}$ noite) e escudos de descanso do `Cenário Sigma`.
2. **Guardião Único / Núcleo Pequeno Atendendo um Círculo Maior ($N_s = 1\text{ a }3\text{ artífices}, N_p > 1\text{ pessoas atendidas}$):**
   * *Contexto:* Um marceneiro/mecânico habilidoso (ou micro-núcleo de 2 a 3 pessoas) cuidando de uma oficina de garagem bem equipada e Biblioteca de Ferramentas que atende um núcleo de 10 casas vizinhas.
   * *Modelo de Governança:*
     * **Pilar II de Capacidades de Trabalho & Blindagem Automatizada (`BBS+`):** Sob os **Quatro Pilares Constitucionais da Camada 5**, quando qualquer um dos $N_p$ vizinhos quer emprestar uma serra circular ou reservar um `PersonalSlotIntent` na fresadora CNC, o guardião nunca precisa fazer papel de "fiscal chato"—o terminal d'*A Oficina* só destrava o contator mediante a verificação em $< 50\,\mathrm{ms}$ de uma credencial cega `BBS+` do `Cenário Capa` (`T1` com coleira dupla do mentor ou `T2+` autônomo).
     * **Fila *Pull-Only* & Proteção Anti-Martírio:** Pedidos de fabricação e conserto vindos do círculo $N_p$ entram numa fila coletiva assíncrona *pull-only*. Se a carga concentrar-se numa única pessoa ($> 4.000\,\mathrm{bp}$ / $40\%$), o `Cenário Sigma` trava novas admissões solitárias e abre bolsas no `Cenário Capa` (onde aprendizes com $R_i(0) = 0$ conquistam sua reputação inicial realizando **Retrofits Híbridos Direito-ao-Reparo de Estágio II**).
     * **Privacidade Criativa Inviolável em $R_0$:** Qualquer pessoa habilitada que reserve um `PersonalSlotIntent` fora de pico desfruta de privacidade criativa total: o Livro-Contábil registra apenas a exergia do fuso (`uJ`) e o desgaste discreto da ferramenta n'*A Oficina*, com **zero telemetria** sobre qual objeto pessoal foi criado.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Guilda de Fabricação de bairro ou assentamento ($N_s = 5\text{ a }40+$ guardiões de ofício) operando baias multimodais e cumprindo tratados recursivos de `BOM` através de uma bacia hidrográfica federada ($N_p = 150\text{ a }2.500+$ moradores).
   * *Modelo de Governança:*
     * **Envelopes Bicamerais de Ofício vs. Ética ($N_s > R_i(t) + 2$):** Dentro da guilda, $V_i(t) = 1 + R_i(t)$ combina a reputação de ofício $R_i(t)$ com o piso cívico não-decrescente $B = 1$ ($10.000\,\mathrm{bp}$) que impede o sequestro por mantenedor único quando um catálogo maduro de plantas repousa em **Equilíbrio Dormente**. Simultaneamente, a **Câmara dos Comuns** ($N_p$ moradores, $1\text{ pessoa} = 1\text{ voto}$) detém um **Veto Ético Incondicional de Projeto**—bloqueando armas, vigilância oculta ou obsolescência programada—e define os pisos ecológicos e acústicos ($F_{\mathrm{eco}}$).
     * **Recusa Conscienciosa & Evolução Estigmérgica (`collective:thermodynamicProof`):** Reparos concluídos e menor $\lambda_{\mathrm{ERC}}$ por ciclo atualizam os rankings `collective:thermodynamicProof` nas Bibliotecas Soberanas, conduzindo à convergência voluntária para padrões modulares abertos (parafusos métricos `M6`/`M8`, rolamentos ISO, caixas Euro-tote).

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Aquisição de Componentes-Folha Irredutíveis (`ProcurementIntent`):** Quando uma árvore `BOM` recursiva chega a um componente-folha que nenhum nó da malha fabrica ainda (ex.: chips MOSFET de carbeto de silício, pastilhas de metal duro, fusos de esferas retificados).<br>**B. Fabricação Sob Encomenda de Excedente com Teto (`policy:commons_plus_l7_zero_bill`):** Vender peças usinadas em CNC ou marcenaria sob medida para compradores externos a fim de quitar IPTU, contas de energia ou leasing de máquinas. | • **Registro no Índice de Dependência Legada ($D_t$):** Toda peça-folha importada é registrada automaticamente no Livro-Contábil, elevando importações frequentes a prioridades de substituição de importações para a fundição do `Cenário Teta` e o `Cenário Zero`.<br>• **Desligamento Automático em $\Phi_t = \text{R\$ } 0,00$:** A produção comercial externa só é permitida acima dos pisos ecológicos e de sobrevivência locais ($F_{\mathrm{eco}} + B_{\mathrm{baseline}}$) e **desliga-se automaticamente** no instante em que os boletos mensais da Camada 7 e as metas do fundo de IPTU/ITR atingem $\text{R\$ } 0{,}00$. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Blindagem Associativa, Cooperativa & Segurança de Máquinas (`NR-10` / `NR-12`):** Proteger bibliotecas comunitárias de ferramentas e oficinas de reparo contra assédio regulatório comercial ou bloqueios de propriedade intelectual de fabricantes.<br>**B. Segurança Operacional da Cadeia de Suprimentos (`Camuflagem Semântica Mundana`):** Impedir que fornecedores externos infiram capacidades internas do nó a partir de pedidos de peças. | • **Chassi Tripartite Brasileiro & Conformidade NR-12:** Ampara o empréstimo gratuito de ferramentas entre vizinhos no **Comodato Gratuito (Arts. 579 a 585 do Código Civil)** e na **Associação Civil (Art. 5º, XVIII CF/88)**, as oficinas produtivas no **Ato Cooperativo (Lei 5.764/1971 Art. 79 e Lei 12.690/2012)** com travas físicas que cumprem e superam as normas **`NR-10` e `NR-12`**, e o solo do galpão no **Termo Territorial Coletivo (`TTC — Lei 13.465/2017`)**.<br>• **Filtragem de Camuflagem:** O `col-commonsd` remove todas as identidades `did:peer`, árvores de plantas e contexto de uso antes de cruzar o `l6_l7.sock`, emitindo apenas pedidos atacadistas agregados e mundanos. |

---

## 7. Vinculação Arquitetural do Conselho Horizonte v3.2 (*A Arquitetura do Tempo Ilimitado* — D1–D8)

| Dimensão Arquitetural | Vinculação Canônica v3.2 para `SCN-ALPHA-FAB` (`alpha_fabrication`) |
| :--- | :--- |
| **1. Regime de Membros do Nó ($N_s$) & Fórmula Unificada de Voto $V_i(t) = 1 + R_i(t)$ (`CCI-2`)** | $N_s = 1 \to 6$ torneiros/marceneiros ativos por baia ($V_{i,\mathrm{bp}}(t) = 10.000 + R_{i,\mathrm{bp}}(t)$ no Livro-Contábil). Combina o piso cívico não-decrescente $B = 1$ ($10.000\,\mathrm{bp}$) com as escadas de dupla chave mentor-aprendiz do **Pilar II (`collective:WorkCapabilityMatrix`, `Cenário Capa`)**, permitindo que recém-chegados com $R_i(0) = 0$ conquistem peso inicial de guarda executando **Retrofits Híbridos Direito-ao-Reparo de Estágio II** (`ProofOfStewardshipRetrofitRecordV50`). |
| **2. Lente Primária da Camada 6 & Localização SI na Borda (`CCI-1`, `CCI-3`)** | ***A Oficina (The Workshop Lens)*** (desgaste mecânico discreto por ciclo de uso, horas de fuso, intervalos de calibração e concessões de custódia—explicitamente contrastada com as curvas perecíveis de Arrhenius d'*A Despensa*). Traduz medidas legadas de ofício (`1,5 polegadas` $\to 38.100\,\mu\mathrm{m}$ / `38,1 mm`) na borda humana para inteiros SI canônicos de 64 bits antes da Leitura Determinística, garantindo `0` telemetria sobre criações pessoais em `PersonalSlotIntent` ($R_0$). |
| **3. Concessão do Hipervisor Espacial do Nó Envolvente (`CCI-4`)** | Opera dentro do **Envelope da Baia Industrial / Oficina de Garagem** sob concessões físicas cegas ao domínio do Hipervisor do Nó Envolvente: teto acústico $\le 8.500\,\mathrm{cB}$ ($85{,}0\,\mathrm{dBA}$) diurno / $\le 5.000\,\mathrm{cB}$ ($50{,}0\,\mathrm{dBA}$) vespertino (ou $\le 3.800\,\mathrm{cB}$ no toque de recolher residencial noturno) e concessão elétrica de $4.500.000\,\mathrm{mW}$ ($4{,}5\,\mathrm{kW}$ de pico para fuso/solda). |
| **4. Franquia de Código Aberto, Cascata L4 & Progressão da Frota Estágio I$\to$III (`CCI-5`, `CCI-6`)** | Instancia plantas *turnkey* de oficina das 3 Bibliotecas Soberanas; resolve diferenças de maquinário local via **Nível 1: Escalonamento de Parâmetros SI** (divisão de passes/avanço) e **Nível 2: Substituição de Macro-Atividade** (substituindo solda a laser por flange parafusado `M6`), travando em **Nível 3: Parada Dura de Restrição Física Real** (descompilada em Diff Semântico na Camada 6). Elimina as 4 penalidades do **Estágio I: Máquinas/CNCs Resgatados da Camada 7 com DRM** fabricando **Kits de Retrofit Híbrido Estágio II (RISC-V GRBL/LinuxCNC aberto)** e **Máquinas Nativas Estágio III de Especificação Aberta** que transmitem desgaste harmônico contínuo de rolamentos para trocas preventivas em 5 minutos (**Libertação Zero-Stat**). |
