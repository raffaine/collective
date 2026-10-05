import os, re

bib_db = {
    "prigogine1977time": "Prigogine, I. (1977). \\textit{Time, structure and fluctuations}. Nobel lecture, 8, 1977.",
    "dowling2012mechanical": "Dowling, N. E. (2012). \\textit{Mechanical behavior of materials}. Pearson.",
    "gordon2003structures": "Gordon, J. E. (2003). \\textit{Structures: or why things don't fall down}. Da Capo Press.",
    "balanis2015antenna": "Balanis, C. A. (2015). \\textit{Antenna theory: analysis and design}. John Wiley \& Sons.",
    "davies1990induction": "Davies, J. (1990). \\textit{Induction heating handbook}. McGraw-Hill.",
    "landauer1961irreversibility": "Landauer, R. (1961). \\textit{Irreversibility and heat generation in the computing process}. IBM journal of research and development, 5(3), 183-191.",
    "bennett1982thermodynamics": "Bennett, C. H. (1982). \\textit{The thermodynamics of computation}. International Journal of Theoretical Physics, 21(12), 905-940.",
    "seifert2012stochastic": "Seifert, U. (2012). \\textit{Stochastic thermodynamics, fluctuation theorems and molecular machines}. Reports on progress in physics, 75(12), 126001.",
    "fredkin1982conservative": "Fredkin, E., \& Toffoli, T. (1982). \\textit{Conservative logic}. International Journal of Theoretical Physics, 21(3), 219-253.",
    "bicerano2002prediction": "Bicerano, J. (2002). \\textit{Prediction of polymer properties}. CRC Press.",
    "mackay2008sustainable": "MacKay, D. J. (2008). \\textit{Sustainable energy-without the hot air}. UIT Cambridge.",
    "crank1979mathematics": "Crank, J. (1979). \\textit{The mathematics of diffusion}. Oxford university press.",
    "tillman1981wood": "Tillman, D. A. (1981). \\textit{Wood as an energy resource}. Academic Press.",
    "maturana1980autopoiesis": "Maturana, H. R., \& Varela, F. J. (1980). \\textit{Autopoiesis and cognition: The realization of the living}. Springer Science \& Business Media.",
    "szargut1988exergy": "Szargut, J., Morris, D. R., \& Steward, F. R. (1988). \\textit{Exergy analysis of thermal, chemical, and metallurgical processes}. Hemisphere Publishing Corporation.",
    "shockley1961detailed": "Shockley, W., \& Queisser, H. J. (1961). \\textit{Detailed balance limit of efficiency of p-n junction solar cells}. Journal of applied physics, 32(3), 510-519.",
    "laine2010efficient": "Laine, S., \& Karras, T. (2010). \\textit{Efficient sparse voxel octrees}. IEEE Transactions on Visualization and Computer Graphics, 17(8), 1048-1059.",
    "wolfram1983statistical": "Wolfram, S. (1983). \\textit{Statistical mechanics of cellular automata}. Reviews of modern physics, 55(3), 601.",
    "kinsler2000fundamentals": "Kinsler, L. E., Frey, A. R., Coppens, A. B., \& Sanders, J. V. (2000). \\textit{Fundamentals of acoustics}. John Wiley \& Sons.",
    "stauffer2014introduction": "Stauffer, D., \& Aharony, A. (2014). \\textit{Introduction to percolation theory}. Taylor \& Francis.",
    "incropera2007fundamentals": "Incropera, F. P., Lavine, A. S., Bergman, T. L., \& DeWitt, D. P. (2007). \\textit{Fundamentals of heat and mass transfer}. John Wiley \& Sons.",
    "lasseter2002microgrids": "Lasseter, R. H. (2002). \\textit{Microgrids}. In 2002 IEEE Power Engineering Society Winter Meeting. Conference Proceedings (Vol. 1, pp. 305-308). IEEE.",
    "dijkstra1965solution": "Dijkstra, E. W. (1965). \\textit{Solution of a problem in concurrent programming control}. Communications of the ACM, 8(9), 569.",
    "harris1913how": "Harris, F. W. (1913). \\textit{How many parts to make at once}. Factory, The Magazine of Management, 10(2), 135-136.",
    "laidler1987chemical": "Laidler, K. J. (1987). \\textit{Chemical kinetics}. Harper \& Row.",
    "friston2010free": "Friston, K. (2010). \\textit{The free-energy principle: a unified brain theory?}. Nature reviews neuroscience, 11(2), 127-138.",
    "ratkowsky1982relationship": "Ratkowsky, D. A., Olley, J., McMeekin, T. A., \& Ball, A. (1982). \\textit{Relationship between temperature and growth rate of bacterial cultures}. Journal of bacteriology, 149(1), 1-5.",
    "madigan2014brock": "Madigan, M. T., Martinko, J. M., Bender, K. S., Buckley, D. H., \& Stahl, D. A. (2014). \\textit{Brock biology of microorganisms}. Pearson.",
    "ashrae2017fundamentals": "ASHRAE. (2017). \\textit{ASHRAE Handbook—Fundamentals}. American Society of Heating, Refrigerating and Air-Conditioning Engineers.",
    "nowak2006evolutionary": "Nowak, M. A. (2006). \\textit{Evolutionary dynamics: exploring the equations of life}. Harvard university press.",
    "adams1988energy": "Adams, R. N. (1988). \\textit{The eighth day: Social evolution as the self-organization of energy}. University of Texas Press.",
    "watts1998collective": "Watts, D. J., \& Strogatz, S. H. (1998). \\textit{Collective dynamics of ‘small-world’ networks}. nature, 393(6684), 440-442.",
    "burt2004structural": "Burt, R. S. (2004). \\textit{Structural holes and good ideas}. American journal of sociology, 110(2), 349-399.",
    "kahneman1979prospect": "Kahneman, D., \& Tversky, A. (1979). \\textit{Prospect theory: An analysis of decision under risk}. Econometrica, 47(2), 263-291.",
    "pijanowski2011soundscape": "Pijanowski, B. C., Villanueva-Rivera, L. J., Dumyahn, S. L., Farina, A., Krause, B., Napoletano, B. M., ... \& Pieretti, N. (2011). \\textit{Soundscape ecology: the science of sound in the landscape}. BioScience, 61(3), 203-216.",
    "lehmann2015biochar": "Lehmann, J., \& Joseph, S. (Eds.). (2015). \\textit{Biochar for environmental management: science, technology and implementation}. Routledge.",
    "brillouin2013science": "Brillouin, L. (2013). \\textit{Science and information theory}. Courier Corporation.",
    "wood2014ethereum": "Wood, G. (2014). \\textit{Ethereum: A secure decentralised generalised transaction ledger}. Ethereum project yellow paper, 151(2014), 1-32."
}

chapters_dir = '/Users/raffaine/dev/collective/doc/book_layer1/chapters'
for f_name in os.listdir(chapters_dir):
    if f_name.endswith('.tex'):
        f_path = os.path.join(chapters_dir, f_name)
        with open(f_path, 'r') as f:
            content = f.read()
            
        # Clean up existing broken bibliographies
        content = re.sub(r'\\begin\{thebibliography\}.*?\\end\{thebibliography\}', '', content, flags=re.DOTALL)
        
        # Find citations
        cites = re.findall(r'\\cite\{([^}]+)\}', content)
        chapter_keys = set()
        for cite in cites:
            keys = [k.strip() for k in cite.split(',')]
            chapter_keys.update(keys)
            
        if chapter_keys:
            chap_bib = "\n\n\\begin{thebibliography}{99}\n"
            for key in chapter_keys:
                if key in bib_db:
                    chap_bib += f"\\bibitem{{{key}}}\n{bib_db[key]}\n\n"
                else:
                    chap_bib += f"\\bibitem{{{key}}}\nUnknown citation for {key}.\n\n"
            chap_bib += "\\end{thebibliography}\n"
            
            content = content.strip() + chap_bib
            
            with open(f_path, 'w') as f:
                f.write(content)
