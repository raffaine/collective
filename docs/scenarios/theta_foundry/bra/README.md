# Cenário Teta: Fundição Secundária, Metalurgia de Sucata via `XRF` & Cadinhos Solares em Lote (`SCN-THETA-FOUNDRY`)

* **Identificador:** `SCN-THETA-FOUNDRY`
* **Onda Horizontal:** Onda D (Terra Regenerativa, Desconstrução, Materiais & Arquitetura)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 07):** [../../../../doc/podcast/pt-br/episodes/ep07_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep07_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Teta é a **espinha dorsal metalúrgica e de matérias-primas pesadas** d'O Coletivo. Enquanto o `Cenário Alfa` usina e monta peças, o Cenário Teta **refina, funde, vaza em moldes, forja e sintetiza as ligas estruturais brutas, o vidro silicato e borossilicato, as cerâmicas refratárias e os reagentes minerais básicos** que tornam possíveis os `Cenários Alfa`, `Beta`, `Ksi` e `Qui` sem depender de mineradoras multinacionais.

Como a fusão de aço, bronze, alumínio e vidro exige altas temperaturas ($900\text{ a }1.900\,\mathrm{K}$) e guarda rigorosa contra riscos físicos, o Cenário Teta incorpora **restrição termodinâmica estrita** e **segurança física multipessoal em hardware** no centro de sua arquitetura:
* **A Hierarquia de Materiais em Três Níveis (*"Reuso Antes de Fusão, Fusão Antes de Mineração"*):** Sempre esgotando (1) o reuso dimensional direto a frio de vigas, eixos e chapas resgatadas no `Cenário Mi` antes de permitir (2) a refusão secundária de sucata agrupada no pico solar, e travando (3) qualquer extração mineral geosférica virgem atrás de uma supermaioria da assembleia da bacia hidrográfica e sobretaxa de $10\times$ o custo de reposição ecológica ($\lambda_{\mathrm{ERC}}$) no Livro-Contábil.
* **Intertravamentos de Vazamento a Duas Pessoas em $\le 250\,\mathrm{ms}$ (`NR-12` / `NR-15`):** Garantindo que nenhum forno de indução bascule metal líquido, nenhuma célula de eletrorecuperação de alta corrente opere e nenhum reator cáustico abra sem a copresença física simultânea de dois guardiões certificados e pressão negativa verificada nos lavadores de gases.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

Na indústria metalúrgica, vidreira e química básica legada (Camada 7), a produção de materiais primários apoia-se em quatro pilares destrutivos:

1. **Mineração a Céu Aberto & Rompimento de Barragens de Rejeitos:** Embora a humanidade já tenha extraído bilhões de toneladas de ferro, cobre e alumínio que estão paradas em sucatas e estruturas antigas, o mercado de commodities subsidia a mineração virgem a céu aberto que devasta serras e rios brasileiros e deixa barragens letais de rejeitos.
2. **Altos-Fornos a Carvão Fóssil/Desmatamento & Drenagem da Rede em Horário de Ponta:** Fundições convencionais queimam coque fóssil ou carvão vegetal ilegal e drenam megawatts contínuos da rede elétrica 24 horas por dia, em vez de sincronizar as corridas de alta entalpia com os picos de excedente solar ou micro-hídrico.
3. ***Downcycling* por Mistura Desleixada de Sucata:** Trituradores comerciais misturam fiação de cobre com aço e elementos contaminantes (chumbo, bismuto, cádmio, antimônio), degradando o metal reciclado em vergalhão quebradiço de baixa qualidade em vez de separar e refinar a sucata de volta em ligas nobres de grau mecânico e cirúrgico.
4. **Acidentes Graves de Fundição & Plumas Tóxicas Sem Lavagem:** Fundições comerciais precarizadas expõem trabalhadores a respingos de metal líquido, silicose e vapores de metais pesados enquanto lançam gases ácidos sobre bairros trabalhadores.

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Teta substitui a mineração predatória e os altos-fornos poluentes por cinco laços de precisão material:

1. **Hierarquia Obrigatória de Materiais em Três Níveis:**
   * *Nível 1 — Reuso Dimensional e Estrutural Direto (`Tier1RepurposeMatchIntent`):* Antes de derreter um único grama de metal ou vidro, o `col-commonsd` consulta o estoque resgatado pelo `Cenário Mi` (perfis I, eixos retificados, chapas e frascos de vidro) para encontrar uma correspondência dimensional direta (consumindo $< 5\%$ da exergia de refusão).
   * *Nível 2 — Refusão Secundária da Tecnosfera em Lote Solar & Eletrorecuperação (`PyrometallurgyAndElectrowinIntent`):* Fusão limpa em cadinho de indução de sucata classificada de aço, ferro nodular, bronze-silício, latão e alumínio, além de eletrorecuperação hidrometalúrgica de cobre e metais preciosos de placas eletrônicas (`Cenário Mi`)—exigindo $\ge 8.500\,\mathrm{bp}$ ($85\%$) de eficiência de preenchimento do cadinho e agendada estritamente durante o pico de excedente solar/hídrico ($S_{\mathrm{solar}} \ge 8.500\,\mathrm{bp}$).
   * *Nível 3 — Extração Geosférica de Último Recurso (`LastResortGeosphericExtractionIntent`):* Coleta local de argilas abundantes, areia de sílica ou calcário apenas quando os estoques de Nível 1 e Nível 2 estiverem esgotados em toda a biorregião $R_4$, carregando um mandato de restauração ecológica de $10\times \lambda_{\mathrm{ERC}}$.
2. **Metrologia de Ligas via `XRF`/`OES` & Bloqueio de Contaminantes:** Toda carga de cadinho e amostra de corrida fundida é verificada por Fluorescência de Raios-X (`XRF`) ou Espectroscopia de Emissão Óptica (`OES`) para garantir a estequiometria da liga e tetos estritos contra elementos contaminantes (`Pb`, `Cd`, `Bi`) antes do vazamento.
3. **Fornos de Vidro Silicato/Borossilicato & Cerâmica Refratária Técnica:** Refusão de cacos de vidro limpos em potes padronizados de conserva para as autoclaves do `Cenário Gama`, vidros duplos isolantes para o `Cenário Zeta` e vidraria de laboratório em borossilicato para os `Cenários Qui` e `Iota`, ao lado da queima de tijolos refratários de alta alumina.
4. **Síntese Solar de Cloro-Álcali & Reagentes Inorgânicos Básicos:** Eletrólise por membrana fechada de salmoura e calcinação solar de calcário/cinza vegetal para produzir reagentes essenciais de limpeza, saboaria e construção: hidróxido de sódio ($\mathrm{NaOH}$, soda cáustica para sabões do `Cenário Ni`/`Gama`), hipoclorito de sódio para desinfecção, hidróxido de cálcio ($\mathrm{Ca(OH)_2}$, cal hidratada para as paredes respiráveis do `Cenário Ksi`) e hidrogênio/oxigênio verdes para maçaricos de solda.
5. **Portão Bimanual de Homem-Morto a Duas Pessoas em $\le 250\,\mathrm{ms}$ & Cascata Térmica:** O basculamento do forno de indução exige assinatura física simultânea de dois guardiões em $\le 250\,\mathrm{ms}$ e pressão diferencial verificada nos filtros de manga e lavadores de gases (*wet-scrubbers*), enquanto a água das camisas de resfriamento do forno entrega calor de alto grau para as estufas de secagem de madeira do `Cenário Eta` e a rede hidrônica do `Cenário Beta`.

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`Tier1RepurposeMatchIntent`:** Disparado automaticamente sempre que uma planta do `Cenário Alfa` ou `Cenário Ksi` solicita metal estrutural ou vidro—buscando no inventário do `Cenário Mi` uma viga, eixo ou vaso intacto antes de autorizar qualquer corrida de forno.
* **`PyrometallurgyAndElectrowinIntent`:** Agendamento de uma corrida de cadinho de indução de Nível 2 no pico solar, fundição em areia/cera perdida, fornada de vidro ou lote de eletrorecuperação de cobre com preenchimento de cadinho $\ge 8.500\,\mathrm{bp}$.
* **`AlloyAndReagentAssayAttestation`:** Registro da composição elementar `XRF`/`OES` e limites de pureza em lingotes fundidos, barras, vidro borossilicato ou reagentes químicos.
* **`LastResortGeosphericExtractionIntent`:** Proposta de extração pontual de Nível 3 (argila/calcário/sílica) perante a Assembleia Biorregional $R_4$ com caução integral de restauração ecológica de $10\times \lambda_{\mathrm{ERC}}$.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Preempção de Três Níveis em `https://w3id.org/collective/v1/`:** O `col-commonsd` recusa-se a validar um `PyrometallurgyAndElectrowinIntent` a menos que o pacote inclua uma prova assinada `tier1CacheMissProof` demonstrando que não existe peça intacta compatível para reuso a frio no pátio local ou adjacente ($R_2$), e que a massa acumulada do lote atinge $\ge 8.500\,\mathrm{bp}$ da capacidade do cadinho.
* **Leitura Determinística Obrigatória de Vazamento & Nível 3:** Antes de energizar o forno de indução ou destravar o atuador de basculamento, o Console da Fundição exibe uma **Leitura Determinística (*Read-Back*)** em linguagem clara da massa da carga, ensaio `XRF` de contaminantes, folga solar `SoC_bp` do microgrid, pressão negativa do lavador de gases e as credenciais `T3_Foundry` (`Cenário Capa`) dos dois guardiões co-assinantes.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Console de Metrologia & Lote de Cadinho da Fundição** | Púlpito Blindado da Fundição (`theta:foundry`, `bpmn:review`) | Lousa industrial/E-Ink protegida contra calor: agrupa pequenos pedidos de fundição do `Cenário Alfa` ao longo da semana em uma única corrida de cadinho cheio ($\ge 8.500\,\mathrm{bp}$) no pico solar da tarde; exibe espectros `XRF`/`OES` ao vivo. |
| **Chaves de Homem-Morto a Duas Pessoas ($\le 250\,\mathrm{ms}$)** | Estação de Basculamento do Forno (`tool:unlock`, `theta:pour`) | Dois gatilhos físicos sem tela espaçados a $2{,}5\,\mathrm{m}$ de distância fora da zona de respingo: exige dois guardiões `Capa T3` mantendo contato simultâneo dentro de uma janela de $\le 250\,\mathrm{ms}$ enquanto a exaustão está verde. |
| **Pareador Nível-1 "Reuso Antes de Fusão"** | Tablet da Oficina / Pátio de Resgate (`crdt:read`, `intent:author`) | Intercepta requisições de matéria-prima bruta e mostra em 3D como um eixo ou chapa já resgatado no `Cenário Mi` pode ser usinado diretamente a frio sem gastar energia de refusão. |
| **Punção de Pedigree de Corrida Estampado no Lingote** | Estampado a Frio/Quente nos Lingotes | Marcação física/QR sem tela em cada tarugo de bronze-silício, alumínio ou aço vinculando-o ao seu laudo exato de composição `XRF`. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ morador}$):**
   * *Contexto:* Um morador na Fase 1A separando sucata de cobre/alumínio/aço na garagem e praticando o reuso mecânico a frio de Nível 1 (cortar, furar e parafusar peças resgatadas).
   * *Modelo de Governança:* **Fronteira absoluta de segurança física (`Proibido Vazamento Solitário de Metal Fundido`).** Enquanto um guardião solitário pode separar sucata (`Cenário Mi`), recozer pequenos fios de cobre ou operar uma pequena mufla cerâmica fechada com corte térmico em hardware, a política da Camada 5 do Cenário Teta **proíbe constitucionalmente o vazamento solitário de metal líquido ($N_s < 2$)**—encaminhando demandas de fundição para o mutirão em dupla da oficina do bairro (`Fase 1B/2`) para que ninguém jamais manuseie bronze ou ferro líquido sozinho.
2. **Equipe Pequena de Fundição Atendendo um Círculo de Bairro ($N_s = 2\text{ a }4\text{ fundidores}, N_p > 1\text{ artífices do bairro}$):**
   * *Contexto:* Uma equipe de $2\text{ a }4$ guardiões operando um forno de indução de $15\text{ a }50\,\mathrm{kW}$ e bancada de moldagem em areia/PLA perdido para um núcleo de bairro ($N_p > 1$).
   * *Modelo de Governança:*
     * **Copresença Obrigatória de Duas Pessoas ($N_s \ge 2$, sincronia $\le 250\,\mathrm{ms}$):** Todo vazamento exige dois guardiões certificados fisicamente nos gatilhos de homem-morto, combinados com pausas automáticas de hidratação/resfriamento do `Cenário Sigma` e recuperação hidroterápica na Casa de Banhos (`Cenário Zeta`) após o turno quente.
     * **Agrupamento em Lote de Cadinho ($\ge 8.500\,\mathrm{bp}$):** Em vez de ligar o forno toda vez que um vizinho precisa de uma única bucha de bronze, os pedidos do círculo $N_p$ acumulam-se no Console de Cadinho até que a árvore de moldes atinja $\ge 85\%$ da capacidade e chegue o pico solar de uma tarde ensolarada.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Guilda Regional de Fundição, Vidro e Química Básica ($N_s = 15\text{ a }80+$ metalurgistas, vidreiros e engenheiros químicos) operando fornos de indução pesados, laminadores, fornos de vidro borossilicato e células de cloro-álcali para toda uma federação de bacia hidrográfica ($R_3 \dots R_5$).
   * *Modelo de Governança:*
     * **Especialização Biorregional (Nem Toda Vila Precisa de uma Fundição Pesada):** Sob os tratados federados do `Cenário Pi`, um único polo regional do Cenário Teta instalado em um galpão industrial recuperado (*brownfield*) com alta disponibilidade solar/micro-hídrica refina lingotes, chapas, vidro borossilicato e reagentes básicos para $10\text{ a }30$ assentamentos vizinhos, compensando via canais `BCC` de soma zero no Livro-Contábil e transporte hidroviário/ferroviário do `Cenário Beta`.
     * **Supermaioria da Bacia $R_4$ para Extração de Nível 3:** Qualquer proposta rara de coleta geosférica de Nível 3 (argila, calcário, areia de sílica) exige aprovação por supermaioria da Assembleia da Bacia Hidrográfica $R_4$, caução de restauração ecológica de $10\times \lambda_{\mathrm{ERC}}$ e monitoramento de águas subterrâneas pelo `Cenário Iota`.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Arbitragem de Sucata Metálica & Aquisição de Elementos de Liga Residual:** Adquirir sucata metálica subvalorizada na cidade ou importar elementos de liga traço (ex.: ferrossilício, molibdênio, níquel ou tubos virgens de borossilicato) ainda não recuperados em pureza suficiente pela mineração urbana local.<br>**B. Exportação de Fundidos Reciclados / Bronze Artístico (`policy:commons_plus_l7_zero_bill`):** Vender peças fundidas sob encomenda ou vidraria reciclada para compradores externos a fim de custear refratários e o fundo de impostos do nó. | • **Rastreamento de Dependência de Ligas ($D_{\mathrm{materiais}}$):** Todo elemento de liga importado é registrado no Índice de Dependência Legada do Livro-Contábil (`Cenário Ômega`), orientando os scanners `XRF` do `Cenário Mi` a priorizar o resgate daqueles metais nas próximas desconstruções urbanas.<br>• **Desligamento Comercial em $\Phi_t = \text{R\$ } 0,00$:** A fundição comercial para clientes externos desliga-se automaticamente a cada mês assim que os custos de manutenção e boletos do nó batem $\text{R\$ } 0{,}00$. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Conformidade com Emissões Atmosféricas (`CONAMA 491/2018`) & Segurança Metalúrgica (`NR-12` / `NR-15`):** Proteger a fundição contra embargos de zoneamento industrial ou autuações ambientais.<br>**B. Rastreabilidade Legal de Sucata & Produtos Químicos (`Camuflagem Semântica Mundana`):** Cumprir exigências legais de origem de sucata metálica e armazenamento químico sem expor as árvores internas de `BOM` do nó. | • **Provas Criptográficas Contínuas dos Filtros de Manga e Lavadores de Gases (`CONAMA nº 491/2018`, `NR-12` e `NR-15`):** Sensores de pressão diferencial e particulados da Camada 2 gravam atestados contínuos assinados em hardware comprovando emissões muito abaixo dos tetos legais e operação 100% bimanual com EPIs certificados (`NR-6`).<br>• **Carapaça de Cooperativa Metalúrgica de Reciclagem (`Lei 5.764/1971` e `PNRS Lei 12.305/2010`):** Opera a entrada externa de sucata sob uma cooperativa regular de reciclagem de metais secundários pelo `l6_l7.sock`. |
