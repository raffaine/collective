import os, re

main_tex_path = '/Users/raffaine/dev/collective/doc/book_layer1/main.tex'
chapters_dir = '/Users/raffaine/dev/collective/doc/book_layer1/chapters'

# 1. Extract the centralized bibliography from main.tex
with open(main_tex_path, 'r') as f:
    main_content = f.read()

bib_env_match = re.search(r'\\begin\{thebibliography\}.*?\\end\{thebibliography\}', main_content, re.DOTALL)
if not bib_env_match:
    print("No centralized bibliography found!")
    exit(1)

bib_env = bib_env_match.group(0)

# Extract all bibitems into a dictionary
bib_items = {}
# Find all \bibitem{key} text
items = re.findall(r'\\bibitem\{(.*?)\}(.*?)(?=\\bibitem|\Z|\\end\{thebibliography\})', bib_env, re.DOTALL)
for key, text in items:
    bib_items[key.strip()] = text.strip()

# Remove the centralized bibliography and the TOC line from main.tex
main_content = re.sub(r'\\addcontentsline\{toc\}\{chapter\}\{Bibliography\}\s*', '', main_content)
main_content = main_content.replace(bib_env, '')

with open(main_tex_path, 'w') as f:
    f.write(main_content)

# 2. Process each chapter
for f_name in os.listdir(chapters_dir):
    if f_name.endswith('.tex'):
        f_path = os.path.join(chapters_dir, f_name)
        with open(f_path, 'r') as f:
            content = f.read()
            
        # Find all cited keys in this chapter
        # \cite{key} or \cite{key1,key2}
        cites = re.findall(r'\\cite\{([^}]+)\}', content)
        chapter_keys = set()
        for cite in cites:
            keys = [k.strip() for k in cite.split(',')]
            chapter_keys.update(keys)
            
        if chapter_keys:
            # Build the chapter-specific bibliography
            chap_bib = "\n\n\\begin{thebibliography}{99}\n"
            for key in chapter_keys:
                if key in bib_items:
                    chap_bib += f"\\bibitem{{{key}}}\n{bib_items[key]}\n\n"
                else:
                    print(f"Warning: {key} not found in main bib dictionary.")
            chap_bib += "\\end{thebibliography}\n"
            
            # Remove any existing bibliography in the chapter just in case
            content = re.sub(r'\\begin\{thebibliography\}.*?\\end\{thebibliography\}', '', content, flags=re.DOTALL)
            
            # Append the new one at the very end
            content = content.strip() + chap_bib
            
            with open(f_path, 'w') as f:
                f.write(content)

print("Chapter bibliographies generated successfully.")
