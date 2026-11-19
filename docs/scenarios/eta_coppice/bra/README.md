# Cenário Eta: Agrofloresta Sintrópica Regenerativa, Talhadia Rotacional & Botica da Floresta (`SCN-ETA-COPPICE`)

* **Identificador:** `SCN-ETA-COPPICE`
* **Onda Horizontal:** Onda D (Terra Regenerativa, Desconstrução, Materiais & Arquitetura)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 07):** [../../../../doc/podcast/pt-br/episodes/ep07_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep07_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Eta governa **como o assentamento trabalha em parceria viva com as florestas, sistemas agroflorestais sintrópicos (SAFs), bosques de talhadia rotacional, áreas micológicas e ecossistemas botânicos nativos**. Ele fecha o ciclo de matérias-primas biológicas para a madeira estrutural de construção (`Cenário Ksi`), marcenaria e madeiras acústicas de luthieria (`Cenário Alfa`/`Lambda`), fibras vegetais e taninos de curtume (`Cenário Ni`), castanhas e frutas perenes (`Cenário Gama`) e precursores fitoterápicos verificados (`Cenário Qui`).

Ao contrário da silvicultura industrial de corte raso que trata a floresta como uma mina de celulose a ser arrasada, o Cenário Eta opera sob **silvicultura regenerativa multidecadal e Agricultura Sintrópica (consagrada no Brasil por Ernst Götsch)**:
* **Talhadia Rotacional (*Coppicing* e *Pollarding*):** Manejo de espécies rebrotadoras vigorosas (eucalipto citriodora/cloeziana, bracatinga, angico, acácia-negra, bambu-gigante *Dendrocalamus asper*, castanheira e salgueiro) em ciclos rotacionais de $5\text{ a }25\text{ anos}$ onde o sistema radicular vivo e a rede micorrízica permanecem $100\%$ intactos, sequestrando carbono no solo e lançando varas e toras retas a cada ciclo.
* **Polígonos Sagrados de Santuário da Árvore-Mãe:** Imposição de um veto constitucional inquebrável em hardware e software sobre árvores-matrizes centenárias, matas ciliares (`APPs`) e corredores de fauna.
* **A Botica da Floresta:** Cultivo e coleta ética de ervas medicinais, resinas, óleos essenciais e cogumelos comestíveis/medicinais protegidos por duplo ensaio analítico de segurança.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

Nas cadeias florestais e botânicas legadas (Camada 7), a demanda humana por madeira, papel e compostos vegetais impulsiona quatro desastres ecológicos:

1. **Monocultura de Corte Raso & Erosão de Bacias Hidrográficas:** Empresas madeireiras arrasam ecossistemas biodiversos, pulverizam glifosato contra a regeneração nativa e plantam desertos verdes monoetários que secam nascentes, destroem o solo e explodem em incêndios florestais catastróficos (`Cenário Ípsilon`).
2. **Desperdício Energético em Estufas Fósseis & Madeira Envenenada com Arsênio (`CCA`):** A madeira comercial viaja centenas de quilômetros até estufas movidas a combustível fóssil e recebe tratamento sob pressão com arseniato de cobre cromado (`CCA`) ou creosoto tóxico que contamina o solo e impede qualquer compostagem segura em fim de vida (`Cenário Mi`).
3. **Sobre-Exploração Predatória de Plantas Nativas:** Mercados comerciais de suplementos e óleos essenciais esgotam populações silvestres de plantas medicinais e fungos até a extinção local sem monitoramento de abundância.
4. **Extratos Botânicos Adulterados & Sem Ensaio:** Produtos herbais comerciais frequentemente apresentam contaminação por metais pesados, resíduos de agrotóxicos ou espécies tóxicas confundidas na coleta (como fungos portadores de amatoxinas).

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Eta substitui o desmatamento extrativo e a coleta predatória por cinco laços ecológicos de longo horizonte:

1. **Florestas Alimentares de Sete Estratos & Talhadia na Dormência da Seiva (`HeavyTimberAndCoppiceIntent`):** Maneja consórcios agroflorestais sintrópicos de sete estratos (árvores emergentes e castanheiras de dossel, frutíferas de subdossel, cepas de talhadia madeireira, arbustos fixadores de nitrogênio, ervas perenes, raízes e trepadeiras) onde os cortes estruturais de talhadia só destravam durante o repouso fisiológico/dormência da seiva ($< 278.150\,\mathrm{mK}$ / $5{,}0^\circ\mathrm{C}$ nas serras do Sul/Sudeste ou auge da estação seca nos biomas tropicais) para maximizar a resistência natural contra brocas e fungos sem uma gota de veneno químico.
2. **Veto Ecológico Duro (`Polígonos de Santuário da Árvore-Mãe` / `ERR_MOTHER_TREE_SANCTUARY_VETO`):** Árvores-matrizes centenárias (jequitibás, castanheiras, araucárias, figueiras, ipês), núcleos micorrízicos, árvores-ninho e matas ciliares são mapeados como polígonos sagrados de santuário que carregam um veto absoluto em hardware e software (`ERR_MOTHER_TREE_SANCTUARY_VETO`), bloqueando qualquer requisição de corte independentemente da demanda da oficina.
3. **Teto Ético de Coleta Silvestre de 1-em-4 ($\le 2.500\,\mathrm{bp}$ / $25\%$ — `ForagingQuotaIntent`):** A coleta extrativista sustentável de frutos nativos (açaí, juçara, pequi, pinhão, baru), raízes medicinais, cascas e cogumelos é limitada em hardware a $\le 2.500\,\mathrm{bp}$ ($25\%$ da abundância observada no bosque), deixando $\ge 75\%$ intocados para a fauna silvestre, dispersão de sementes e regeneração natural.
4. **Estufas Solares de Desumidificação de Madeira:** Curam a madeira estrutural até $\le 1.200\,\mathrm{bp}$ ($12{,}0\%$) de umidade de equilíbrio para os encaixes *Kigumi* do `Cenário Ksi` e madeiras acústicas de luthieria até $\le 800\,\mathrm{bp}$ ($8{,}0\%$) para os instrumentos do `Cenário Lambda`, utilizando coletores solares térmicos, bombas de calor e calor residual da fundição (`Cenário Teta`).
5. **Botica Botânica & Micológica de Ensaio Duplo (`PharmacopoeiaBatchAttestation`):** Cogumelos cultivados em toras e câmaras de frutificação (*Hericium* juba-de-leão, *Trametes*, *Lentinula* shiitake) e tinturas/óleos essenciais das **Farmácias Vivas** passam obrigatoriamente por ensaios duplos de pureza co-assinados por dois guardiões verificando `amatoxin == 0 ng` e metais pesados `< 100 ppb` antes de entrarem na despensa do `Cenário Gama`, nos taninos do `Cenário Ni` ou na clínica do `Cenário Qui`.

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`HeavyTimberAndCoppiceIntent`:** Solicitação de manejo de talhadia na janela de repouso da seiva ou aproveitamento de árvore caída por vento, rastreando cada tora desde a cepa até a pilha da estufa solar e a entrega na oficina.
* **`ForagingQuotaIntent`:** Registro de levantamento de abundância de um bosque nativo e reserva de cota sazonal de coleta estritamente dentro do teto ecológico de $\le 2.500\,\mathrm{bp}$ ($25\%$).
* **`PharmacopoeiaBatchAttestation`:** Co-assinatura da liberação analítica de segurança e potência de um extrato fitoterápico, banho de tanino vegetal (`Cenário Ni`) ou lote de cogumelos medicinais.
* **`MotherTreeSanctuaryAmendment`:** Ampliação de um polígono permanentemente protegido de mata nativa, Árvore-Mãe ou mata ciliar (`APP` / `RPPN`).

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **94% de Máquinas de Estado Fenológicas em Silêncio (`DORMANT`):** Como cepas de talhadia crescem ao longo de $5\text{ a }25\text{ anos}$ e pilhas de madeira curam por $6\text{ a }18\text{ meses}$, o `col-commonsd` e o `col-execd` mantêm $94\%$ dos mais de $340$ fluxos simultâneos do Cenário Eta em estado silencioso `DORMANT`—despertando um guardião humano apenas quando as sondas de solo indicam a janela de dormência da seiva ou quando os sensores da estufa solar atingem a umidade-alvo ($\le 1.200\,\mathrm{bp}$ / $\le 800\,\mathrm{bp}$).
* **Leitura Determinística Obrigatória de Santuário & Dormência:** Antes que um agroflorestador assine um `HeavyTimberAndCoppiceIntent`, o caderno de campo exibe uma **Leitura Determinística (*Read-Back*)** confirmando zero interseção com os `Polígonos de Santuário da Árvore-Mãe`, janela de repouso da seiva verificada e crescimento líquido positivo da área basal do dossel ($F_{\mathrm{eco}} > 0$) no Livro-Contábil.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Caderno de Campo GIS Agroflorestal Offline (`Domínio-B`)** | Caminhada na Mata / Tablet E-Ink (`agro:forestry`, `crdt:sync`) | Mapa offline de curvas de nível exibindo idade das cepas de talhadia, fronteiras do Santuário da Árvore-Mãe e medidores de coleta silvestre ($\le 2.500\,\mathrm{bp}$); sincroniza via `LoRa` (`Cenário Beta`) ou Wi-Fi do galpão ao retornar. |
| **Lousa da Botica Botânica de Ensaio Duplo** | Bancada da Farmácia Viva / Laboratório (`pharma:assay`) | Lousa de alto contraste que exige a leitura e assinatura de dois fitoterapeutas/químicos certificados no `Cenário Capa` confirmando `amatoxin == 0 ng` e metais pesados `< 100 ppb` antes de imprimir os rótulos do lote. |
| **Painel E-Ink de Umidade & Cura da Estufa Solar** | Galpão de Secagem de Madeira (`agro:forestry`, `bpmn:review`) | Visor E-Ink biestável de $0\,\mathrm{mW}$ em cada pilha de madeira exibindo a umidade do cerne em pontos-base (`1200 bp` estrutural / `800 bp` luthieria) para que os carpinteiros nunca cortem madeira verde. |
| **Plaqueta de Linhagem em Inox Estampado Sem Tela** | Cravada no Topo das Toras / Cepas | Etiqueta física em aço inox com número/QR vinculando cada viga estrutural no `Cenário Ksi` exatamente à cepa de talhadia e ao ano da colheita. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ morador}$):**
   * *Contexto:* Um morador cuidando de um quintal agroflorestal, uma cerca-viva de bambu/gliricídia para estacas de horta, toras de shiitake e um secador solar de ervas medicinais.
   * *Modelo de Governança:* **Calendário fenológico calmo & guia de segurança botânica.** A Camada 6 permanece em silêncio por meses a fio, trazendo lembretes suaves apenas quando chega a janela de poda de inverno/seca ou quando as toras de shiitake estão prontas para hidratação, além de listas de verificação contra micotoxinas para os chás da casa.
2. **Guardião Único / Núcleo Pequeno Atendendo um Círculo Maior ($N_s = 1\text{ ou pequeno núcleo}, N_p > 1\text{ núcleo de bairro}$):**
   * *Contexto:* Um ou dois agroflorestadores/herbalistas cuidando de um bosque comunitário de 1 a 2 hectares, uma estufa solar de secagem de madeira e um armário de fitoterápicos para $10\text{ a }30$ famílias ($N_p > 1$).
   * *Modelo de Governança:*
     * **Prevenindo a Tragédia dos Comuns na Coleta Silvestre:** Quando os $N_p$ vizinhos caminham pelo bosque para colher frutos nativos, cogumelos ou ervas medicinais, o medidor compartilhado `ForagingQuotaIntent` fecha suavemente a coleta naquela mancha (`EM_REPOUSO_PARA_A_FAUNA`) assim que a colheita acumulada atinge $2.500\,\mathrm{bp}$ ($25\%$), protegendo o ecossistema sem precisar de guardas florestais.
     * **Dupla Assinatura na Botica Comunitária:** Antes que qualquer tintura herbal ou lote de cogumelos colhidos entre no armário compartilhado do `Cenário Gama`/`Qui`, um segundo guardião qualificado co-assina a `PharmacopoeiaBatchAttestation`.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Guilda de Silvicultura Sintrópica e Botica da bacia hidrográfica ($N_s = 15\text{ a }80+$ engenheiros florestais, agroecologistas, raizeiros e micologistas) cuidando de centenas de hectares de agrofloresta, madeira de talhadia e aceiros verdes hidratados (`Cenário Ípsilon`) através dos anéis $R_2 \dots R_5$.
   * *Modelo de Governança:*
     * **Personalidade Constitucional da Floresta (Veto $F_{\mathrm{eco}}$):** Tanto a Guilda de Silvicultura quanto a Câmara dos Comuns reconhecem os `Polígonos de Santuário da Árvore-Mãe` como invioláveis; qualquer tentativa de corte não autorizado dentro de um polígono de santuário dispara imediatamente um processo de restauração ecológica no `Cenário Psi`.
     * **Guarda Intergeracional Multidecadal com os Sábios Eméritos (`Cenário Fi`):** Bosques de madeira de lei e castanheiras de ciclo longo (plantados por uma geração para amadurecer para a seguinte) são co-governados com os Sábios Eméritos do `Cenário Fi`, que guardam a memória fenológica multidecadal da bacia hidrográfica.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Aquisição de Mudas Matrizes Crioulas & Micélio Semente:** Importar mudas enxertadas resistentes, sementes crioulas ou culturas iniciais de fungos para implantar novos estratos perenes.<br>**B. Exportação Certificada de Madeira Acústica / Extratos Botânicos (`policy:commons_plus_l7_zero_bill`) & Pagamento por Serviços Ambientais (`PSA — Lei 14.119/2021`):** Vender uma pequena fração de madeira seca ao sol ou óleos essenciais e receber recursos de `PSA` por preservação de nascentes para quitar custos de aquisição de terra ou equipamentos florestais. | • **Multiplicação Local em Viveiro Próprio:** Mudas e culturas miceliais importadas são imediatamente multiplicadas nos viveiros e laboratórios do próprio nó (`Cenário Iota`) para que a dependência externa caia a $0\,\mathrm{bp}$.<br>• **Piso de Crescimento Basal & Teto $\Phi_t = \text{R\$ } 0,00$:** Vendas externas de madeira ou extratos são bloqueadas a menos que o crescimento anual do bosque supere a colheita em $\ge 2.000\,\mathrm{bp}$ ($20\%$) e desligam-se automaticamente assim que as obrigações mensais do nó atingem $\text{R\$ } 0{,}00$. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Isenção de 100% do `ITR` via `RPPN`, Reserva Legal Sintrópica & Servidão Ambiental ($70\%$ a $100\%$ de Corte Tributário):** Neutralizar de forma permanente o maior custo tributário recorrente sobre terras rurais e periurbanas no Brasil.<br>**B. Defesa Contra Biopirataria & Proteção de Sementes Crioulas (`Lei de Sementes 10.711/2003`):** Impedir que corporações agroquímicas patenteiem cultivares locais ou conhecimentos tradicionais. | • **Averbação Perpétua de `RPPN` (`Lei 9.985/2000 Art. 21`), Manejo Agroflorestal em Reserva Legal (`Código Florestal Lei 12.651/2012 Art. 66 § 3º`) & Isenção de `ITR` (`Lei 9.393/1996 Art. 10 § 1º II`):** Averba os Polígonos de Santuário da Árvore-Mãe como **Reserva Particular do Patrimônio Natural (`RPPN`)** e as áreas produtivas como Sistemas Agroflorestais (`SAFs`) e Manejo Florestal Comunitário (`Lei 11.284/2006`), blindando juridicamente a floresta contra loteamento comercial e **reduzindo o `ITR` em $70\%$ a $100\%$ por lei federal**.<br>• **Anterioridade Defensiva & Proteção de Sementes Crioulas:** Publica descrições fenotípicas como *Prior Art* defensivo via `Cenário Iota` e ampara a troca livre de sementes crioulas no **`Art. 8º, § 3º da Lei de Sementes (Lei 10.711/2003)`**. |
