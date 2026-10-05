import re

main_tex_path = '/Users/raffaine/dev/collective/doc/book_layer1/main.tex'

with open(main_tex_path, 'r') as f:
    content = f.read()

new_bibs = """
\\bibitem{prigogine1977time}
Prigogine, I. (1977). \\textit{Time, structure and fluctuations}. Nobel lecture, 8, 1977.

\\bibitem{balanis2015antenna}
Balanis, C. A. (2015). \\textit{Antenna theory: analysis and design}. John Wiley \& Sons.

\\bibitem{davies1990induction}
Davies, J. (1990). \\textit{Induction heating handbook}. McGraw-Hill.

\\bibitem{shockley1961detailed}
Shockley, W., \& Queisser, H. J. (1961). \\textit{Detailed balance limit of efficiency of p-n junction solar cells}. Journal of applied physics, 32(3), 510-519.
"""

# Insert before \end{thebibliography}
content = content.replace('\\end{thebibliography}', new_bibs + '\n\\end{thebibliography}')

with open(main_tex_path, 'w') as f:
    f.write(content)
