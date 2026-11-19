# Cenário Beta: Logística das Cinco Artérias, Rotas *Milk-Run* de Cadeia Fria & Veto Autônomo `No-Transit` (`SCN-BETA-TRANSIT`)

* **Identificador:** `SCN-BETA-TRANSIT`
* **Onda Horizontal:** Onda B (Sobrevivência Periurbana, Energia, Fabricação & Logística Semente)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 05):** [../../../../doc/podcast/pt-br/episodes/ep05_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep05_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Beta governa as **cinco artérias físicas** que conectam casas, oficinas, cozinhas solidárias e assentamentos-irmãos em um organismo metabólico vivo:
1. **Pessoas:** Mobilidade humana acessível por meio de calçadas sombreadas, ciclovias, bicicletas e triciclos elétricos, microfrotas elétricas compartilhadas e vans/barcos elétricos entre vales.
2. **Cargas & Retorno de Materiais (`BOM Backhaul`):** Rotas consolidadas (*milk-runs*) de triciclos elétricos de carga, reboques térmicos de inox para marmitas e cadeia fria, e corredores hidroviários/ferroviários na bacia.
3. **Exergia Elétrica:** Barramentos domésticos de $48\,\mathrm{V}$ em corrente contínua (CC) e interligações de microgrid de bairro em $380\,\mathrm{V}$ HVDC com ilhamento mecânico instantâneo por alavanca vermelha.
4. **Água em Cascata & Laços Térmicos Hidrônicos:** Cisternas de água de chuva, captação de nascentes, filtragem de águas cinzas por 72h em jardins filtrantes de bananeiras e papiros, e tubulações hidrônicas PEX-a de recuperação de calor.
5. **Dados & Pacotes Tolerantes a Atraso (`DTN` / *Sneakernet*):** Fibra escura comunitária, rádio em malha `LoRa 915 MHz` (homologado ANATEL) com protocolo Reticulum, e drives sólidos NVMe de 4 Terabytes transportados fisicamente no guidão dos triciclos de carga (*Sneakernet*).

Em vez de pavimentar bairros para carros particulares de duas toneladas ou depender de monopólios frágeis de concessionárias, o Cenário Beta coordena uma circulação de alta eficiência em escala humana, garantindo a cada comunidade o poder de traçar fronteiras invioláveis de silêncio e proteção (**polígonos `No-Transit`**) e preservar no mínimo $80\%$ da vazão natural dos rios.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

Na infraestrutura urbana e periurbana legada (Camada 7), a circulação de pessoas, cargas, energia, água e dados sofre com quatro patologias estruturais:

1. **Dependência do Automóvel & Entropia das Entregas Ponto-a-Ponto:** Famílias são forçadas a endividar-se comprando carros caros apenas para trabalhar ou fazer compras, enquanto dezenas de motos e vans de aplicativos sobem a mesma rua meia-vazias o dia inteiro, enchendo o bairro de barulho, fumaça e microplásticos de pneus.
2. **Redes Centralizadas Frágeis de Energia e Água:** Redes elétricas aéreas envelhecidas e adutoras centralizadas sofrem apagões em cascata a cada tempestade, não oferecem amortecimento local às famílias e tratam cada litro de água potável do mesmo jeito—seja para beber ou para dar descarga.
3. **Opacidade da Cadeia Fria & Desperdício de Perecíveis:** Alimentos e medicamentos enviados por transportadoras comerciais sofrem excursões térmicas não registradas, estragando no caminho ou exigindo pilhas de isopor e gelo químico descartável.
4. **Monopólio de Provedores de Internet & Roteamento Dependente de Nuvem:** A comunicação entre vizinhos da mesma rua depende de torres celulares comerciais e provedores que caem durante temporais e submetem todo o tráfego local à inspeção corporativa.

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Beta substitui a infraestrutura fóssil fragmentada por cinco laços unificados de circulação:

1. **Rotas *Milk-Run* Consolidadas de Clarke-Wright ($\eta_{\mathrm{route}} \ge 6.000\,\mathrm{bp}$ / $\ge 60\%$):** Em vez de despachar viagens avulsas para entregar um item por vez, o `col-commonsd` resolve um Problema Determinístico de Roteamento de Veículos Capacitados (`CVRP`) em inteiros exigindo um fator mínimo de ocupação de $60\%$—agrupando as marmitas quentes do `Cenário Gama`, peças usinadas do `Cenário Alfa`, fraldas de pano higienizadas do `Cenário Delta` e o retorno de composto do `Cenário Mi` em uma única ronda calma de triciclo elétrico pela manhã e à tarde.
2. **Cadeia Fria Verificada em Hardware & Passagem de Custódia em Duas Fases (`HTLC`):** As caixas térmicas de inox nos triciclos contam com células de carga calibradas ($\pm 5.000\,\mathrm{mg}$) e registradores térmicos PT100 (`ATECC608A`) que comprovam refrigeração ininterrupta entre $[275.150,\, 277.150]\,\mathrm{mK}$ ($2{,}0\text{–}4{,}0^\circ\mathrm{C}$) ou conservação quente $\ge 333.150\,\mathrm{mK}$ ($60{,}0^\circ\mathrm{C}$) antes de liberar a pré-imagem criptográfica de liquidação `HTLC` no Livro-Contábil mediante duplo toque NFC.
3. **Rede Tolerante a Atrasos (*Sneakernet* `DTN`) Acoplada aos Triciclos:** Todo triciclo elétrico de carga e barco/van da bacia carrega no suporte do guidão um cache sólido criptografado `DTN` (NVMe de 4 TB)—sincronizando automaticamente atualizações de software (`Cenário Zero`), espelhos da Biblioteca de Alexandria (`Cenário Iota`), mapas florestais (`Cenário Eta`) e grafos CRDT Automerge entre vales sem sinal de rádio com largura de banda de múltiplos gigabytes.
4. **Hierarquia Ecológica de Cinco Níveis para Água e Microgrid ($\ge 8.000\,\mathrm{bp}$ / $80\%$ do Rio para os Peixes):** Impõe em hardware um piso constitucional inviolável de $\ge 8.000\,\mathrm{bp}$ ($80\%$) da vazão natural do rio ou nascente para a piracema, os peixes e a mata ciliar antes que qualquer válvula agrícola ou de oficina possa abrir, e organiza o uso da água em cascata: água potável $\to$ águas cinzas de banho/lavanderia $\to$ jardim filtrante de 72h com bananeiras e papiros $\to$ irrigação de pomar e recuperação de estruvita/biocarvão no `Cenário Mi`.
5. **Polígonos Espaciais `No-Transit` & Ilhamento Mecânico por Alavanca Vermelha:** A comunidade desenha polígonos criptográficos `No-Transit` ao redor das cirandas infantis (`Delta`), corredores de fauna (`Eta`), rodas culturais (`Lambda`), quartos em descanso (`Sigma`) e pátios residenciais (`Zeta`), enquanto cada casa mantém uma **Alavanca Vermelha puramente mecânica** para isolar sua bateria de $48\,\mathrm{V}$ ou cisterna instantaneamente com a própria mão.

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`FreightCourierIntent`:** Agendamento de uma rota consolidada multi-paradas de triciclo de carga ou barco/van agrupando caixas físicas e pacotes de dados `piggybackedDtnBundles` com fator de carga $\eta_{\mathrm{route}} \ge 6.000\,\mathrm{bp}$.
* **`ExergyIntertieIntent` & `HydroTransferIntent`:** Autorização de partilha par-a-par de watt-horas no microgrid CC, troca térmica hidrônica ou balanceamento de água entre cisternas de lotes vizinhos.
* **`NoTransitPolygonAmendment`:** Traçado ou atualização de uma fronteira espacial de santuário que proíbe veículos de carga ou rotas não autorizadas de atravessar zonas silenciosas ou protegidas (exceto ambulâncias ou veículos de emergência do `Cenário Qui`/`Ípsilon` conduzidos por humanos).
* **`CustodyHandoffReceipt`:** Duplo toque NFC (remetente/mensageiro/destinatário) atestando integridade de massa e envelope térmico na entrega.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Grafos Multi-Artéria de Rotas e Contenção (`https://w3id.org/collective/v1/`):** Modelam nós de lotes, enlaces de fibra escura/LoRa, folga de barramento HVDC e vazão ecológica preservada do rio (`instream_flow_preserved_bp >= 8000`) em um grafo espacial-topológico unificado.
* **Invariante de Custódia da Cadeia Fria e Sanitária:** O `col-commonsd` impede que qualquer `FreightCourierIntent` transportando alimentos ou insumos clínicos perecíveis libere sua pré-imagem `HTLC` no Livro-Contábil ou marque `DELIVERED_SAFE` se o registro térmico assinado em hardware na Camada 2 apontar qualquer excursão fora de $[275.150,\, 277.150]\,\mathrm{mK}$ (frio) ou $< 333.150\,\mathrm{mK}$ (quente), desviando automaticamente alimentos comprometidos para a compostagem termofílica do `Cenário Mi`.
* **Leitura Determinística Obrigatória de Interligação & `No-Transit`:** Antes que uma família ou assembleia assine um `ExergyIntertieIntent` ou `NoTransitPolygonAmendment`, a lente apresenta uma **Leitura Determinística (*Read-Back*)** em linguagem clara mostrando os pisos exatos de reserva da bateria doméstica (`SoC_bp` de sobrevivência em $R_0$ jamais é drenado por vizinhos) e as regras de passagem livre para emergências médicas.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Painel Offline Domínio-B no Guidão do Triciclo** | Fixado no Guidão do Triciclo Elétrico (`courier:dtn`, `crdt:sync`) | Tela monocromática reflexiva legível sob sol forte com botões de $\ge 64 \times 64\,\mathrm{px}$ operáveis com luvas e $100\%$ offline; condensa dezenas de entregas da quadra em 1 ou 2 rotas calmas por dia e sincroniza os pacotes *sneakernet* DTN via Wi-Fi/NFC em cada parada. |
| **Painel Mecânico da Alavanca Vermelha & Cisterna** | Parede de Utilidades da Casa (`intertie:steward`) | Alavanca contatora manual física pareada a um mostrador E-Ink biestável de $0\,\mathrm{mW}$ exibindo `SoC_bp` da bateria local, litros na cisterna e partilha ativa com vizinhos; puxar a alavanca vermelha isola fisicamente a casa em $< 5\,\mathrm{ms}$. |
| **Editor Espacial de Mapas de Santuário `No-Transit`** | Tablet / Desktop dos Comuns (`gov:ratify`) | Permite aos moradores desenhar zonas de silêncio e proteção ao redor de berçários, casas de idosos e matas ciliares, simulando rotas alternativas de carga antes da ratificação. |
| **Leitor NFC Duplo de Custódia na Caixa da Varanda** | Caixa Térmica de Entrega na Varanda (`courier:dtn`) | Base NFC sem tela nas caixas térmicas das varandas: o mensageiro encosta a caixa + crachá para comprovar a cadeia fria e liquidar o recibo `HTLC` no Livro-Contábil em menos de um segundo. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ pessoa atendida}$):**
   * *Contexto:* Um morador solitário gerenciando sua bateria solar de $48\,\mathrm{V}$, cisterna de água de chuva, jardim filtrante de águas cinzas, nó de rádio LoRa e triciclo elétrico de carga.
   * *Modelo de Governança:* **Zero reuniões, autoproteção automatizada em cinco níveis.** As Camadas 5 e 2 protegem automaticamente os pisos de sobrevivência em $R_0$ do lote—desligando cargas discricionárias da oficina antes da refrigeração ou iluminação essencial se a geração solar cair, e mantendo $\ge 8.000\,\mathrm{bp}$ ($80\%$) da vazão do córrego intocados.
2. **Guardião Único / Núcleo Pequeno Atendendo um Círculo Maior ($N_s = 1\text{ ou pequeno núcleo}, N_p > 1\text{ pessoas atendidas}$):**
   * *Contexto:* Um ou dois mensageiros / guardiões de microgrid do bairro conduzindo a rota matinal do triciclo de carga e mantendo o barramento compartilhado de $48\,\mathrm{V}$ e cisternas entre $5\text{ a }12$ casas vizinhas ($N_p > 1$).
   * *Modelo de Governança:*
     * **Consolidação em Lote contra a Fadiga de Entregas:** Em vez de permitir que 12 casas chamem o triciclo para entregas avulsas o dia inteiro, o solucionador `CVRP` agrupa pedidos não-emergenciais nas janelas consolidadas da manhã e da tarde ($\eta_{\mathrm{route}} \ge 6.000\,\mathrm{bp}$).
     * **Soberania de Ilhamento em Hardware:** Cada uma das $N_p$ casas conectadas mantém poder físico incondicional de puxar sua Alavanca Vermelha para desconectar sua bateria ou caixa-d'água a qualquer momento, sem precisar pedir permissão ao guardião do microgrid.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Guilda de Logística e Microgrid da bacia hidrográfica ($N_s = 10\text{ a }60+$ mensageiros, eletricistas e hidrólogos) operando interligações HVDC de $380\,\mathrm{V}$, barcos/vans elétricos e corredores de carga `HTLC` multi-salto através dos anéis $R_2 \dots R_5$.
   * *Modelo de Governança:*
     * **Veto Espacial Constituinte (`No-Transit`):** Sob a autonomia celular, qualquer quadra ou vila constituinte ($R_1 / R_2$) pode ratificar um polígono `No-Transit` sobre suas vias internas; a Guilda Regional de Logística é obrigada a contornar esse santuário (exceto em emergências médicas ou de incêndio do `Cenário Qui`/`Ípsilon` conduzidas por humanos).
     * **Liquidação Multi-Salto Encadeada via `HTLC` e `BCC`:** O transporte de cargas entre vales e as transferências de exergia HVDC são liquidados atomicamente no Livro-Contábil por meio de Canais de Compensação Bilateral (`BCC`) de soma zero usando recibos criptográficos de entrega ($\sigma_{\mathrm{delivery}}$), sem pedágios em moeda fiat.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Amortecimento Assimétrico com a Rede Elétrica e de Água Municipal:** Utilizar energia da concessionária fora de ponta ou água municipal durante estiagens severas nas Fases 1 e 2, antes que a captação solar e as cisternas locais atinjam $100\%$ de autonomia.<br>**B. Recebimento Consolidado de Cargas na Borda:** Receber entregas atacadistas (`GPO`) em uma única doca de perímetro para que caminhões a diesel comerciais jamais entrem nas ruas residenciais internas. | • **Redução Unidirecional de Dependência ($D_{\mathrm{energia}}, D_{\mathrm{agua}} \to 0$):** A Camada 3 registra cada watt-hora e litro importados no Índice de Dependência Legada do Livro-Contábil, priorizando a expansão de baterias e cisternas até que os disjuntores externos possam ser abertos em definitivo (`Cenário Ômega`).<br>• **Quarentena de Fracionamento na Borda (`VLAN 40`):** Transportadoras comerciais descarregam paletes na doca externa; a distribuição interna passa imediatamente para triciclos elétricos de carga e caixas retornáveis de aço inox. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Blindagem da Geração Distribuída Compartilhada, Uso Ecológico da Água & Transporte Associativo:** Impedir que monopólios de concessionárias ou órgãos de transporte enquadrem as rotas *milk-run* de triciclo entre vizinhos ou o microgrid comunitário como transporte comercial tarifado ou distribuição clandestina.<br>**B. Privacidade de Pacotes e Rotas (`Camuflagem Semântica Mundana`):** Proteger o tráfego de rádio e fibra entre nós contra vigilância de provedores comerciais. | • **Marco da Geração Distribuída (Lei 14.300/2022 & REN ANEEL 1.000/2021), Lei das Águas (Lei 9.433/1997) & Transporte Associativo:** Enquadra o compartilhamento solar/baterias em **Empreendimento com Múltiplas Unidades Consumidoras (`EMUC`)** e **Cooperativa de Geração Compartilhada** (homologado nas normas **`ABNT NBR 16149` e `PRODIST`**), supera com o piso de $\ge 80\%$ as exigências de vazão ecológica ($Q_{7,10}$ / $Q_{90}$) da **Lei 9.433/1997**, e ampara a circulação de marmitas e ferramentas nos triciclos como **Transporte Associativo Próprio sem Fins Comerciais e Depósito/Comodato Gratuito (Arts. 579 e 627 do Código Civil, zero ICMS/ISS)**.<br>• **Criptografia Ponta-a-Ponta Reticulum & WireGuard:** Todos os pacotes que cruzam vias públicas ou drives *sneakernet* DTN são criptografados em hardware. |
