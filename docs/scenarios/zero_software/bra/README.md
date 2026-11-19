# Cenário Zero: O Comum de Software Soberano, Jogos e Cultura Digital (`SCN-ZERO-SOFTWARE`)

* **Identificador:** `SCN-ZERO-SOFTWARE`
* **Onda Horizontal:** Onda A (Meta-Núcleo Autopoético, Acolhimento & Homeostase)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 04):** [../../../../doc/podcast/pt-br/episodes/ep04_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep04_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Zero é o **meta-cenário digital autopoético** d'O Coletivo. Ele define como um assentamento ou bairro projeta, compila, verifica, distribui e mantém toda a sua pilha de software—desde o firmware de microcontroladores em tempo real e kernels de sistema operacional até os daemons das sete camadas da Pilha Soberana (`col-*`), a **Constelação aberta de Lentes de Aplicação da Camada 6**, suítes criativas colaborativas, sandboxes de simulação 3D interativa (**Oasis**), jogos eletrônicos, *mods* comunitários e bibliotecas digitais offline.

Inspirado tanto na engenharia de sistemas enxutos quanto na tradição brasileira dos **Pontos de Cultura, Software Livre e Metareciclagem**, o Cenário Zero não trata o software como um serviço corporativo alugado ou um painel de controle burocrático, mas como um **ofício convivial de propriedade local**. Todo binário que roda num nó é compilado de modo reprodutível a partir de código-fonte auditável, projetado para rodar com agilidade em computadores resgatados de vinte anos de idade, livre de telemetria de vigilância e padrões obscuros viciantes (*dark patterns*), e acoplado termodinamicamente ao território para que o calor residual dos compiladores e GPUs aqueça a água do banho comunitário e as estufas de mudas.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

Na economia legada de software e mídia digital (Camada 7), pessoas e comunidades enfrentam quatro armadilhas sistêmicas:

1. **Feudalismo da Nuvem & Extração de Renda via SaaS:** Ferramentas cotidianas de trabalho, suítes de CAD, controladores agrícolas e softwares criativos ficam trancados atrás de assinaturas perpétuas em nuvem. Se o fornecedor aumenta os preços, vai à falência ou bloqueia uma conta remotamente, a comunidade perde da noite para o dia o acesso às suas próprias ferramentas e registros históricos.
2. **Inchaço de Software & Obsolescência Programada de Lixo Eletrônico:** Sistemas operacionais comerciais e aplicativos empacotados em navegadores consomem gigabytes de memória RAM para tarefas triviais, tornando notebooks, desktops e tablets perfeitamente funcionais obsoletos a cada três a cinco anos e alimentando montanhas de lixo eletrônico.
3. **Telemetria de Vigilância & Padrões Obscuros de Captura da Atenção:** Aplicativos de consumo e jogos comerciais são desenhados ao redor de telemetria comportamental, ciclos de compulsão de razão variável, feeds algorítmicos e microtransações predatórias que extraem a atenção humana e destroem a calma mental.
4. **Cadeias de Suprimento Opacas & Esgotamento do Mantenedor Solitário:** Infraestruturas críticas de código aberto frequentemente dependem de mantenedores solitários exaustos e não remunerados, enquanto *blobs* binários proprietários escondem *backdoors* e deriva de ponto flutuante não-determinística.

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Zero substitui a dependência da nuvem proprietária por seis capacidades digitais integradas:

1. **Auto-Hospedagem Autopoética da Pilha Soberana:** Compilações herméticas e reprodutíveis bit-a-bit em múltiplas arquiteturas (`x86_64`, `aarch64`, `riscv64`) de todos os daemons do nó, imagens de microcontroladores Zephyr RTOS e módulos determinísticos de fluxo de trabalho em WebAssembly (`WASM`).
2. **O SDK da Constelação & Suíte de Aplicações Conviviais:** Ferramentas de autoria para Lentes de Aplicação leves da Camada 6 (lousas da Cozinha Solidária, cadernos de campo agroflorestais, terminais de chão de fábrica, pagers de paramédicos) e aplicações colaborativas *local-first* baseadas em CRDT (CAD/CAM paramétrico, BIM, estações de áudio digital, estúdios vetoriais/raster e visualizadores clínicos).
3. **O Mandato do Hardware Resgatado de 20 Anos (`<= 256 MiB` OS, `<= 512 MiB` App):** Todo daemon do sistema e toda lente da Constelação são orçados para rodar dentro de $\le 256\,\mathrm{MiB}$ de RAM no sistema operacional base e $\le 512\,\mathrm{MiB}$ de arena de memória por aplicação, garantindo que notebooks ThinkPad de 2006, kits de Metareciclagem, Raspberry Pis e placas RISC-V permaneçam estações de trabalho de primeira classe por décadas.
4. **Oasis, Estúdios Soberanos de Jogos & *Modding* Sem Royalties:** Jogos eletrônicos, RPGs narrativos, sandboxes de física e o **Oasis** (simultaneamente um simulador recreativo de assentamento e o gêmeo digital sombra da `Camada 1b` do nó) são desenvolvidos como arte cultural celebrada em parceria com músicos e escritores do `Cenário Lambda`, com grafo de *mods* nativo e zero royalties de plataforma.
5. **O Diodo de Quarentena Detox (`l6_l7.sock`):** Um pipeline de ingestão em sala limpa que importa pacotes úteis de código aberto da Camada 7, arranca SDKs de telemetria, rastreadores de anúncios e *blobs* proprietários, substitui chamadas de nuvem por vínculos locais CRDT/malha e armazena o código-fonte desintoxicado no espelho offline local.
6. **Computação Termodinâmica & Escalonamento Solar:** Compilações pesadas de integração contínua (CI), *baking* de iluminação 3D, verificação formal (`Lean 4` / `TLA+`) e inferência local de modelos abertos são agendados automaticamente nos picos solares do microgrid (`Cenário Beta`), capturando $\ge 8.000\,\mathrm{bp}$ ($80\%$) do calor dos servidores via blocos hidrônicos de cobre a $50\text{–}60^\circ\mathrm{C}$ para aquecer os banhos comunitários do `Cenário Zeta` e as estufas do `Cenário Gama`.

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
Desenvolvedores, artistas, mantenedores e moradores coordenam a produção digital por meio de quatro intenções JSON-LD primárias:
* **`SoftwareOrGameProductionIntent`:** Comissionamento de uma nova lente de aplicação, patch de daemon, compilação de jogo ou lote de CI hermético, vinculando automaticamente a recuperação de calor hidrônico e os limites acústicos noturnos de dormitório ($\le 380\,\mathrm{cB}$).
* **`CommunityModPublicationIntent`:** Publicação de um *mod* criativo de jogo, extensão ontológica, pacote de formulários SHACL ou sandbox de simulação pedagógica (`Cenário Iota`) com atribuição criptográfica automática às dependências de base.
* **`SoftwareAgoraPackageManifest`:** Registro de um CID de pacote compilado de forma reprodutível e co-assinado por limiar (*threshold*) no catálogo descentralizado de software da malha.
* **`DrudgeryAutomationIntake` (em pareamento com o `Cenário Sigma`):** Recebimento de missões de alívio ergonômico vindas das guildas físicas para transformá-las em projetos abertos de firmware, automação e robótica convivial.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
O daemon `col-commonsd` expõe o comum de software por meio de grafos ontológicos autodescritivos em `https://w3id.org/collective/v1/`:
* **Rótulos Nutricionais de Capacidade (`AppCapabilityManifestV1`):** Cada Lente de Aplicação ou pacote de software traz um manifesto imposto por máquina, exibido em linguagem clara como um **Rótulo Nutricional de Capacidade** antes da instalação ou assinatura. Ele declara tetos exatos de memória ($\le 512\,\mathrm{MiB}$), verificação de zero ponto flutuante (`0` opcodes IEEE-754 em módulos determinísticos do Livro-Contábil), escopos RBAC solicitados (`intent:author`, `crdt:read`, etc.), garantias de isolamento de rede (`Acesso à VLAN 40: ZERO`) e resultados do teste canário de $\ge 10.000\text{ ticks}$ no sandbox (`0x01`).
* **DAGs Merkle de SBOM Endereçados por Conteúdo:** As Listas de Materiais de Software (`SBOM`) vinculam cada CID binário à sua árvore exata de código-fonte, hash de compilador e DIDs dos contribuidores de base, fazendo fluir automaticamente o reconhecimento não-financeiro de Riqueza Reputacional ($\mathcal{R}$) no Livro-Contábil para mantenedores de bibliotecas e artistas sempre que seu trabalho é utilizado.
* **Leitura Determinística Obrigatória de Lançamento:** Antes que os mantenedores co-assinem um lançamento com suas chaves físicas, a lente apresenta uma **Leitura Determinística (*Read-Back*)** destacando qualquer alteração em escopos de capacidade, consumo de memória ou permissões de soquete em relação à versão anterior.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela
A Camada 6 serve o ofício de software e jogos por meio de lentes especializadas que suprimem $91\%$ do ruído rotineiro de compilação e testes em segundo plano:

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Estúdio Forja Soberana (*Sovereign Forge Studio*)** | Notebook Resgatado / Estação (`forge:author`, `bpmn:review`) | IDE *local-first*, bancada de *mods* e construtor de lentes SHACL/JSON-LD; exibe o status ao vivo de reprodutibilidade multi-arquitetura Nix/Guix e agenda compilações pesadas no excedente solar da tarde. |
| **Registro de Rótulos Nutricionais de Capacidade** | Tablet / E-Ink / Desktop (`crdt:read`, `gov:ratify`) | Exibe os "Rótulos Nutricionais" de alto contraste para cada aplicativo e *mod*; permite inspecionar permissões e confirmar zero telemetria antes do pareamento via `#pair_token` na VLAN 30. |
| **Inspetor de Quarentena Detox** | Bancada de Mantenedor (`forge:detox`) | Visualizador *diff* lado a lado destacando endpoints de rastreamento da Camada 7 removidos, travas de licença em nuvem e chamadas de ponto flutuante substituídas antes de o pacote entrar no espelho offline. |
| **Sincronizador DTN de Pacotes via Triciclo** | Sem Tela / CLI / Doca E-Ink (`courier:dtn`) | Sincroniza lançamentos de múltiplos gigabytes, *assets* de jogos e imagens de SO através dos drives *sneakernet* nos triciclos de carga do `Cenário Beta` e enlaces de rádio Reticulum sem internet. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ pessoa atendida}$):**
   * *Contexto:* Um morador ou desenvolvedor operando um servidor doméstico de nó único (`col-storaged` + aplicativos locais da Constelação) num mini-PC ou ThinkPad resgatado.
   * *Modelo de Governança:* **Soberania pessoal completa com zero cerimônia.** O operador individual pode criar, fazer *fork* e implantar scripts, *mods* e lentes pessoais imediatamente; as travas automatizadas das Camadas 5 e 6 continuam protegendo-o ao exigir o Rótulo Nutricional de Capacidade, o teste canário sem ponto flutuante e o teto de silêncio noturno do quarto ($\le 380\,\mathrm{cB}$).
2. **Guardião Único / Núcleo Pequeno Atendendo um Círculo Maior ($N_s = 1\text{ ou pequeno núcleo}, N_p > 1\text{ pessoas atendidas}$):**
   * *Contexto:* Um ou dois administradores de sistema/desenvolvedores do bairro mantendo o servidor compartilhado e o catálogo de lentes da Constelação para um núcleo de $5\text{ a }15$ casas ($N_p > 1$).
   * *Modelo de Governança:*
     * **Isolamento de Privilégio Mínimo:** Como cada morador pareia seu dispositivo pessoal via concessões `AppCapabilityToken` de escopo restrito sobre TLS 1.3 na VLAN 30, o mantenedor local jamais tem acesso às notas privadas em $R_0$, prontuários de saúde ou dados familiares dos vizinhos.
     * **Ensaio Canário Antes da Produção:** Atualizações nas lentes compartilhadas do bairro precisam passar por um ensaio sombra determinístico de $10.000\text{ ticks}$ no `Oasis` (`0x01`) antes de entrar em produção, evitando que um ajuste feito tarde da noite quebre a lousa da cozinha ou a trava das marmitas na manhã seguinte.
     * **Escudo Anti-Martírio do Mantenedor:** Relatos de bugs e pedidos de melhoria do círculo $N_p$ entram numa fila calma *pull-only*; o `Cenário Sigma` bloqueia notificações urgentes para desejos não-críticos de software, garantindo que o programador do bairro nunca seja tratado como suporte técnico 24 horas.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Guilda Federada de Software e Jogos ($N_s = 10\text{ a }100+$ desenvolvedores, artistas e auditores) mantendo os daemons centrais da pilha, lentes da Constelação e estúdios de jogos através dos anéis $R_2 \dots R_5$.
   * *Modelo de Governança:*
     * **Co-Assinatura por Limiar entre Duas Guildas (`t-of-n` FROST):** Promover um daemon central ou lente padrão da Constelação para `/var/lib/collective/apps/` exige assinaturas físicas independentes tanto da **Guilda de Software** (verificando hashes de compilação reprodutível e limites de memória) quanto da **Guilda de Segurança/Epistêmica** (`Cenário Rô`/`Iota`).
     * **O Veto Criativo de Estúdio (`Protocolo de Recusa`):** Qualquer desenvolvedor, artista ou testador detém um veto arquitetural sem penalidade para barrar recursos de software ou jogos que introduzam padrões obscuros viciantes, rankings públicos de produtividade ou telemetria de vigilância.
     * **Liberdade de *Fork* & Propagação de Reputação via SBOM:** Criadores dissidentes podem fazer *fork* limpo de qualquer jogo ou lente no grafo CRDT; autores de bibliotecas de base e artistas do `Cenário Lambda` recebem automaticamente reconhecimento de Riqueza Reputacional ($\mathcal{R}$) no Livro-Contábil sempre que *forks* ou *mods* derivados são utilizados na malha.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Ingestão de Pacotes Upstream de Código Aberto:** Baixar compiladores, árvores de kernel Linux/Zephyr e bibliotecas científicas da internet pública.<br>**B. Publicação Comercial de Jogos, Apps e Consultoria de TI (`policy:commons_plus_l7_zero_bill`):** Vender edições binárias, trilhas sonoras ou pacotes de apoio do **Oasis** e de jogos/softwares independentes em lojas externas (Steam, GOG, itch.io) ou prestar serviços de desenvolvimento via **Cooperativa de Trabalho de TI (Lei 12.690/2012 e Ato Cooperativo Art. 79 da Lei 5.764/1971)** ou **Inova Simples** para quitar boletos e capitalizar o fundo de IPTU/ITR do nó. | • **Diodo Obrigatório de Quarentena Detox:** Nenhum código externo cruza o soquete `l6_l7.sock` para dentro da malha sem remoção automatizada de sinalizadores de telemetria, *blobs* proprietários e chamadas de nuvem, seguida de empacotamento offline local.<br>• **Desligamento Automático em $\Phi_t = \text{R\$ } 0,00$:** A receita externa entra na carapaça cooperativa do `Cenário Épsilon` exclusivamente para zerar as faturas mensais em Reais ($\Phi_t \to 0$) e capitalizar o Fundo Patrimonial de IPTU/ITR, enquanto o software permanece $100\%$ livre e gratuito dentro da malha.<br>• **Rastreamento de Dependência de Silício ($D_t$):** CPUs, FPGAs e SSDs importados são registrados no Índice de Dependência Legada do Livro-Contábil para guiar a metareciclagem (`Cenário Mi`) e a pesquisa de silício aberto. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Blindagem Copyleft & Anterioridade Defensiva (*Prior Art*):** Impedir captura corporativa ou *patent trolling* contra softwares, protocolos e motores gráficos d'O Coletivo.<br>**B. Segurança Operacional do Desenvolvedor (`Camuflagem Semântica Mundana` e `LGPD`):** Proteger contribuidores individuais contra *doxxing*, retaliação patronal ou assédio jurídico. | • **Licenciamento Recíproco Forte & Registro no INPI:** Publica lançamentos externos sob licenças **GNU AGPLv3** e **CERN-OHL-S v2** titularizadas pela cooperativa de software, com depósito criptográfico de anterioridade (*Prior Art*) junto ao **INPI (Lei da Propriedade Industrial nº 9.279/1996 e Lei de Software nº 9.609/1998)**.<br>• **Pseudonimato & Proteção LGPD (Lei 13.709/2018):** O `col-commonsd` remove todos os grafos internos `did:peer` de autoria antes de publicar espelhos públicos pelo `l6_l7.sock`, assinando pacotes externos apenas com a chave institucional do estúdio cooperativo. |
