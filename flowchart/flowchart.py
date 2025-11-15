from graphviz import Digraph

FONT_MAIN = "Arial"
FONT_BOLD = "Arial Bold"
FONT_MONO = "Courier"

COLOR_START = "#27ae60"
COLOR_DECISION = "#e67e22"
COLOR_MODE = "#9b59b6"
COLOR_HEADER = "#2980b9"
COLOR_STATS = "#8e44ad"
COLOR_EXIT = "#e74c3c"
COLOR_NOTE = "#f39c12"
COLOR_UTIL = "#ecf0f1"

NODE_SEP = "0.8"
RANK_SEP = "0.8"

LABEL_TITLE = "RAFTAAR - Program Flowchart"

dot = Digraph(comment="RAFTAAR Flowchart")
dot.attr(rankdir="TB", splines="ortho", nodesep=NODE_SEP, ranksep=RANK_SEP)

dot.attr(label=LABEL_TITLE, labelloc="t", fontsize="20", fontname=FONT_BOLD)

dot.node("start", "START\n(from main())", shape="ellipse", style="filled",
         fillcolor=COLOR_START, fontname=FONT_MAIN)

dot.node("menu", "Main Menu\n(uses FTXUI)", shape="box", style="filled,rounded",
         fillcolor=COLOR_HEADER, fontcolor="white", fontname=FONT_MAIN)

dot.node("choice", "Mode Select?", shape="diamond", style="filled",
         fillcolor=COLOR_DECISION, fontcolor="white", fontname=FONT_MAIN)

dot.attr("node", shape="box", style="filled,rounded", fillcolor=COLOR_MODE,
         fontcolor="white", fontname=FONT_MAIN)

dot.node("words", "WORDS MODE\nrun_words_mode()")
dot.node("equations", "EQUATIONS MODE\nrun_equations_mode()")
dot.node("timed", "TIMED MODE\nrun_timed_mode()")

dot.node("mode_note",
         "Each mode is kept in separate\n.cpp/.hpp files",
         shape="note", style="filled", fillcolor=COLOR_NOTE,
         fontcolor="black", fontsize="10")

dot.node("stats", "STATS PAGE\n(SFML)\nrun_stats_page()",
         fillcolor=COLOR_STATS, fontcolor="white")

dot.attr("node", shape="box", style="filled,rounded",
         fillcolor=COLOR_HEADER, fontcolor="white")

dot.node("gameloop", "Game Loop\nDisplay & Input")
dot.node("calc", "Calculate\nWPM & Accuracy")
dot.node("save", "Save Statistics\nto File")

dot.node("again", "Go Back?",
         shape="diamond", style="filled", fillcolor=COLOR_DECISION)

dot.node("exit", "EXIT", shape="ellipse", style="filled",
         fillcolor=COLOR_EXIT)

dot.attr("node", shape="box", style="filled", fillcolor=COLOR_UTIL,
         fontcolor="black", fontname=FONT_MONO)

dot.node("util1", "calculate_wpm()")
dot.node("util2", "get_random_words()\n(sourced from assets/words_alpha.txt)")

dot.edge("start", "menu")
dot.edge("menu", "choice")

dot.edge("choice", "words")
dot.edge("choice", "equations")
dot.edge("choice", "timed")
dot.edge("choice", "stats")
dot.edge("choice", "exit")

dot.edge("words", "gameloop")
dot.edge("equations", "gameloop")
dot.edge("timed", "gameloop")

dot.edge("gameloop", "calc")
dot.edge("calc", "save")
dot.edge("save", "again")

dot.edge("again", "exit")
dot.edge("again", "choice", style="dashed")

dot.edge("stats", "exit", style="dashed")

dot.edge("calc", "util1", style="dotted", color="#3498db")
dot.edge("calc", "util1", style="dotted", color=COLOR_HEADER)
dot.edge("gameloop", "util2", style="dotted", color=COLOR_HEADER)

dot.render("raftaar_flowchart", format="pdf", cleanup=True)
print("done w flowchart")
