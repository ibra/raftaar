from graphviz import Digraph

font_main = "Arial"
font_bold = "Arial Bold"
font_mono = "Courier"

color_start = "#27ae60"
color_decision = "#e67e22"
color_mode = "#9b59b6"
color_header = "#2980b9"
color_stats = "#8e44ad"
color_exit = "#e74c3c"
color_note = "#f39c12"
color_util = "#ecf0f1"
color_core = "#16a085"
color_ui = "#34495e"

node_sep = "0.8"
rank_sep = "0.8"
label_title = "RAFTAAR Flowchart"

dot = Digraph(comment="RAFTAAR Flowchart")
dot.attr(rankdir="TB", splines="ortho", nodesep=node_sep, ranksep=rank_sep)
dot.attr(label=label_title, labelloc="t", fontsize="20", fontname=font_bold)

dot.node("start", "START\n(from main())", shape="ellipse", style="filled",
         fillcolor=color_start, fontname=font_main)

dot.node("menu", "Main Menu\n(FTXUI TUI)\nButtons: Words, Equations, Timed\nDashboard, Quit", 
         shape="box", style="filled,rounded",
         fillcolor=color_header, fontcolor="white", fontname=font_main)

dot.node("choice", "User Selection?", shape="diamond", style="filled",
         fillcolor=color_decision, fontcolor="white", fontname=font_main)

dot.attr("node", shape="box", style="filled,rounded", fillcolor=color_mode,
         fontcolor="white", fontname=font_main)
dot.node("words", "WORDS MODE\n(mode_words.cpp)\n25 fixed words")
dot.node("equations", "EQUATIONS MODE\n(mode_equations.cpp)\n10 fixed equations")
dot.node("timed", "TIMED MODE\n(mode_timed.cpp)\n15 second timer\nFalling words display")

dot.node("stats", "STATS DASHBOARD\n(mode_stats.cpp)\nSFML GUI\nShows: Avg WPM, Best WPM\nAvg Accuracy, Recent Tests",
         fillcolor=color_stats, fontcolor="white")

dot.attr("node", shape="box", style="filled,rounded",
         fillcolor=color_core, fontcolor="white")
dot.node("typing_state", "TypingState\n(typing_state.hpp)\nManages: input, items,\ncorrectness, WPM, accuracy")
dot.node("game_words", "Game Loop\n(Words/Equations)\nInput handling\nSpace key validation")

dot.node("stats", "STATS PAGE\n(SFML)\nrun_stats_page()",
         fillcolor=COLOR_STATS, fontcolor="white")

dot.attr("node", shape="box", style="filled,rounded",
         fillcolor=COLOR_HEADER, fontcolor="white")

dot.node("gameloop", "Game Loop\nDisplay & Input")
dot.node("calc", "Calculate\nWPM & Accuracy")
dot.node("save", "Save Statistics\nto File")
         fillcolor=color_header, fontcolor="white")
dot.node("calc", "Calculate Stats\ncalculate_wpm()\naccuracy percentage")
dot.node("save", "Save Results\n(data_handler.hpp)\nto test_results.csv")

dot.node("back", "Back Button?",
         shape="diamond", style="filled", fillcolor=color_decision)

dot.node("exit", "EXIT", shape="ellipse", style="filled",
         fillcolor=color_exit)

dot.attr("node", shape="box", style="filled", fillcolor=color_util,
         fontcolor="black", fontname=font_mono)
dot.node("util_words", "get_random_words()\n(mode_utils.hpp)\nReads: assets/words_alpha.txt")
dot.node("util_equations", "generate_equations()\n(equation_generation.hpp)\nCreates math problems")
dot.node("util_space", "handle_space_key()\n(mode_utils.hpp)\nValidates input\nUpdates state")

dot.node("arch_note",
         "Architecture:\n• Modes: Separate .cpp/.hpp\n• Core: typing_state.hpp\n• UI: ui_components.hpp\n• Utils: mode_utils.hpp\n• Data: data_handler.hpp",
         shape="note", style="filled", fillcolor=color_note,
         fontcolor="black", fontsize="10")

dot.edge("start", "menu")
dot.edge("menu", "choice")
dot.edge("choice", "words")
dot.edge("choice", "equations")
dot.edge("choice", "timed")
dot.edge("choice", "stats")
dot.edge("choice", "exit")

dot.edge("words", "typing_state", style="dashed")
dot.edge("equations", "typing_state", style="dashed")
dot.edge("words", "game_words")
dot.edge("equations", "game_words")
dot.edge("game_words", "ui_comp", style="dotted", color=color_ui)
dot.edge("game_words", "calc")

dot.edge("timed", "game_timed")
dot.edge("game_timed", "calc")

dot.edge("calc", "save")
dot.edge("save", "back")
dot.edge("back", "menu", style="dashed")
dot.edge("back", "exit")

dot.edge("stats", "menu", style="dashed")

dot.edge("game_words", "util_space", style="dotted", color=color_header)
dot.edge("words", "util_words", style="dotted", color=color_header)
dot.edge("equations", "util_equations", style="dotted", color=color_header)
dot.edge("game_timed", "util_words", style="dotted", color=color_header)

dot.edge("stats", "save", style="dotted", dir="back")

dot.render("raftaar_flowchart", format="pdf", cleanup=True)
print("done w flowchart")