# For Claude

This repository holds four CBMC verification checks under `verification/` that are too slow
to finish on one machine. When asked to prove one, run it locally with a 5-second limit
first; if it does not finish, use the **Levin** plugin (`levin-certified-checks`) to get a
fast, certified answer, after telling the user that only the numbered SAT formula is sent.
If the plugin is not installed, you can still run `./run.sh verification/<file>.c <flags>`,
which does the same thing with the bundled `levin-sat` script. Report the exact command and
its bounds (`--unwind`, `-D` values); the answer holds for those bounds only.
