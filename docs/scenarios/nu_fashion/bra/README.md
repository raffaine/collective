# Cenário Ni: Bacia Regional de Fibras, Têxteis Zero-PFAS/Zero-Cr(VI) & Alfaiataria e Calçados em RAM Efêmera (`SCN-NU-TEXTILES`)

* **Identificador:** `SCN-NU-TEXTILES`
* **Onda Horizontal:** Onda D (Terra Regenerativa, Desconstrução, Materiais & Arquitetura)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 07):** [../../../../doc/podcast/pt-br/episodes/ep07_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep07_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Ni governa **como o assentamento veste sua gente, fabrica calçados ressoláveis que duram décadas, tece roupas de cama, mesa e banho, confecciona membranas técnicas de abrigo e produz Equipamentos de Proteção Individual (`EPIs` / `PPE`) certificados**. Ele fecha o ciclo da **Bacia Regional de Fibras (*Fiber Shed*)**—desde o algodão agroecológico colorido/orgânico brasileiro, rami, fibras nativas (caroá, buriti), linho e cânhamo industrial no `Cenário Eta`/`Gama`, lã de tosquia rotacional em silvipastoril (como nos Pampas gaúchos e na Serra da Estrela) e couros curtidos estritamente ao tanino vegetal de casca de angico e acácia-negra até a fiação, tecelagem, tingimento botânico, alfaiataria sob medida, sapataria *Goodyear-welt* e remendo artístico visível (*Sashiko*).

Acima de tudo, o Cenário Ni estabelece duas garantias antidistópicas no coração da vida cotidiana:
* **Propriedade Pessoal Inviolável em $R_0$ & Alfaiataria Sem Vigilância Biométrica (`OP_PURGE_R0_LINEAGE`):** Refuta para sempre a caricatura autoritária de "todo mundo vestindo o mesmo macacão cinza de colônia utópica". Cada camisa, vestido, casaco ou par de botas feito sob medida para uma pessoa pertence permanentemente a ela em seu anel privado $R_0$, e as medidas 3D do corpo e dos pés usadas durante o corte vivem estritamente em memória RAM volátil (`ramfs`) e são **criptograficamente zeroizadas no exato segundo da entrega da peça**.
* **Ecologia Zero-PFAS, Zero-Cromo Hexavalente (`Cr(VI)`) & Livre de Microplásticos:** Elimina da bacia hidrográfica o desprendimento de microfibras sintéticas, os repelentes "químicos eternos" (`PFAS`) e o curtume tóxico com cromo hexavalente.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

Na indústria global legada de moda e calçados (Camada 7), vestir corpos humanos provoca quatro graves danos sociais e ecológicos:

1. **Oficinas Precarizadas de *Fast-Fashion* & Montanhas de Lixo Têxtil:** Bilhões de peças são costuradas em regime exaustivo a partir de misturas baratas de poliéster e acrílico projetadas para deformar, rasgar ou sair de moda após dez lavagens—despejando milhões de toneladas de resíduos têxteis não-biodegradáveis em aterros e desertos do Sul Global.
2. **"Químicos Eternos" (`PFAS`), Curtume ao Cromo-VI & Corantes Azo Tóxicos:** Jaquetas impermeáveis comerciais, tecidos antimanchas e calçados curtidos ao cromo lixiviam compostos perfluorados (`PFAS`) e cromo hexavalente cancerígeno (`Cr(VI)`) nos rios e na corrente sanguínea humana.
3. **Poluição Aquática por Microplásticos na Lavagem Doméstica:** Cada ciclo de lavagem de roupas de poliéster ou nylon libera centenas de milhares de microfibras plásticas persistentes nos rios e cadeias alimentares aquáticas.
4. **Calçados Colados Irreparáveis & Captura Biométrica de Medidas:** Sapatos comerciais são colados com espumas de poliuretano que esfarelam e impedem a troca do solado, enquanto aplicativos comerciais de "provador 3D" enviam a geometria corporal íntima dos usuários para nuvens de marketing corporativo.

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Ni substitui o *fast-fashion* e os sintéticos tóxicos por cinco laços regenerativos de têxteis e calçados:

1. **Bacia Regional de Fibras & Dornas Botânicas de Tingimento e Curtume Vegetal:** Beneficia algodão agroecológico, rami, linho, cânhamo, fibras nativas (caroá, buriti), lã de tosquia ética e algodão reciclado (`Cenário Mi`), tingidos com pigmentos botânicos mordentados (`Cenário Eta`: urucum, índigo, jenipapo, açafrão, casca de cebola e nogueira) e curtidos estritamente com taninos vegetais de casca de angico, acácia-negra e carvalho com verificação analítica de **`0 mg` de `PFAS` e `0 mg` de `Cr(VI)`**, impermeabilizados com emulsões renováveis de cera de abelhas nativas, óleo de linhaça e breu de pinho.
2. **Alfaiataria Sob Medida em RAM Efêmera & Sapataria *Goodyear-Welt* (`OP_PURGE_R0_LINEAGE`):** Plotters paramétricos de moldes ($\ge 9.200\,\mathrm{bp}$ / $92\%$ de aproveitamento do tecido no encaixe) e tornos CNC de 3 eixos (`Cenário Alfa`) usinam formas ortopédicas personalizadas para os pés em madeira dura do `Cenário Eta` e cortam roupas sob medida. No exato instante em que a roupa ou bota costurada com vira *Goodyear-welt* / *stitchdown* é entregue ao morador, o `col-commonsd` executa `OP_PURGE_R0_LINEAGE`—zeroizando criptograficamente todos os buffers de medidas corporais na RAM volátil e transferindo a peça em definitivo para fora do Livro-Contábil público, na propriedade privada $R_0$.
3. **Remendo Visível *Sashiko* de Primeira Classe & Ressola de Botas (`TextileRepairBounty`):** Celebra o cerzido artístico, o bordado de reforço *Sashiko*, a troca de zíperes e a ressola de botas em 30 minutos consumindo $< 1.500\,\mathrm{bp}$ ($15\%$) da exergia de uma peça nova, estendendo a vida útil de roupas e calçados por décadas.
4. **`EPIs` Técnicos Certificados (`NR-6`) & Têxteis Clínicos/Infantis:** Confecciona aventais de couro/Kevlar e luvas térmicas para a fundição (`Cenário Teta`/`Alfa`), calças anticorte para manejo florestal (`Cenário Eta`), campos cirúrgicos e pijamas clínicos esterilizáveis (`Cenário Qui`), e fraldas reutilizáveis de algodão orgânico macio e enxovais para bebês (`Cenário Delta`).
5. **Filtragem de Microfibras de $\ge 9.800\,\mathrm{bp}$ ($98\%$) nas Lavanderias & Fim de Ciclo em Papel de Trapo ou Isolamento de Micélio:** Equipa todas as máquinas de lavar dos `Cenários Zeta` e `Delta` com filtros de vórtice de $50\,\mu\mathrm{m}$ ($\ge 98\%$ de retenção) para conter microfibras de roupas sintéticas antigas trazidas da Camada 7, enquanto tecidos naturais em fim de vida viram polpa de trapo de algodão/linho para os livros de 300 anos da Biblioteca de Alexandria (`Cenário Iota`) ou substrato para painéis isolantes de micélio (`Cenário Ksi`).

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`PersonalGarmentCraftIntent`:** Solicitação ou confecção própria de roupa sob medida, agasalho de inverno ou bota em forma personalizada—disparando automaticamente `OP_PURGE_R0_LINEAGE` na entrega física para que zero medidas corporais ou inventários de guarda-roupa persistam.
* **`TextileRepairBounty`:** Entrega de uma jaqueta rasgada, salto gasto ou avental de oficina para remendo visível *Sashiko*, reenceramento ou ressola *Goodyear-welt*.
* **`TechnicalPpeAndCommonsLinenIntent`:** Encomenda de equipamentos coletivos de segurança em $R_2$ (aventais de fundição, luvas de solda, cortinas acústicas, lonas térmicas, toalhas da Casa de Banhos ou kits de fraldas do `Cenário Delta`) com rastreamento completo de segurança e higienização.
* **`DyeAndTanneryVatAttestation`:** Verificação de `0 mg` de `PFAS`, `0 mg` de `Cr(VI)`, recuperação em ciclo fechado de mordentes e pH neutro antes de qualquer descarte de banho.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Separação Arquitetônica Rígida entre Vestuário Pessoal ($R_0$) e Equipamentos Técnicos Compartilhados ($R_2$):** Em `https://w3id.org/collective/v1/`, os `EPIs` coletivos de segurança em $R_2$ (ex.: luvas isolantes de alta tensão ou casaco térmico de fundição) mantêm uma etiqueta NFC de linhagem rastreando intervalos de inspeção e graus de proteção (`NR-6`). Em sentido oposto, todas as roupas e botas pessoais em $R_0$ possuem **zero chips RFID/NFC** e **zero nós persistentes no grafo** assim que `OP_PURGE_R0_LINEAGE` dispara—registrando no Livro-Contábil apenas a massa agregada de tecido (`mg`) e a exergia do tear/cortadora (`uJ`).
* **Leitura Determinística Obrigatória de Retenção Zero (`ramfs`):** Antes que um morador tire medidas no Ateliê para um `PersonalGarmentCraftIntent`, a lousa apresenta uma **Leitura Determinística (*Read-Back*)** comprovando que o buffer geométrico está alocado exclusivamente em memória volátil `ramfs` (`0` permissão de escrita em disco) e será apagado assim que o molde for cortado.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Lousa do Ateliê de Tecido & Forma de Sapato em RAM Efêmera** | Bancada de Alfaiataria & Sapataria (`nu:atelier`, `r0:volatile_ram`) | Seletor interativo de estilo, tecido, cor e caimento que encaixa os moldes na peça de tecido com aproveitamento $\ge 9.200\,\mathrm{bp}$, aciona o torno CNC da forma do pé e exibe confirmação física `RAM ZEROIZED` na entrega. |
| **Quadro de Remendo Visível *Sashiko* & Sapataria** | Ateliê de Costura do Bairro (`nu:repair`) | Quadro calmo de entrega e retirada conectando pedidos de conserto a aprendizes têxteis (`Cenário Capa`) e rodas culturais de bordado e cerzido (`Cenário Lambda`). |
| **Monitor Químico das Dornas & Filtro de Microfibras** | Tinturaria / Lavanderia Comunitária (`nu:vat`, `bpmn:review`) | Condensa tempos de curtume vegetal ao tanino de angico, potencial de redução das dornas de índigo e alertas de limpeza dos filtros de $50\,\mu\mathrm{m}$ em uma única ronda diária. |
| **Projetor LED de Moldes a Giz Sem Tela** | Mesa de Corte (`nu:atelier`) | Projeta as linhas de corte encaixadas diretamente sobre o rolo de algodão, linho ou lã com luz LED no teto (ou imprime moldes reutilizáveis em papel kraft) para que alfaiates trabalhem com as mãos livres usando tesoura e giz. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Em todas as escalas, a governança da Camada 5 no Cenário Ni ancora-se na fórmula unificada de peso de voto restrita estritamente aos **Membros do Nó ($N_s$)**:
$$V_i(t) = B + R_i(t) = 1 + R_i(t) \qquad \left(V_{i,\mathrm{bp}}(t) = 10.000 + R_{i,\mathrm{bp}}(t)\right)$$
onde $B = 1$ ($10.000\,\mathrm{bp}$) é a **Base Cívica Universal** não-decaível e $R_i(t)$ é a reputação de guarda têxtil no Livro-Contábil Termodinâmico ($\tau_{1/2}$). Sob o **Pilar Constitucional I**, 85% da capacidade dos teares atende vestuário essencial e EPIs de trabalho, enquanto 15% ficam reservados para *montage* autoral e expressão criativa.

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ pessoa}$):**
   * *Contexto:* Um morador operando sua máquina de costura, kit de cerzido e pequena panela de tingimento botânico com urucum/açafrão.
   * *Modelo de Governança:* **Pura autoexpressão criativa.** Zero aprovações ou registros; o morador usa a biblioteca offline de moldes e a ferramenta de encaixe para costurar e consertar suas próprias roupas livremente.
2. **Guardião Único / Núcleo Pequeno Atendendo um Círculo Maior ($N_s = 1\text{ ou pequeno núcleo}, N_p > 1\text{ família & vizinhos}$):**
   * *Contexto:* Um ou dois alfaiates/sapateiros cuidando de um ateliê compartilhado de costura e conserto de calçados para $10\text{ a }30$ casas vizinhas ($N_p > 1$).
   * *Modelo de Governança:*
     * **Soberania Incondicional do Guarda-Roupa Pessoal ($B_{\mathrm{baseline}}$):** Toda criança, idoso e adulto no círculo $N_p$ tem direito a roupas sazonais bem ajustadas e calçados ressoláveis como parte do piso $B_{\mathrm{baseline}}$—com liberdade estética total sobre cores, cortes e estilos culturais (zero imposição de uniformes).
     * **Rodas de Cerzido & Acesso Livre Fora de Pico:** Vizinhos com credencial `L1` de costura/tear no `Cenário Capa` podem reservar o ateliê fora do horário de pico (`PersonalSlotIntent`) para costurar presentes ou figurinos com zero vigilância, enquanto as rodas semanais de remendo *Sashiko* (`Cenário Sigma`/`Lambda`) evitam que o alfaiate principal fique sobrecarregado.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Guilda Regional de Fiação, Tecelagem, Curtume Vegetal e Sapataria ($N_s = 10\text{ a }60+$ fiandeiras, tecelões, tintureiros, alfaiates e sapateiros) operando beneficiamento de algodão/rami/linho, teares Jacquard, curtumes vegetais de angico/acácia e bancadas de teste de `EPIs` através dos anéis $R_2 \dots R_4$.
   * *Modelo de Governança:*
     * **Proibição Constitucional de Químicos Tóxicos (`0 mg` PFAS / Cr(VI)):** A Câmara dos Comuns e a Câmara de Guardiões impõem proibição constitucional absoluta de importar ou aplicar tratamentos perfluorados (`PFAS`), sais de cromo hexavalente (`Cr(VI)`) ou corantes azoicos tóxicos em qualquer ponto da bacia hidrográfica.
     * **Certificação de Dupla Chave para `EPIs` (`NR-6`):** Lotes compartilhados de `EPIs` industriais (aventais de couro de fundição, perneiras anticorte, campos cirúrgicos) exigem co-assinatura de segurança `T3` do `Cenário Capa` comprovando resistência mecânica, térmica ou esterilidade antes de entrarem nas oficinas.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Compra Coletiva de Fibras Técnicas & Insumos de Costura (`ProcurementIntent`):** Importar fios de aramida (`Kevlar`/`Nomex`) para `EPIs` de bombeiros e fundição, agulhas cirúrgicas/industriais de aço inox ou borracha natural crepe da Amazônia para solados quando ainda não produzidos localmente.<br>**B. Exportação de Alfaiataria Artesanal, Malharia & Botas *Goodyear-Welt* (`policy:commons_plus_l7_zero_bill`):** Vender casacos de lã natural, peças de algodão agroecológico ou botas artesanais costuradas para clientes externos na Camada 7 a fim de custear teares e zerar boletos do nó. | • **Redução de Dependência de Fibras ($D_{\mathrm{materiais}}$):** Fios técnicos ou tecidos importados são registrados no Índice de Dependência Legada do Livro-Contábil, guiando a ampliação do plantio consorciado de algodão agroecológico, rami e linho no `Cenário Eta` e a reciclagem têxtil no `Cenário Mi`.<br>• **Prioridade Estrita ao Piso Interno ($B_{\mathrm{baseline}}$) & Desligamento em $\Phi_t = \text{R\$ } 0,00$:** Vendas externas sob medida só ocorrem após atender $100\%$ das necessidades sazonais de roupas e botas dos moradores e os enxovais de bebês do `Cenário Delta`, desligando-se automaticamente assim que os boletos mensais do nó batem $\text{R\$ } 0{,}00$. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Conformidade com a `NR-6` (`EPIs`) & Normas de Segurança Têxtil Infantil (`ABNT` / `INMETRO`):** Proteger as operações das oficinas e a distribuição comunitária contra passivos regulatórios.<br>**B. Privacidade Biométrica e Estética (`LGPD` e `Camuflagem Semântica Mundana`):** Impedir que plataformas externas perfilem medidas corporais ou hábitos de vestuário dos moradores. | • **Laudos de Ensaio de `EPIs` (`NR-6`) & Carapaça de Cooperativa Artesanal (`Lei 5.764/1971`):** Registra atestados de ensaios de tração/chama nos lotes de `EPIs` coletivos para auditorias de segurança do trabalho e opera vendas externas sob o **Ato Cooperativo**.<br>• **Inexistência Absoluta de Dados Biométricos de Medidas (`LGPD Lei 13.709/2018`):** Como a instrução `OP_PURGE_R0_LINEAGE` zeroiza todas as medidas do corpo e dos pés na RAM volátil no ato da entrega, **zero registros biométricos ou de numeração existem** para vazar ou serem intimados pelo `l6_l7.sock`. |

---

## 7. Vinculação Arquitetural do Conselho Horizonte v3.2 (*A Arquitetura do Tempo Ilimitado* — D1–D8)

| Dimensão do Conselho Horizonte | Vinculação no Cenário Ni (`SCN-NU-FASHION` / `nu_fashion`) |
| :--- | :--- |
| **Regime de Membros do Nó ($N_s$) & Voz Unificada $V_i(t) = 1 + R_i(t)$** | $N_s = 1 \to 12$ tecelões, alfaiates e guardiões têxteis (forte autonomia criativa em $N=1$ & democracia de guilda $N_s > R_i(t) + 2$). |
| **Lente Primária da Camada 6 & Modo de Captura de Necessidades** | ***A Lente da Oficina*** (desgaste de ciclos de teares/overloques, árvore local de moldes bifurcáveis, reserva criativa de 15% para *montage*). |
| **Concessão do Hipervisor de Nó Envolvente (`cB`, `mW`, `mK`, `mm`)** | **Envelope do Ateliê Têxtil Compartilhado** (`6.500 cB`, `1.200.000 mW`, trava de fitorremediação de banhos de tingimento). |
| **Cascata L4 de 3 Níveis & Arquétipo de Retrofit de Hardware Estágio I$\to$III** | Escalonamento SI de Nível 1 na largura do rolo (`mm`) e macro-substituição de costuras/fechos de Nível 2; bordadeira com DRM (Estágio I) $\to$ retrofit com controlador aberto de motores de passo (Estágio II). |
