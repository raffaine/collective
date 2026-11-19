# Cenário Gama: A Cozinha Comunitária, Agroecologia Keyline & Nutrição Incondicional (`SCN-GAMMA-KITCHEN`)

* **Identificador:** `SCN-GAMMA-KITCHEN`
* **Onda Horizontal:** Onda B (Sobrevivência Periurbana, Energia, Fabricação & Logística Semente)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 05):** [../../../../doc/podcast/pt-br/episodes/ep05_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep05_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Gama governa como um bairro ou assentamento **planta, colhe, fermenta, cozinha e partilha a nutrição diária**. Ele fecha o ciclo metabólico desde as hortas de quintal em curva de nível (*Keyline*), consórcios de leguminosas, pequenos plantéis de aves caipiras em silvipastoril rotacionado e adegas de cogumelos até as Cozinhas Solidárias de alta eficiência, despensas de lactofermentação e entrega de marmitas térmicas de inox (*tiffins*) na vizinhança.

Em seu coração está uma promessa constitucional simples: **a alimentação é um direito incondicional de existência ($B_{\mathrm{baseline}} = 10.000\,\mathrm{bp}$), jamais uma mercadoria precificada**. Seja o morador um mestre torneiro, uma mãe ou pai em recuperação pós-parto, uma criança, uma avó ou um hóspede recém-acolhido, refeições quentes, sazonais e densas em nutrientes (ancoradas desde o Dia 1 pelo *Piso do Caldo de Ervilha, Feijão com Arroz Integral, Mandioca e Pão de Fermentação Natural* e escalando para toda a riqueza da culinária biorregional brasileira) são partilhadas livremente com zero preço interno, zero tokens de refeição no Livro-Contábil e proteção criptográfica contra alergias alimentares e patógenos.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

No sistema agroalimentar industrial legado (Camada 7), a nutrição cotidiana está presa em quatro crises interligadas:

1. **Oligopólio dos Supermercados & Insegurança Calórica:** As famílias gastam de $20\%$ a $40\%$ de sua renda líquida em alimentos transportados por milhares de quilômetros em cadeias frias frágeis, onde a inflação especulativa transforma o prato de comida diário numa fonte constante de ansiedade financeira.
2. **A Exaustão dos Vinte Fogões Acesos às 20h & Desperdício Doméstico:** Todas as noites, dezenas de famílias exaustas na mesma rua compram, picam cebola, cozinham e lavam pilhas de panelas separadamente em fogões ineficientes—desperdiçando $30\%$ a $40\%$ das verduras compradas caras que apodrecem nas gavetas de geladeiras individuais e multiplicando por dez o gasto térmico e de trabalho doméstico.
3. **Esgotamento do Solo & Confinamento Animal com Grãos Humanos:** A monocultura industrial destrói o solo vivo e contamina nascentes com agrotóxicos e fertilizantes nitrogenados sintéticos, enquanto granjas industriais alimentam animais confinados com milho e soja perfeitamente comestíveis por seres humanos, misturados a antibióticos profiláticos.
4. **Riscos Opacos de Alergênicos & Patógenos:** O processamento industrial e refeitórios comerciais dependem de pranchetas de papel e cadeias opacas de ingredientes, onde uma única contaminação cruzada ou falha de temperatura provoca choque anafilático grave ou surto alimentar.

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Gama substitui a dependência alimentar industrial por seis laços agroculinários integrados:

1. **Piso Incondicional do Panelão & Artes Culinárias Plurais:** Garante refeições diárias nutritivas (caldos de ervilha e mandioquinha, feijão bem temperado com arroz integral e farofa de talos, pão rústico de fermentação natural *levain*, raízes assadas, caldos de ossos ou cogumelos, verduras frescas e fermentados sazonais) a preço interno exatamente zero (`domesticCareTokenMinting: 0`), enquanto celebra as diversas tradições culinárias regionais (`Cenário Lambda`) e disponibiliza ingredientes para quem desejar cozinhar em casa.
2. **Hortas Keyline & Consórcios de Leguminosas Fixadoras de Nitrogênio (*Fabaceae*):** Canteiros biodiversos em curva de nível consorciando *Fabaceae* fixadoras de nitrogênio (feijão-guandu, ervilha, fava, feijão-de-porco em treliças de bambu e eucalipto feitas no `Cenário Alfa`) com mandioca, batata-doce, couve, abóbora, quiabo, taioba e bananeiras irrigados por gravidade com água de chuva e jardins filtrantes de águas cinzas (`Cenário Beta`).
3. **A Lei Constitucional da Não-Competição Trófica (`human_edible_grain_fed_mg == 0`):** Galinhas caipiras, patos e pequenos ruminantes pastam estritamente em piquetes rotacionados de pomar e agrofloresta ($\ge 30\text{ dias}$ de repouso do pasto, `Cenário Eta`), aparas vegetais pré-consumo da cozinha, **larvas de Mosca-Soldado-Negra (`BSFL`)** cultivadas sobre resíduos orgânicos e grit de cálcio de ossos calcinados—com **zero miligramas de grãos comestíveis por humanos** (`human_edible_grain_fed_mg == 0`).
4. **Fermentação de Precisão, Conservas em Autoclave `316L` & Despensas Frescas:** Potes cerâmicos de lactofermentação (chucrute, picles de maxixe e cenoura, missô de feijão, tempeh), autoclaves de pressão em aço inox `316L`, desidratadores solares e despensas semienterradas que transformam picos de safra em segurança alimentar para o ano inteiro.
5. **Segurança Termodinâmica de Pasteurização de Bigelow & Acidificação Imposta em Hardware:** Travas físicas de temperatura e pH impedem que qualquer lote seja liberado para distribuição até que a letalidade de patógenos seja matematicamente comprovada pelos sensores da Camada 2.
6. **Marmitas Inox com Trava Zero-Knowledge contra Alergias & Retorno em Ciclo Fechado:** Marmitas térmicas retornáveis de inox `304` (*tiffins*) com chip passivo NFC circulam nas rotas *milk-run* de triciclo do `Cenário Beta`, devolvendo os recipientes enxaguados e os restos orgânicos pós-refeição para a compostagem termofílica a $55^\circ\mathrm{C}$ do `Cenário Mi` e para a bioconversão por `BSFL`.

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`DietaryNeedIntent`:** Registro da quantidade diária de porções de uma casa, preferências culinárias e máscaras cegas em zero-knowledge de restrições alérgicas ou éticas/religiosas (ex.: amendoim, castanha-de-caju, doença celíaca/glúten, crustáceos, gergelim, vegano, Halal, Kosher).
* **`MealOffering`:** Publicação de um lote cozido na Cozinha Solidária ou excedente de despensa com sua árvore recursiva de ingredientes (`recipe_bom`), atestado verificado de pasteurização/pH e `domesticCareTokenMinting: 0`.
* **`AgroPastoralBatchAttestation`:** Registro da proveniência semente-à-colheita do canteiro, qualidade da água de irrigação e conformidade do pastoreio rotacionado com zero grão humano.
* **`CompostRoutingIntent`:** Direcionamento de cascas, ossos de caldo e águas cinzas de volta às caixas de larvas `BSFL`, calcinação de cinza óssea e leiras de compostagem quente do `Cenário Mi`.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Grafos Estequiométricos Cultivar-à-Receita (`https://w3id.org/collective/v1/`):** Vinculam diretamente cultivares botânicos (`fabaceae_cultivars.jsonld`) e balanços de umidade/nutrientes do solo às árvores de ingredientes das receitas (`pea_to_soup_needs.jsonld`), permitindo planejar o plantio sazonal conforme a necessidade nutricional real do bairro.
* **Invariantes de Letalidade Térmica $F_0$ de Bigelow & pH contra Botulismo:** O `col-commonsd` e a Camada 4 bloqueiam a transição de um lote para `DISPENSE_READY` a menos que as sondas PT100 da Camada 2 atestem temperatura central $\ge 347.150\,\mathrm{mK}$ ($74{,}0^\circ\mathrm{C}$) continuamente por $\ge 120.000\,\mathrm{ms}$ (ou $\ge 394.150\,\mathrm{mK}$ / $121{,}0^\circ\mathrm{C}$ para conservas de baixa acidez em autoclave)—zerando automaticamente o cronômetro se a temperatura cair—e comprovem conservas lactofermentadas em $\le 4.600\,\mathrm{mpH}$ ($\mathrm{pH} \le 4{,}6$).
* **Prova Zero-Knowledge `BBS+` de Disjunção de Conjuntos contra Anafilaxia:** Nenhum morador precisa expor diagnósticos médicos num quadro público da cozinha. Ao encostar o cartão NFC no armário de marmitas da varanda, o microcontrolador local avalia em tempo $O(1)$ uma prova `BBS+` de disjunção contra a árvore recursiva `recipe_bom` da marmita:
  $$(\mathtt{batch\_mask} \mathbin{\&} \mathtt{diner\_exclusion\_mask}) == 0$$
  Qualquer interseção de alérgeno aciona uma **Parada Dura Mecânica contra Anafilaxia (*Anaphylactic Hard-Stop*)**, mantendo a trava solenoide fechada.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela
Numa cozinha comunitária madura executando de $96\text{ a }480$ processos biológicos e térmicos simultâneos (canteiros escalonados, caldos de 24 horas, queda de pH em fermentados e higienização de marmitas), a Camada 6 condensa as verificações rotineiras em **uma caminhada matinal de 20 minutos pela horta e um turno vespertino calmo de preparo**:

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Lousa E-Ink Lavável `IP67` da Cozinha & Despensa** | Fixada ao lado dos Caldeirões de Indução (`hearth:steward`, `bpmn:review`) | Tela de alto contraste à prova de vapor e mãos molhadas exibindo cronômetros de etapa, medidores ao vivo de letalidade térmica $F_0$ de Bigelow e verificação de limpeza `CIP` em inox `316L`. |
| **Caderno de Campo Keyline & Pequenos Plantéis** | Tablet Offline Domínio-B (`agro:cultivar`, `crdt:read`) | Agrupa alertas de umidade do solo, colheita nas treliças de *Fabaceae*, alimentação de larvas `BSFL` e rotações de 30 dias do pasto em uma única caminhada matinal. |
| **Armário de Marmitas Inox com Trava NFC Zero-Knowledge** | Varanda / Vestíbulo da Cozinha (`tiffin:dispense`) | Trava solenoide sem tela ou com mini E-Ink: encoste o cartão para destravar uma marmita quente e segura; pisca alerta vermelho Zero-LLM e permanece trancada se houver conflito alérgico. |
| **Etiqueta QR Lavável de Rastreio de Lote** | Presa aos Potes de Fermentação & Caldeirões | Etiqueta de aço inox ou polímero lavável presa aos vasos para leitura rápida de marcos de pH e temperatura sem tocar em telas. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ pessoa atendida}$):**
   * *Contexto:* Um morador solitário cuidando de sua horta de quintal, 4 galinhas poedeiras e uma despensa pessoal de conservas e fermentados.
   * *Modelo de Governança:* **Zero reuniões, puro companheiro culinário e de segurança alimentar.** A Camada 6 organiza a rega e a checagem dos fermentados numa ronda diária tranquila enquanto impõe travas físicas de segurança na autoclave ($\ge 394.150\,\mathrm{mK}$) e no pH das conservas ($\le 4.600\,\mathrm{mpH}$) para garantir segurança alimentar o ano inteiro sem risco de deterioração.
2. **Guardião Único / Núcleo Pequeno Atendendo um Círculo Maior ($N_s = 1\text{ ou pequeno núcleo}, N_p > 1\text{ pessoas atendidas}$):**
   * *Contexto:* Um ou dois cozinheiros/agricultores do bairro preparando caldeirões de sopa, feijão e marmitas $2\text{ a }3$ dias por semana para a família extensa, uma avó vizinha (`Cenário Fi`), uma família em Casulo Pós-Parto (`Cenário Delta`) ou $5\text{ a }12$ casas pareadas da rua ($N_p > 1$).
   * *Modelo de Governança:*
     * **Escudo Automatizado contra Alergias e Responsabilidade Sanitária:** Ao cozinhar para $N_p$ vizinhos, o cozinheiro não precisa decorar 30 restrições alimentares individuais—o `recipe_bom` é registrado uma única vez na Lousa da Cozinha, e as travas NFC das marmitas impedem automaticamente qualquer vizinho alérgico de abrir uma marmita incompatível.
     * **Sem Servidão na Cozinha:** Os pedidos de porções (`DietaryNeedIntent`) são agregados antes do início do preparo, a lavagem das panelas/marmitas e a capina da horta rodam entre todo o círculo $N_p$ via Mutirão Rotativo (`Cenário Sigma`), e se o cozinheiro tocar em `Descansar Agora`, o `col-commonsd` transfere o atendimento para a despensa pronta ou para a cozinha solidária vizinha.
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Cozinha Solidária e Guilda Agroecológica completa ($N_s = 8\text{ a }50+$ guardiões) nutrindo de $150\text{ a }2.500+$ moradores e trocando excedentes sazonais entre nós da bacia hidrográfica ($R_2 \dots R_4$).
   * *Modelo de Governança:*
     * **Piso Ecológico e Nutricional Bicameral:** A **Câmara dos Comuns** ($N_p$ moradores) ratifica as cláusulas pétreas éticas e ecológicas—como a proibição absoluta de alimentar animais com grãos humanos (`human_edible_grain_fed_mg == 0`), zero plásticos descartáveis, zero agrotóxicos e variedade sazonal mínima—enquanto a **Câmara de Guardiões** ($N_s$ cozinheiros e agricultores) governa a rotação de culturas, a sanitização `CIP` em inox `316L` e a arte culinária.
     * **Compensação Sazonal Entre Nós (`BCC`):** Excedentes sazonais de mandioca, feijão seco, frutas ou azeite são compensados entre vales vizinhos por meio de Canais de Compensação Bilateral (`BCC`) de soma zero no Livro-Contábil e rotas *milk-run* refrigeradas do `Cenário Beta`, enquanto todas as refeições internas permanecem $100\%$ gratuitas (`0` tokens).

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Compra Coletiva Atacadista de Secos e Minerais (`ProcurementIntent`):** Reunir pedidos do bairro no atacado (`GPO`) para sal marinho, especiarias, azeite ou grãos secos ainda não produzidos com $100\%$ de autossuficiência local.<br>**B. Venda Transitória de Excedentes Artesanais e Institucionais (`policy:commons_plus_l7_zero_bill`):** Vender pães de fermentação natural, conservas, doces e hortaliças excedentes em feiras locais (com **Selo ARTE — Lei 13.680/2018**) ou para a merenda escolar pública via **PNAE e PAA (Lei Federal 11.947/2009 — mínimo de 30% da agricultura familiar)** para quitar contas de água, energia ou IPTU. | • **Rastreamento de Dependência Calórica e de Micronutrientes ($D_t$):** Todo insumo alimentar importado é registrado por massa e exergia calórica no Índice de Dependência Legada do Livro-Contábil, guiando a expansão de pomares agroflorestais (`Cenário Eta`) e sinalizadores de crescimento agrícola (`Cenário Pi`).<br>• **Prioridade Absoluta ao Piso Interno ($F_{\mathrm{eco}} + B_{\mathrm{baseline}}$) & Desligamento Automático em $\Phi_t = \text{R\$ } 0,00$:** A Camada 5 proíbe exportar um único grama de alimento para a Camada 7 até que $100\%$ das marmitas dos moradores estejam garantidas, e **desliga automaticamente as vendas comerciais externas** no exato instante em que os boletos mensais da Camada 7 atingem $\text{R\$ } 0{,}00$. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Blindagem da Cozinha Solidária, `GCR`/`CSA` & Agricultura Urbana:** Proteger a partilha de marmitas entre vizinhos e as hortas comunitárias contra assédio fiscal ou exigências burocráticas desenhadas para redes industriais de restaurantes.<br>**B. Privacidade Alimentar Familiar (`Camuflagem Semântica Mundana` e `LGPD`):** Impedir que redes varejistas ou seguradoras perfilem dietas religiosas ou alergias médicas dos moradores. | • **Carapaça da Lei das Cozinhas Solidárias (Lei 14.628/2023), Art. 5º XVIII CF/88 & RDC ANVISA 49/2013:** Enquadra a partilha interna de refeições como clube alimentar associativo sem fins lucrativos e Cozinha Solidária (`0` vendas comerciais dentro da malha), amparada pela **RDC ANVISA nº 49/2013** (inclusão produtiva e produção artesanal/comunitária) e pelo **Ato Cooperativo (Lei 5.764/1971)** nas operações externas.<br>• **Anonimização de Compras Coletivas (`LGPD Lei 13.709/2018`):** Pedidos de atacado removem todos os perfis domésticos `DietaryNeedIntent`, emitindo apenas ordens agregadas de insumos pelo `l6_l7.sock`. |
