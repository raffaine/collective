import os, re

d = '/Users/raffaine/dev/collective/doc/book_layer1/chapters'
main_tex_path = '/Users/raffaine/dev/collective/doc/book_layer1/main.tex'

all_bib_items = []

# 1. Extract and remove bibliographies from chapters
for f in os.listdir(d):
    if f.endswith('.tex'):
        path = os.path.join(d, f)
        with open(path, 'r') as file:
            content = file.read()
        
        # Find all bibliography blocks
        bib_blocks = re.findall(r'\\begin\{thebibliography\}.*?(.*?)\\end\{thebibliography\}', content, re.DOTALL)
        
        for block in bib_blocks:
            # remove setcounter
            block = re.sub(r'\\setcounter\{enumiv\}\{\d+\}', '', block)
            # extract bibitems
            items = re.findall(r'\\bibitem\{.*?\}.*?(?=\\bibitem|\Z)', block, re.DOTALL)
            all_bib_items.extend(items)
        
        # Remove from content
        new_content = re.sub(r'\\begin\{thebibliography\}.*?\\end\{thebibliography\}', '', content, flags=re.DOTALL)
        
        if new_content != content:
            with open(path, 'w') as file:
                file.write(new_content)

# Clean up duplicate bib items by key
unique_bibs = {}
for item in all_bib_items:
    match = re.search(r'\\bibitem\{(.*?)\}', item)
    if match:
        key = match.group(1)
        # Strip trailing newlines/spaces
        unique_bibs[key] = item.strip()

# 2. Add bibliography to main.tex
with open(main_tex_path, 'r') as file:
    main_content = file.read()

# If it already has a bibliography, remove it
main_content = re.sub(r'\\begin\{thebibliography\}.*?\\end\{thebibliography\}', '', main_content, flags=re.DOTALL)

bib_str = "\\addcontentsline{toc}{chapter}{Bibliography}\n\\begin{thebibliography}{99}\n"
for item in unique_bibs.values():
    bib_str += item + "\n\n"
bib_str += "\\end{thebibliography}\n"

# Insert before \end{document}
main_content = main_content.replace('\\end{document}', bib_str + '\n\\end{document}')

# 3. Change fonts in main.tex
font_replacement = r"""\usepackage{mathpazo} % Palatino font for elegant text and math
\usepackage[scaled=0.95]{helvet} % Helvetica for sans-serif
\usepackage{courier} % Courier for monospace
\linespread{1.05} % Palatino looks better with slightly increased leading"""

main_content = re.sub(r'\\usepackage\{lmodern\}', font_replacement, main_content)

with open(main_tex_path, 'w') as file:
    file.write(main_content)

print("Formatting updated.")
