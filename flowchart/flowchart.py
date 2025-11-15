# this is a python script i wrote for generating the flowchart.
# it creates raftaar_flowchart.pdf in the directory.

from graphviz import Digraph

dot = Digraph(comment='RAFTAAR Flowchart')
dot.attr(rankdir='TB', splines='ortho', nodesep='0.8', ranksep='0.8')
dot.attr('node', shape='box', style='filled,rounded', fontname='Arial', fontsize='12')

dot.attr(label='RAFTAAR - Program Flowchart', labelloc='t', fontsize='20', fontname='Arial Bold')
dot.attr('node', shape='box', style='filled,rounded', fillcolor='#2980b9', fontcolor='white')

dot.node('start', 'START\n (from main())', shape='ellipse', fillcolor='#27ae60')
dot.node('menu', 'Main Menu\n (uses FTXUI)')
dot.node('choice', 'Mode Select?', shape='diamond', fillcolor='#e67e22')

dot.attr('node', fillcolor='#9b59b6')
dot.node('words', 'WORDS MODE\nrun_words_mode()')
dot.node('equations', 'EQUATIONS MODE\nrun_equations_mode()')
dot.node('timed', 'TIMED MODE\nrun_timed_mode()')

dot.node('mode_note', 'Each mode is kept in separate\n.cpp/.hpp files', 
         shape='note', fillcolor='#f39c12', fontcolor='black', fontsize='10')

dot.node('stats', 'STATS PAGE\n(SFML)\nrun_stats_page()', fillcolor='#8e44ad')

dot.attr('node', fillcolor='#2980b9')
dot.node('gameloop', 'Game Loop\nDisplay & Input')
dot.node('calc', 'Calculate\nWPM & Accuracy')
dot.node('save', 'Save Statistics\nto File')
dot.node('again', 'Go Back?', shape='diamond', fillcolor='#e67e22')

dot.node('exit', 'EXIT', shape='ellipse', fillcolor='#e74c3c')

dot.attr('node', fillcolor='#ecf0f1', fontcolor='black', shape='box', style='filled')
dot.node('util1', 'calculate_wpm()', fontname='Courier')
dot.node('util2', 'get_random_words()\n(sourced from assets/words_alpha.txt)', fontname='Courier')

dot.edge('start', 'menu')
dot.edge('menu', 'choice')

dot.edge('choice', 'words')
dot.edge('choice', 'equations')
dot.edge('choice', 'timed')
dot.edge('choice', 'stats')
dot.edge('choice', 'exit')

dot.edge('words', 'gameloop')
dot.edge('equations', 'gameloop')
dot.edge('timed', 'gameloop')

dot.edge('gameloop', 'calc')
dot.edge('calc', 'save')
dot.edge('save', 'again')
dot.edge('again', 'exit')
dot.edge('again', 'choice', style='dashed')

dot.edge('stats', 'exit', style='dashed')

dot.edge('calc', 'util1', style='dotted', color='#3498db')
dot.edge('gameloop', 'util2', style='dotted', color='#3498db')

dot.render('raftaar_flowchart', format='pdf', cleanup=True)
print("done w flowchart")
