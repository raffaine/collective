# Cenário Zeta: Moradia Desmercantilizada em Termo Territorial Coletivo & Casas de Banhos Regenerativas (`SCN-ZETA-LODGE`)

* **Identificador:** `SCN-ZETA-LODGE`
* **Onda Horizontal:** Onda C (Cuidado, Parentesco, Aprendizagem & Moradia Desmercantilizada)
* **Versão em Inglês:** [../README.md](../README.md)
* **Especificação Full-Stack Versionada:** [../versions/v1_full_stack_spec.md](../versions/v1_full_stack_spec.md)
* **Registro de Decisões Arquiteturais:** [../CHANGELOG.md](../CHANGELOG.md)
* **Dossiê-Fonte da Rádio Lambda Brasil (Ep. 06):** [../../../../doc/podcast/pt-br/episodes/ep06_source_pack.md](../../../../doc/podcast/pt-br/episodes/ep06_source_pack.md)

---

## 1. Escopo Pretendido & Visão de Domínio

O Cenário Zeta governa **como o bairro ou assentamento abriga seus moradores permanentes, acolhe visitantes de nós pares e hóspedes em situação de crise, e mantém Casas de Banhos regenerativas de hidroterapia térmica**. Enquanto o `Cenário Ksi` cuida da engenharia estrutural e da construção em madeira de encaixe *Kigumi*, o Cenário Zeta governa a **guarda viva da moradia desmercantilizada, os pisos domésticos de energia e conforto térmico, a hospitalidade e a recuperação somática nas Casas de Banhos**.

Seus princípios centrais são inegociáveis:
* **Moradia Desmercantilizada (`0` Aluguel Interno no Livro-Contábil):** A terra e as edificações são retiradas permanentemente do mercado especulativo por meio do **Termo Territorial Coletivo (`TTC — Lei Federal nº 13.465/2017`)** e da **Concessão de Direito Real de Uso Coletivo (`CDRU — Decreto-Lei nº 271/1967`)**, garantindo a cada morador um lar digno e seguro e um piso diário de $43{,}2\,\mathrm{MJ/dia}$ ($12{,}0\,\mathrm{kWh/dia}$) de conforto elétrico e térmico como parte de $B_{\mathrm{baseline}} = 10.000\,\mathrm{bp}$.
* **Casas $R_0$ 100% Sem Telas & Sem Vigilância (`R0_INTERIOR_DARK`):** O recurso mais importante da Camada 6 dentro de uma moradia privada é **a sua completa ausência**—zero câmeras, zero caixas de som "inteligentes", zero sensores de presença e zero painéis brilhantes na parede dentro das suítes privadas $R_0$.
* **Casas de Banhos Hidrônicas Regenerativas & Saneamento em Ciclo Fechado:** Captura do calor residual dos servidores (`Zero`/`Épsilon`), da cozinha (`Gama`) e da fundição (`Teta`), somada a serpentinas de cobre de recuperação de calor de água de dreno (`DWHR` $\ge 5.500\,\mathrm{bp}$ / $55\%$), para aquecer piscinas de imersão a $60^\circ\mathrm{C}$, saunas de cedro e lavanderias comunitárias, integradas a câmaras de compostagem com biocarvão sob pressão negativa ($\le -25\,\mathrm{Pa}$, $100\%$ sem odor) e jardins filtrantes de 72 horas.

---

## 2. O Panorama Atual (Camada 7) & Seus Problemas Estruturais

No mercado imobiliário e de hospedagem legado (Camada 7), o abrigo humano é deformado por quatro patologias estruturais:

1. **A Servidão do Aluguel e do Financiamento Imobiliário ($30\%$ a $60\%$ da Vida Humana):** Fundos especulativos e rentistas tratam o abrigo humano básico como ativo financeiro—obrigando famílias trabalhadoras a entregarem metade de sua vida desperta apenas para pagar aluguel ou juros bancários, sob ameaça constante de despejo.
2. **Vigilância Doméstica da "Casa Inteligente" (*Smart Home*):** Termostatos Wi-Fi, câmeras de campainha, Smart TVs e assistentes de voz corporativos transformam a sala e o quarto da família num grampo permanente que mapeia rotinas íntimas, conversas e horários de sono.
3. **Turistificação Predatória de Curta Temporada (*Airbnbização*):** Plataformas comerciais de locação por temporada esvaziam bairros residenciais, destroem os laços de vizinhança e expulsam moradores locais.
4. **Desperdício Termodinâmico e Sanitário:** Edifícios convencionais dão descarga no vaso sanitário com água potável tratada e jogam pelo ralo toda a energia térmica da água quente do chuveiro sem nenhuma recuperação de calor ou nutrientes.

---

## 3. Horizonte Amplo de Funcionalidades

O Cenário Zeta substitui a extração de aluguel e a vigilância doméstica por cinco laços conviviais de moradia:

1. **Guarda Residencial Desmercantilizada via `TTC` / `CDRU` (`HousingStewardshipIntent`):** Os moradores detêm posse segura, vitalícia e renovável de sua casa, apartamento ou suíte familiar em $R_0$ (`Cenário Ômicron`) com zero aluguel interno, zero juros hipotecários (`domesticCareTokenMinting: 0`), permutas adaptativas conforme a família cresce ou envelhece, e um piso incondicional de $43{,}2\,\mathrm{MJ/dia}$ ($12\,\mathrm{kWh/dia}$) de conforto elétrico e hidrônico.
2. **Santuário Interior $R_0$ 100% Sem Telas (`R0_INTERIOR_DARK`):** Toda coordenação digital compartilhada da Camada 6 para do lado de fora, no **Quiosque E-Ink do Vestíbulo do Pátio**. Dentro das casas privadas $R_0$, os moradores controlam a iluminação quente, o aquecimento hidrônico e a ventilação por meio de interruptores táteis de latão pesado, válvulas termostáticas mecânicas e o ferrolho de madeira do `Cenário Sigma`—com **zero sensores de rede no interior**.
3. **Hospitalidade Federada & Suítes de Refúgio de Crise Sem Crachá (`LodgingIntent`):** Mantém quartos acolhedores para artesãos visitantes dos anéis $R_2\text{–}R_5$, pesquisadores (`Cenário Iota`) e viajantes em mutirão, ao lado da reserva permanente ($\ge 500\,\mathrm{bp}$ / $5\%$) de suítes de refúgio não-monitoradas para o acolhimento de crise Caminho 4 (`Cenário Tau`) e para saídas unilaterais de Porto Seguro em $< 500\,\mathrm{ms}$ (`Cenário Ômicron`).
4. **Casas de Banhos Regenerativas Aquecidas por Calor Residual (`DWHR` $\ge 5.500\,\mathrm{bp}$):** As Casas de Banhos comunitárias combinam serpentinas de cobre de recuperação de calor de dreno ($\ge 55\%$ de captura térmica) com a água quente ($55\text{–}65^\circ\mathrm{C}$) vinda dos blocos de resfriamento líquido dos servidores do `Cenário Zero`/`Épsilon` para aquecer saunas de madeira, banhos de imersão e a lavanderia comunitária a ozônio—oferecendo recuperação somática diária aos guardiões (`Cenário Sigma`).
5. **Jardins Filtrantes de 72 Horas & Câmaras de Biocarvão a Pressão Negativa ($\le -25\,\mathrm{Pa}$):** As águas cinzas de chuveiros e lavanderia passam por jardins filtrantes subsuperficiais de bananeiras, taiobas e papiros (*Constructed Wetlands*) com Tempo de Retenção Hidráulica (`HRT`) verificado em hardware de $\ge 72\text{ horas}$ antes de irrigar os pomares do `Cenário Eta`, enquanto sanitários secos ou de microespuma alimentam câmaras subterrâneas de biocarvão e estruvita mantidas sob pressão negativa contínua ($\le -25\,\mathrm{Pa}$, $100\%$ sem odor) rumo à biorrefinaria do `Cenário Mi`.

---

## 4. A Superfície da Camada 6: Necessidades, Intenções, Representações Semânticas & Lentes de Aplicação

### A. Necessidades Humanas & Intenções Canônicas
* **`HousingStewardshipIntent`:** Solicitação de alocação de moradia residente em $R_0$, permuta adaptativa pelo ciclo da vida (ex.: quando nasce um bebê no `Cenário Delta` ou quando um idoso prefere uma suíte térrea sem degraus no `Cenário Fi`) ou manutenção externa do telhado/fachada.
* **`LodgingIntent`:** Reserva de quarto de hóspedes para um guardião visitante de nó irmão ou alocação imediata de uma suíte de santuário de crise Zero-ID (`domesticCareTokenMinting: 0`).
* **`BathhouseAndLaundrySlotIntent`:** Consulta de horários de silêncio, reserva de janela privada/familiar de hidroterapia na Casa de Banhos ou uso da lavanderia comunitária a ozônio e bomba de calor.
* **`SkillOfferingBarterIntent` (Hospitalidade Cultural Opcional):** Permite que um viajante ou músico visitante (`Cenário Lambda`) ofereça voluntariamente uma roda de violão, oficina ou palestra durante sua estadia—estritamente desvinculada de seu direito incondicional a cama limpa e prato de comida quente.

### B. Representações Semânticas & Leituras Determinísticas (*Read-Backs*)
* **Fronteira Arquitetônica da Não-Representação (`R0_INTERIOR_DARK`):** Em `https://w3id.org/collective/v1/`, o `col-commonsd` impõe uma restrição SHACL inquebrável proibindo qualquer tópico de sensor, câmera, microfone ou estado de ocupação dentro de uma unidade residencial $R_0$ ativa. Apenas métricas agregadas na borda externa do edifício (geração solar do telhado, nível da cisterna principal, `HRT` de 72h do jardim filtrante e pressão negativa $\le -25\,\mathrm{Pa}$ do exaustor da câmara de composto) existem no grafo de conhecimento e no Livro-Contábil.
* **Leitura Determinística Obrigatória de Permissão de Entrada para Reparo:** Quando um morador assina um `HousingStewardshipIntent` ou solicita que uma dupla de manutenção do `Cenário Alfa`/`Ksi` entre em sua casa para consertar um encanamento, o Quiosque do Vestíbulo exige uma **Leitura Determinística (*Read-Back*)** especificando a janela exata de horário em que o morador autoriza a entrada física temporária em seu limiar $R_0$.

### C. Constelação de Lentes de Aplicação & Modalidades Sem Tela

| Lente de Aplicação / Modalidade | Contexto-Alvo & Escopo RBAC | Experiência Humana & Atenuação de Variedade |
| :--- | :--- | :--- |
| **Quiosque E-Ink do Vestíbulo de Hospitalidade & Casa de Banhos** | Pátio Externo / Entrada da Casa de Banhos (`zeta:hospitality`) | Lousa E-Ink calma instalada fora das casas privadas: gerencia check-in de hóspedes, horários da Casa de Banhos, avisos de término da lavanderia e chamados de manutenção predial. |
| **Interruptores de Latão & Válvulas Hidrônicas Sem Tela em $R_0$** | Interior das Casas Privadas (`r0:mechanical_only`) | Controles domésticos puramente analógicos e táteis (interruptores de latão pesado, registros térmicos manuais e o ferrolho de madeira do `Cenário Sigma`) com zero telas brilhantes ou microfones. |
| **Dispensador de Chaves de Latão Zero-ID para Crises** | Vestíbulo Perimetral de Acolhimento (`R0_GUEST`) | Entrega uma chave física de latão para uma suíte de refúgio aquecida e abastecida para acolhimento de crise Caminho 4 (`Cenário Tau`) ou saída unilateral (`Cenário Ômicron`) em $< 500\,\mathrm{ms}$ sem pedir identificação. |
| **Manômetro Diferencial do Jardim Filtrante & Câmara de Biocarvão** | Subsolo Técnico / Estufa (`bpmn:review`) | Condensa mais de 290 verificações térmicas, `DWHR` ($\ge 5.500\,\mathrm{bp}$), `HRT` de 72h de águas cinzas e pressão negativa $\le -25\,\mathrm{Pa}$ em uma única ronda técnica semanal de 15 minutos. |

---

## 5. Governança através das Escalas Humanas ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Escala do Guardião Solitário ($N = 1 \implies N_s = 1\text{ operador}, N_p = 1\text{ morador}$):**
   * *Contexto:* Um morador solitário vivendo em sua casa ou sítio semente com chuveiro solar/casa de banhos e jardim filtrante de águas cinzas.
   * *Modelo de Governança:* **Tranquilidade doméstica completa.** O nó mantém silenciosamente a reserva térmica e elétrica de $43{,}2\,\mathrm{MJ/dia}$, monitora a pressão negativa do exaustor da câmara de compostagem ($\le -25\,\mathrm{Pa}$) e só alerta o morador se uma bomba ou válvula física precisar de atenção.
2. **Guardião Único / Núcleo Pequeno Atendendo um Círculo Maior ($N_s = 1\text{ ou pequeno núcleo}, N_p > 1\text{ moradores & hóspedes}$):**
   * *Contexto:* Um núcleo de co-habitação mantendo uma casa multifamiliar, edícula de quintal e sauna/casa de banhos compartilhada para $6\text{ a }20$ moradores e visitantes ($N_p > 1$).
   * *Modelo de Governança:*
     * **Zero Poder de Senhorio / Proprietário:** Como o imóvel é regido por um pacto associativo/`TTC` não-extrativo com $0$ aluguel interno, o guardião de manutenção predial detém **zero poder de despejo** e **zero direito de entrada sem aviso** na suíte privada $R_0$ de qualquer co-morador.
     * **Cuidado Rotativo da Casa de Banhos & Áreas Comuns:** A limpeza do vestíbulo, a reposição de toalhas de algodão (`Cenário Ni`) e a inspeção dos filtros do jardim de bananeiras rodam entre os moradores via Mutirões Rotativos do `Cenário Sigma` (`domestic_care_token_minting == 0`).
3. **Guilda Multi-Guardiões Atendendo uma Comunidade & Biorregião ($N_s > 1\text{ guardiões}, N_p > 1\text{ comunidade & nós pares}$):**
   * *Contexto:* Uma Guilda de Habitação e Hospitalidade do assentamento ($N_s = 10\text{ a }50+$ guardiões) cuidando de pátios residenciais sob `TTC`, Casas de Banhos e pousadas comunitárias federadas através dos anéis $R_1 \dots R_4$.
   * *Modelo de Governança:*
     * **Alocação Adaptativa Baseada na Necessidade (Zero Leilão de Preços):** As moradias disponíveis são alocadas pela necessidade transparente da família (tamanho familiar, acessibilidade térrea para idosos do `Cenário Fi`, prioridade absoluta para refúgio `SafeHarborSeveranceIntent` do `Cenário Ômicron`) e jamais por riqueza financeira ou leilão de reputação.
     * **Hospitalidade Recíproca Entre Nós ($R_2\text{–}R_4$):** Guardiões visitantes em mutirões do `Cenário Pi`, intercâmbios do `Cenário Capa` ou caravanas culturais do `Cenário Lambda` desfrutam de hospedagem recíproca gratuita entre todos os nós pareados da bacia hidrográfica.

---

## 6. Governando o Alcance na Camada 7 (Extrativo Transitório vs. Puramente Defensivo)

| Modo de Alcance | Propósito de Governança & Gatilho | Controles Constitucionais & Cláusulas de Desligamento Automático |
| :--- | :--- | :--- |
| **1. Alcance Extrativo / Transitório na Camada 7** | **A. Receita Transitória de Hospedagem Ecológica / Retiros (`policy:commons_plus_l7_zero_bill`):** Nas Fases 1 e 2, disponibilizar um pequeno conjunto perimetral de cabanas de hóspedes para visitantes externos ou retiros educativos a fim de quitar parcelas residuais da terra e impostos externos.<br>**B. Aquisição de Componentes Hidráulicos e Hidrônicos:** Importar geomembranas EPDM para jardins filtrantes, tubos de cobre para serpentinas `DWHR` ou exaustores CC brushless de baixíssimo consumo. | • **Reserva Estrita de Santuário & Desligamento Automático em $\Phi_t = \text{R\$ } 0,00$:** Reservas externas pagas só são permitidas após $100\%$ da moradia dos residentes e a reserva mínima de $\ge 500\,\mathrm{bp}$ ($5\%$) de suítes de refúgio de Porto Seguro estarem garantidas, e **fecham-se automaticamente** a cada mês no instante em que os boletos externos ($\Phi_t$) batem $\text{R\$ } 0{,}00$ e o Fundo Patrimonial de IPTU/ITR (`Cenário Ômega`) está capitalizado.<br>• **Segregação de Visitantes na `VLAN 40`:** Hóspedes externos permanecem na `VLAN 40` isolada, sem acesso aos pátios residenciais internos $R_0/R_1$. |
| **2. Alcance Puramente Defensivo na Camada 7** | **A. Bloqueio Antiespeculativo Irrevogável da Terra (`TTC` / `CDRU` / `Bem de Família`):** Impedir que qualquer facção futura, credor externo ou processo judicial desmembre ou venda as moradias comunitárias no mercado imobiliário especulativo.<br>**B. Blindagem Tributária e Sanitária Ecológica (`Tema 174 STJ`, `IPTU Verde` e `Camuflagem Semântica Mundana`):** Proteger as co-habitações, jardins filtrantes e isenções fiscais do território. | • **Termo Territorial Coletivo (`TTC — Lei Federal nº 13.465/2017`), `CDRU (Decreto-Lei nº 271/1967)` & Impenhorabilidade do Bem de Família (`Lei 8.009/1990` e `Art. 6º CF/88`):** Trava a propriedade da terra permanentemente fora do mercado especulativo sob titularidade coletiva, garantindo o direito fundamental de moradia.<br>• **Enquadramento em `ITR` Agroecológico (`Tema 174 do STJ` / `Decreto-Lei 57/66`), `RPPN` & `IPTU Verde`:** Aplica a jurisprudência consolidada do STJ (incidência de `ITR` — isento nas parcelas de `RPPN` do `Cenário Eta` — em vez de `IPTU` urbano para imóveis com destinação agroecológica, além de descontos de `IPTU Verde` por captação de chuva e jardins filtrantes `ABNT NBR 13969`), removendo listas de moradores antes de cruzar o `l6_l7.sock`. |
