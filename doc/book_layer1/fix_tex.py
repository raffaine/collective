import os, re
d = '/Users/raffaine/dev/collective/doc/book_layer1/chapters'
for f in os.listdir(d):
    if f.endswith('.tex'):
        path = os.path.join(d, f)
        with open(path, 'r') as file:
            content = file.read()
        
        def repl(m):
            s = m.group(1)
            s = s.replace('_', '\\_')
            return f'\\texttt{{{s}}}'
        
        # Match backticks containing words with possible underscores
        new_content = re.sub(r'`([A-Za-z0-9_]+)`', repl, content)
        
        with open(path, 'w') as file:
            file.write(new_content)
