#include "modes/mode_words.hpp"
#include "modes/mode_equations.hpp"
#include "modes/mode_timed.hpp"
#include "modes/mode_stats.hpp"

#include <cstdlib>
#include <ctime>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

int main()
{
    ScreenInteractive screen = ScreenInteractive::Fullscreen();
    srand(time(0));

    bool open_stats_page = false;

    // creating buttons for the different modes and then laying them out horizontally

    auto words_mode_button = Button("WORDS MODE", [&]
                                    { run_words_mode(); });
    auto timed_mode_button = Button("EQUATION MODE", [&]
                                    { run_equations_mode(); });
    auto equation_mode_button = Button("TIMED MODE", [&]
                                       { run_timed_mode(); });

    // these buttons are going to be laid out vertically (relative to the mode buttons)

    auto stats_button = Button("OPEN DASHBOARD", [&]
                               {         
                                open_stats_page = true; 
                                screen.ExitLoopClosure()(); });

    auto quit_button = Button("QUIT", [&]
                              { screen.ExitLoopClosure()(); });

    auto modes_container = Container::Horizontal(
        {words_mode_button, timed_mode_button, equation_mode_button});

    auto menu_container = Container::Vertical({stats_button, quit_button});

    auto main_container = Container::Vertical({
        modes_container,
        menu_container,
    });

    auto ui = Renderer(main_container, [&]
                       { return vbox({
                                    text("RAFTAAR - MAIN MENU"),
                                    separator(),
                                    modes_container->Render(),
                                    menu_container->Render(),
                                }) |
                                center | border; });

    // keep running the loop for the TUI. but if the stats page is opened the loop ends and it runs the stats page
    // and uses SFML for that.

    screen.Loop(ui);

    if (open_stats_page)
        run_stats_page();

    return 0;
}