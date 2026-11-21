# Cenário Pi: Mosaico Policêntrico de Guildas, Sinalizadores de Demanda & Tratados Ricardianos (`SCN-PI-GUILD`)

* **Identificador:** `SCN-PI-GUILD`
* **Onda Horizontal:** Onda E (Terceiro Lugar, Ciência Epistêmica, Cultura & Guildas Policêntricas)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 08):** [../../../../doc/podcast/pt-br/episodes/ep08_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep08_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Pi governa **como todo o mosaico policêntrico de bairros espaciais, guildas de ofício não-espaciais e enxames temporários de mutirão entre nós se auto-organiza sem um Estado centralizador e sem hierarquia corporativa**. Enquanto o `Cenário Capa` governa o aprendizado individual e as credenciais de segurança, o Cenário Pi governa a **morfogênese organizacional coletiva, a captura estigmérgica de demanda e a federação multipartes** (na tradição da governança policêntrica dos comuns de Elinor e Vincent Ostrom e das *horizontalidades* territoriais de Milton Santos):
* **O Mosaico Organizacional Tridimensional:** Entrelaçando (1) *Comunidades Espaciais* (casas contíguas, quadras e vilas de bacia hidrográfica que compartilham terra física e microgrids), (2) *Guildas de Ofício Não-Espaciais* (irmandades transversais de padeiros, torneiros, socorristas, programadores e agroflorestadores distribuídas por vários nós), e (3) *Enxames Efêmeros de Mutirão* (forças-tarefa temporárias que se reúnem para erguer um galpão *Kigumi* ou colher uma safra e **se autodissolvem automaticamente** no segundo em que o recibo de entrega é assinado).
* **Captura Estigmérgica de Demanda Preservando a Privacidade (`MeshGrowthBeacon`):** Agregando intenções locais não atendidas, gargalos de transporte pesado e importações residuais da Camada 7 em sinalizadores luminosos no mapa que indicam onde novas oficinas e capacidades precisam nascer.
* **Sinergia Superlinear Pós-Escassez & Dupla Guarda de Integridade:** Garantindo que cada novo nó somado à malha fortaleça o todo enquanto preserva sua autonomia celular—impedindo tanto o parasitismo ("sifonamento") quanto o entesouramento egoísta de nós ricos ("isolacionismo").

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

Na organização econômica e política legada (Camada 7), a coordenação produtiva entre comunidades fica presa entre dois extremos disfuncionais:

1. **Especulação Cega de Mercado vs. Rigidez do Planejamento Central (`Gosplan`):** Mercados capitalistas dependem de carestia de preços, manipulação publicitária e falências cíclicas para sinalizar demanda, enquanto o planejamento estatal centralizado sufoca a iniciativa local sob cotas burocráticas distantes da realidade do território.
2. **Monopólio Corporativo & Pontos Únicos de Falha ($k = 1$):** Cadeias produtivas voltadas ao lucro concentram a produção em megafábricas únicas ($k = 1$), de modo que um único incêndio, greve ou bloqueio rodoviário paralisa o abastecimento de uma região inteira.
3. **Burocracias Ossificadas que Jamais se Dissolvem (*A Lei de Ferro da Oligarquia*):** Tanto em empresas quanto no Estado, comissões temporárias criadas para resolver um problema específico transformam-se em feudos permanentes que inventam burocracia para justificar seu cargo.
4. **Fragmentação Isolacionista ou Caronismo Parasitário:** Redes descentralizadas sem salvaguardas constitucionais de reciprocidade ou se fragmentam em *bunkers* egoístas que recusam partilhar excedentes, ou são drenadas por nós extrativos que tomam sem retribuir.

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Pi substitui o caos de mercado e a hierarquia burocrática por cinco mecanismos de coordenação policêntrica:

1. **Mosaico Espacial $\times$ Não-Espacial $\times$ Enxame Efêmero de Mutirão (`autoDissolveOnCompletion: true`):** Todo guardião pertence simultaneamente ao seu bairro residencial físico (Comunidade Espacial $R_1 / R_2$) e a uma ou mais Guildas de Ofício transversais ($R_2 \dots R_5$). Quando surge um projeto multidisciplinar (ex.: erguer uma nova clínica do `Cenário Qui` reunindo carpinteiros do `Cenário Ksi`, torneiros do `Cenário Alfa`, eletricistas do `Cenário Beta` e cozinheiros do `Cenário Gama`), o `col-commonsd` convoca um **Enxame Efêmero de Mutirão** cuja autoridade administrativa **autodissolve-se criptograficamente (`autoDissolveOnCompletion: true`)** no exato milissegundo em que o recibo de conclusão é co-assinado.
2. **Tratados Ricardianos Multipartes em 4 Estágios (`Treaty_CID` + `FROST`):** Pactos entre guildas e entre vales são compostos no Estúdio de Tratados Ricardianos—amarrando criptograficamente (1) prosa legível por humanos em português, (2) esquemas canônicos W3C JSON-LD, e (3) máquinas de estado determinísticas BPMN da Camada 4 em um único `Treaty_CID` co-assinado por limiar (`2-de-3` / `3-de-5`) com chaves físicas `FROST`.
3. **Captura de Demanda Guiada por Intenções & Faróis Estigmérgicos (`AggregatedIntentDemandSignal` $\to$ `MeshGrowthBeacon`):** Como a malha sabe qual nova oficina, pomar ou nó irmão plantar a seguir sem bolsa de valores? Sem jamais expor a intenção privada de nenhum indivíduo, o `col-commonsd` agrega no Livro-Contábil:
   * *Cache misses* locais recorrentes que forçaram compras externas na Camada 7 (pontos quentes de dependência $D_t$),
   * Rotas do `Cenário Beta` com alto gasto energético de transporte (ex.: carregar peças fundidas ou grãos por $60\,\mathrm{km}$ toda semana), e
   * Guildas com ponto único de falha onde a redundância produtiva regional cai abaixo de $k < 3$.
   O sistema publica então um **`MeshGrowthBeacon` (Farol de Crescimento da Malha)** não-coercitivo mostrando a economia exata de exergia de transporte (ex.: `7500 bp` / $75\%$ de redução), a demanda bilateral `BCC` pré-comprometida e as bolsas de mentoria do `Cenário Capa` esperando por qualquer equipe que queira implantar aquela capacidade ali perto.
4. **Sinergia Superlinear da Malha & Canais Bilaterais de Compensação (`BCC`) de Soma Zero:** O intercâmbio entre vales liquida-se através de Canais Bilaterais de Compensação de soma zero ($B_{u \to v}(t) + B_{v \to u}(t) = 0\,\mu\mathrm{J}$) no Livro-Contábil, onde a colaboração recíproca ativa expande organicamente a margem bilateral de crédito ($+1.000\,\mathrm{bp}$), enquanto contratos `HTLC` multi-salto liquidam árvores `BOM` complexas entre vales com zero moeda global e zero juros.
5. **Dupla Guarda de Integridade (Demurrage Anti-Isolacionismo & Estrangulamento Anti-Sifão):**
   * *Anti-Isolacionismo:* Um nó rico em recursos não pode entesourar capacidade ociosa em autarquia egoísta recusando-se a parear; o excedente ocioso não compartilhado acima das reservas locais $F_{\mathrm{eco}} + B_{\mathrm{baseline}}$ sofre suave decaimento reputacional/de crédito, que é renovado ao cumprir tratados com pares ou semear novos nós do `Cenário Tau`.
   * *Anti-Sifão:* Inversamente, se um par externo drena cronicamente seu limite máximo de crédito sem reciprocar e sem passar por choque verificado do `Cenário Ípsilon`, sua janela de crédito bilateral estreita-se suavemente (sem jamais cortar o piso interno $R_0$ de sobrevivência de nenhum ser humano).

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`PolycentricMultiPartyRicardianTreaty`:** Proposta, simulação (`Oasis` `0x01`) e assinatura por limiar `FROST` de um tratado inter-guildas ou inter-nós de ajuda mútua, fornecimento de `BOM` ou guarda de bacia hidrográfica.
* **`AggregatedIntentDemandSignal` & `MeshGrowthBeacon`:** Publicação de um sinal regional de demanda preservando a privacidade que convida guildas ou novos nós de Fase 1/2 a implantar uma oficina, fundição ou clínica onde a demanda está concentrada.
* **`WorkBeeSwarmCharter` & `WorkBeeSwarmDeliveryReceipt`:** Convocação de uma força-tarefa temporária inter-guildas com extinção criptográfica automática na entrega da obra.
* **`BPVPCapabilityAdvertisement`:** Anúncio de plantas ratificadas de guilda, estoques excedentes e prazos de entrega através dos Anéis de Confiança $R_1 \dots R_5$.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Equivalência Ricardiana Tripartite (`Treaty_CID`):** Garante que nenhuma parte jamais assine um tratado cuja prosa em português diga uma coisa enquanto a máquina de estados BPMN executa outra:
  $$\mathrm{Treaty\_CID} = \mathrm{SHA256}\bigl(\mathrm{len}(\mathrm{Texto}_{\mathrm{prosa}}) \parallel \mathrm{Texto}_{\mathrm{prosa}} \parallel \mathrm{len}(\mathrm{Schema}_{\mathrm{JCS}}) \parallel \mathrm{Schema}_{\mathrm{JCS}} \parallel \mathrm{len}(\mathrm{BPMN}) \parallel \mathrm{BPMN}\bigr)$$
* **Leitura Determinística Obrigatória de 4 Estágios para Tratados & Enxames:** Antes que uma guilda ou assembleia de bairro co-assine um `Treaty_CID` via `FROST`, o Estúdio de Tratados Ricardianos apresenta uma **Leitura Determinística (*Read-Back*)** exibindo: (1) os tetos máximos de compromisso de exergia e matéria no Livro-Contábil, (2) a prova matemática de que as reservas locais $F_{\mathrm{eco}} + B_{\mathrm{baseline}}$ jamais podem ser violadas pelo tratado, (3) os resultados do teste de estresse de 10.000 ticks no `Oasis`, e (4) as condições exatas de saída unilateral e término.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Estúdio de Tratados Ricardianos de 4 Estágios** | Galpão da Guilda / Tablet da Ágora (`pi:treaty_author`, `gov:ratify`) | Editor lado a lado de prosa em português, JSON-LD e fluxo visual BPMN com pré-visualização integrada de teste de estresse no `Oasis` (`0x01`) e assinatura física por limiar `FROST`. |
| **Mapa Estigmérgico de Faróis `MeshGrowthBeacon`** | Lousa do Observatório Biorregional (`crdt:read`, `pi:beacon`) | Mapa visual da bacia hidrográfica destacando gargalos de transporte pesado, oportunidades de substituição de importações $D_t$ e lacunas de redundância $k < 3$ para orientar novas oficinas. |
| **Quadro de Enxame Efêmero de Mutirão** | Lousa de Canteiro / Colheita (`pi:swarm`, `bpmn:review`) | Coordena voluntários de múltiplas guildas, kits de ferramentas (`Alfa`) e marmitas quentes (`Gama`) para um mutirão de fim de semana, arquivando e dissolvendo visivelmente o comitê quando a última cavilha é batida. |
| **Termômetro de Reciprocidade & Folga Bilateral `BCC`** | Lousa de Guardião de Guilda (`crdt:read`) | Visão calma dos saldos nos canais bilaterais de soma zero ($B_{u \to v} + B_{v \to u} = 0$) e compromissos sazonais de contrapartida entre vales vizinhos. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Em todas as escalas, a governança da Camada 5 no Cenário Pi ancora-se na fórmula unificada de peso de voto restrita estritamente aos **Membros do Nó ($N_s$)**:
$$V_i(t) = B + R_i(t) = 1 + R_i(t) \qquad \left(V_{i,\mathrm{bp}}(t) = 10.000 + R_{i,\mathrm{bp}}(t)\right)$$
onde $B = 1$ ($10.000\,\mathrm{bp}$) é a **Base Cívica Universal** não-decaível e $R_i(t)$ é a reputação de guarda nas guildas de ofício e eletrônica no Livro-Contábil Termodinâmico ($\tau_{1/2}$). O Cenário Pi projeta, testa e publica os **Kits de Retrofit Híbridos de Direito ao Reparo (Estágio II, `D7`)** e as receitas de **Franquia de Código Aberto (`D5`)** para que qualquer artesão $N=1$ ou nó irmão importe uma constituição de guilda com zero aluguel de franquia.

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ nó individual}$):**
   * *Contexto:* Um morador ou artesão pareando pela primeira vez com 3 ou 4 casas vizinhas ($R_1$).
   * *Modelo de Governança:* **Pareamento Soberano 1-de-1 & Descoberta Instantânea de Demanda.** O guardião individual detém sua própria chave (com recuperação social 3-de-5 via `Cenário Tau`), pareia bilateralmente com os vizinhos via canais leves `BCC` no Livro-Contábil e consulta o **Mapa `MeshGrowthBeacon`** para descobrir o que a vizinhança mais precisa (ex.: percebendo que cinco casas próximas precisam de um desidratador solar compartilhado ou bancada de conserto de bicicletas).
2. **Núcleo de Guilda Atendendo um Círculo de Bairro ($N_s = 2\text{ a }5\text{ guardiões}, N_p > 1\text{ núcleo de bairro}$):**
   * *Contexto:* Um pequeno núcleo de panificação, marcenaria ou agrofloresta ($N_s = 2\text{ a }5$) atendendo de $15\text{ a }50$ moradores ($N_p > 1$).
   * *Modelo de Governança:*
     * **Co-Assinatura por Limiar `FROST` (`2-de-3` / `3-de-5`):** Compromissos externos da guilda e pedidos de atacado `GPO` exigem assinaturas múltiplas por limiar para que nenhum guardião individual comprometa acidentalmente a oficina ou a despensa do núcleo.
     * **Semeadura Automática de Redundância ($k \ge 3$):** Se o núcleo do bairro for o *único* produtor ($k = 1$) de um item vital no vale, o `col-commonsd` emite automaticamente um `MeshGrowthBeacon` e uma bolsa de mentoria no `Cenário Capa` para ajudar um segundo e terceiro núcleos a implantarem capacidade redundante—impedindo monopólios e protegendo os guardiões contra o *burnout* (`Cenário Sigma`).
3. **Federação Policêntrica Multi-Guardiões ($N_s > 1\text{ guildas sobrepostas}, N_p > 1\text{ federação biorregional}$):**
   * *Contexto:* Dezenas de bairros espaciais e guildas de ofício não-espaciais ($N_p = 500\text{ a }10.000+$ moradores através dos anéis $R_2 \dots R_5$).
   * *Modelo de Governança:*
     * **Subsidiariedade & Equilíbrio Bicameral (Espacial $\times$ Não-Espacial):** Nem o bairro espacial nem a guilda de ofício manda no outro: a **Guilda Não-Espacial** (Câmara de Guardiões) governa *como* um ofício é praticado com excelência e segurança, enquanto a **Comunidade Espacial** (Câmara dos Comuns) governa *onde* os impactos físicos, o uso do solo e os pisos ecológicos ($F_{\mathrm{eco}}$) tocam a microbacia local.
     * **Forças-Tarefa Autodissolúveis:** Projetos regionais de infraestrutura rodam por meio de **Enxames Efêmeros de Mutirão** com data e condição criptográfica de extinção para que a coordenação entre nós jamais se cristalize numa burocracia estatal central.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Poder de Barganha Atacadista Federado (`GPO`) & Substituição de Importações:** Agregar sinais de demanda entre múltiplos nós para negociar preços de fábrica/atacado em células solares, rolamentos ou grãos enquanto os projetos locais de `MeshGrowthBeacon` estão em construção.<br>**B. Intercâmbio Intercooperativo (`policy:commons_plus_l7_zero_bill`):** Firmar parcerias com cooperativas externas amigas da economia solidária (`MST`, `Unisol`, `Rede Ecovida`, cooperativas de crédito solidário) para troca de bens especializados. | • **Pipeline Da Captura de Demanda à Substituição de Importações:** Cada pedido atacadista `GPO` agregado na Camada 7 aumenta automaticamente o peso gravitacional do `MeshGrowthBeacon` correspondente, direcionando o investimento das guildas para substituir primeiro as importações de maior volume ($D_t \to 0$).<br>• **Zero Arbitragem Especulativa:** O comércio externo é estritamente limitado pelos boletos físicos do nó ($\Phi_t$) e necessidades materiais reais—proibindo especulação com commodities ou derivativos financeiros. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Compartimentalização Jurídica Multi-Entidades (Topologia Anti-Contágio) & Arbitragem Cooperativa (`Lei 9.307/1996`):** Garantir que uma ação judicial abusiva ou ataque regulatório contra uma atividade comercial jamais possa penhorar a terra residencial ou as ferramentas de um nó irmão.<br>**B. Segurança Operacional do Grafo Federado (`Camuflagem Semântica Mundana`):** Ocultar a topologia interna dos tratados policêntricos contra concorrentes corporativos ou vigilância externa. | • **Separação de Entidades por Domínio de Risco (`TTC Lei 13.465/2017` + `Associação Civil Art. 5º XVIII CF/88` + `Cooperativas Centrais e Federações Arts. 6º–8º e 79 da Lei 5.764/1971`):** Isola a terra em Termos Territoriais Coletivos livres de dívidas, a partilha interna em Associações Civis, os enxames autodissolúveis em Convênios com Condição Resolutiva Expressa (`Arts. 127–128 do Código Civil`) e os Tratados Ricardianos `FROST` sob cláusula compromissória da **`Lei de Arbitragem (Lei 9.307/1996)`** e eficácia executiva da **`MP 2.200-2/2001 Art. 10 § 2º` / `Art. 784 CPC`**.<br>• **Confidencialidade Par-a-Par dos Tratados:** Pactos `Treaty_CID` replicam-se apenas entre os anéis participantes ($R_1 \dots R_4$) e jamais cruzam o soquete `l6_l7.sock`. |

---

## 7. Vinculação Arquitetural do Conselho Horizonte v3.2 (*A Arquitetura do Tempo Ilimitado* — D1–D8)

| Dimensão do Conselho Horizonte | Vinculação no Cenário Pi (`SCN-PI-GUILD` / `pi_guild`) |
| :--- | :--- |
| **Regime de Membros do Nó ($N_s$) & Voz Unificada $V_i(t) = 1 + R_i(t)$** | $N_s = 10 \to 60$ guardiões federados de guildas de ofício e eletrônica (piso anticaptura $N_s > R_i(t) + 2$ + direito de *fork* policêntrico sem atrito). |
| **Lente Primária da Camada 6 & Modo de Captura de Necessidades** | ***A Lente da Oficina*** + ***Lente do Estúdio de Forja do Cenário Zero*** (projeto de PCBs/firmware de Kits de Retrofit Estágio II & despacho de mutirões). |
| **Concessão do Hipervisor de Nó Envolvente (`cB`, `mW`, `mK`, `mm`)** | **Envelope do Salão da Guilda & Laboratório de Precisão** (`7.200 cB`, `3.500.000 mW`, intertravamentos de ESD e particulados `ug`). |
| **Cascata L4 de 3 Níveis & Arquétipo de Retrofit de Hardware Estágio I$\to$III** | Projeta, testa e publica os **Kits de Retrofit Híbridos de Direito ao Reparo de Estágio II** (placas RISC-V abertas de sensores/atuadores + adaptadores métricos) para toda a federação (`D5`, `D7`). |
