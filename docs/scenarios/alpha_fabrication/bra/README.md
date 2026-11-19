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
| **Navegador Paramétrico de Plantas & Vetor de Custo** | Desktop / Tablet (`intent:author`, `crdt:read`) | Árvores explodidas de reparo, controles deslizantes de espessura de estoque e leitura prévia de $\mathbf{C}_{\mathrm{prod}}$; agrupa cortes pesados de CNC no excedente solar da tarde (`SoC_bp`). |
| **Terminal E-Ink de Chão de Fábrica Selado contra Serragem** | Fixado ao lado de Tornos / Serras / CNC (`tool:unlock`, `bpmn:review`) | Lousa operável com luvas ligada aos contatores físicos **Lockout/Tagout (`LOTO`, `NR-12`)** e leitores NFC. O toque do cartão verifica a credencial cega `BBS+` do `Cenário Capa` em $< 50\,\mathrm{ms}$; se a guarda da lâmina abrir, a Camada 2 corta a força e crava o freio em $< 10\,\mathrm{ms}$ em hardware puro. |
| **Quiosque NFC de Retirada Rápida da Biblioteca de Ferramentas** | Vestíbulo da Garagem / Galpão (`tool:unlock`) | Trava NFC sem tela ou com mini E-Ink: encoste o cartão para retirar furadeira, câmera térmica ou prensa; um botão tátil de 1 a 5 na devolução sinaliza *"precisa afiar/revisar"* em um segundo. |
| **Ordem de Viajante em Papel com QR Code** | Células de Trabalho Sem Tela | Ficha impressa com códigos QR que acompanha a peça de madeira ou metal do corte bruto à montagem final; os guardiões bipam a conclusão de etapa com leitor manual sem tocar em telas. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ pessoa atendida}$):**
   * *Contexto:* Um morador ou artífice operando sua própria bancada de garagem, impressora 3D e bateria solar.
   * *Modelo de Governança:* **Zero reuniões, zero votações, zero burocracia.** A governança atua estritamente como um companheiro automatizado de segurança física e ritmo pessoal: intertravamentos `LOTO` em hardware, agendamento automático de cortes pesados no pico solar (`SoC_bp`), proteção acústica noturna ($\le 550\,\mathrm{cB}$ dia / $\le 380\,\mathrm{cB}$ noite) e escudos de descanso do `Cenário Sigma`.
2. **Guardião Único / Núcleo Pequeno Atendendo um Círculo Maior ($N_s = 1\text{ ou pequeno núcleo}, N_p > 1\text{ pessoas atendidas}$):**
   * *Contexto:* Um marceneiro/mecânico habilidoso (ou micro-núcleo de 2 a 3 pessoas) cuidando de uma oficina de garagem bem equipada e Biblioteca de Ferramentas que atende um núcleo de 10 casas vizinhas.
   * *Modelo de Governança:*
     * **Blindagem Automatizada de Competência (`BBS+`):** Quando qualquer um dos $N_p$ vizinhos quer emprestar uma serra circular ou reservar um `PersonalSlotIntent` na fresadora CNC, o guardião da oficina nunca precisa fazer papel de "fiscal chato"—o terminal da máquina só destrava o contator mediante a verificação em $< 50\,\mathrm{ms}$ de uma credencial criptográfica não-rastreável `BBS+` de segurança do `Cenário Capa` (`L1+`) para aquela classe de máquina.
     * **Fila *Pull-Only* & Proteção Anti-Martírio:** Pedidos de fabricação e conserto vindos do círculo $N_p$ entram numa fila coletiva assíncrona *pull-only* em vez de tocar no celular do guardião. Se a carga de oficina concentrar-se numa única pessoa ($> 4.000\,\mathrm{bp}$ / $40\%$), o `Cenário Sigma` trava novas admissões solitárias e abre bolsas de aprendizado no `Cenário Capa`.
     * **Privacidade Criativa Inviolável em $R_0$:** Qualquer pessoa habilitada que reserve um `PersonalSlotIntent` fora do horário de pico desfruta de privacidade criativa total: o Livro-Contábil registra apenas a exergia do fuso (`uJ`) e o desgaste do filtro/fresa, com **zero telemetria** sobre qual objeto pessoal foi criado.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Guilda de Fabricação de bairro ou assentamento ($N_s = 5\text{ a }40+$ guardiões de ofício) operando baias multimodais e cumprindo tratados recursivos de `BOM` através de uma bacia hidrográfica federada ($N_p = 150\text{ a }2.500+$ moradores).
   * *Modelo de Governança:*
     * **Envelopes Bicamerais de Ofício vs. Ética:** A **Câmara de Guardiões** ($N_s$ artífices ativos, ponderados pela reputação de ofício $\mathcal{R}$ com teto) ratifica tolerâncias de usinagem, intervalos de manutenção e o catálogo `Ratified_Blueprint_Set`. Simultaneamente, a **Câmara dos Comuns** ($N_p$ moradores, $1\text{ pessoa} = 1\text{ voto}$) detém um **Veto Ético Incondicional de Projeto**—bloqueando componentes de armas, hardware de vigilância oculta ou produtos com obsolescência programada—e define os pisos ecológicos e acústicos ($F_{\mathrm{eco}}$).
     * **Recusa Conscienciosa de Bancada:** Nem o nó nem qualquer artífice individual é uma prestadora passiva de serviços; ambos mantêm direito absoluto de recusa conscienciosa sobre qualquer fabricação não catalogada ou eticamente reprovável.
     * **Evolução Estigmérgica de Plantas & Sinalizadores de Demanda (`MeshGrowthBeacon`):** Entre nós pareados, o `col-kmsd` eleva a preferência de roteamento (`local_pref_bp`) para variantes de plantas com maior durabilidade comprovada em campo (`observed_mtbf_sec`), enquanto gargalos recorrentes de peças geram sinais `MeshGrowthBeacon` no `Cenário Pi` convidando vilas-irmãs a instalar máquinas complementares.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Aquisição de Componentes-Folha Irredutíveis (`ProcurementIntent`):** Quando uma árvore `BOM` recursiva chega a um componente-folha que nenhum nó da malha fabrica ainda (ex.: chips MOSFET de carbeto de silício, pastilhas de metal duro, fusos de esferas retificados).<br>**B. Fabricação Sob Encomenda de Excedente com Teto (`policy:commons_plus_l7_zero_bill`):** Vender peças usinadas em CNC ou marcenaria sob medida para compradores externos a fim de quitar IPTU, contas de energia ou leasing de máquinas. | • **Registro no Índice de Dependência Legada ($D_t$):** Toda peça-folha importada é registrada automaticamente no Livro-Contábil, elevando importações frequentes a prioridades de substituição de importações para a fundição do `Cenário Teta` e o `Cenário Zero`.<br>• **Desligamento Automático em $\Phi_t = \text{R\$ } 0,00$:** A produção comercial externa só é permitida acima dos pisos ecológicos e de sobrevivência locais ($F_{\mathrm{eco}} + B_{\mathrm{baseline}}$) e **desliga-se automaticamente** no instante em que os boletos mensais da Camada 7 e as metas do fundo de IPTU/ITR atingem $\text{R\$ } 0{,}00$. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Blindagem Associativa, Cooperativa & Segurança de Máquinas (`NR-10` / `NR-12`):** Proteger bibliotecas comunitárias de ferramentas e oficinas de reparo contra assédio regulatório comercial ou bloqueios de propriedade intelectual de fabricantes.<br>**B. Segurança Operacional da Cadeia de Suprimentos (`Camuflagem Semântica Mundana`):** Impedir que fornecedores externos infiram capacidades internas do nó a partir de pedidos de peças. | • **Chassi Tripartite Brasileiro & Conformidade NR-12:** Ampara o empréstimo gratuito de ferramentas entre vizinhos no **Comodato Gratuito (Arts. 579 a 585 do Código Civil)** e na **Associação Civil (Art. 5º, XVIII CF/88)**, as oficinas produtivas no **Ato Cooperativo (Lei 5.764/1971 Art. 79 e Lei 12.690/2012)** com travas físicas que cumprem e superam as normas **`NR-10` e `NR-12`**, e o solo do galpão no **Termo Territorial Coletivo (`TTC — Lei 13.465/2017`)**.<br>• **Filtragem de Camuflagem:** O `col-commonsd` remove todas as identidades `did:peer`, árvores de plantas e contexto de uso antes de cruzar o `l6_l7.sock`, emitindo apenas pedidos atacadistas agregados e mundanos. |
